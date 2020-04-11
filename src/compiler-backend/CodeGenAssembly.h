#include "underlying_functionalities.hpp"
#include <BinaryExpr.h>
#include <LiteralExpr.h>
#include <UnaryExpr.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <named_type.hpp>

namespace aeeh {
namespace code_gen {
namespace detail {

using ErrorCode = fluent::NamedType<size_t, struct ErrorCodeParameter>;

using Label = fluent::NamedType<std::string, struct LabelParameter>;//, fluent::Addable, fluent::MethodCallable>;

using Data = fluent::NamedType<std::string, struct DataParameter>;//, fluent::MethodCallable>;

struct DataSectionEntry {
  Label label;
  Data data;
};

struct BssSectionEntry {
  Label label;
  size_t numberOfBytes;
};

using TextSectionEntry = Label;

// TODO move this whole sections stuff to seperate file.
// TODO maybe could we only work on symbols ?
DataSectionEntry
uniquify(const DataSectionEntry &entry,
         const std::vector<DataSectionEntry> &existingEntries) {

  auto hasSymbol = [&](const Label &label) {
    return [&label](const DataSectionEntry &e) {
      return e.label.get() == label.get();
    };
  };

  auto uniquifyiedLabel = entry.label;

  while (std::any_of(existingEntries.begin(), existingEntries.end(),
                     hasSymbol(uniquifyiedLabel))) {
    uniquifyiedLabel.get() += "_u";
  }
  return DataSectionEntry{uniquifyiedLabel, entry.data};
}

class Sections {
private:
  std::vector<DataSectionEntry> dataSection_;
  std::vector<BssSectionEntry> bssSection_;
  std::vector<TextSectionEntry> textSection_;

public:
  const std::vector<DataSectionEntry> &dataSection() { return dataSection_; }

  const std::vector<BssSectionEntry> &bssSection() { return bssSection_; }

  const std::vector<TextSectionEntry> &textSection() { return textSection_; }

  DataSectionEntry addDataSectionEntry(const DataSectionEntry &dataEntry) {
    auto uniquifyied = uniquify(dataEntry, dataSection());
    dataSection_.push_back(uniquifyied);
    return uniquifyied;
  }

  BssSectionEntry addBssSectionEntry(const BssSectionEntry &bssEntry) {
    // TODO uniquify, and return the uniqifiey.
    bssSection_.push_back(bssEntry);
    return bssEntry;
  }

  void addTextSectionEntry(const TextSectionEntry &dataEntry) {}
};

using AsmPiece = std::string;

inline std::string
printDataSection(const std::vector<DataSectionEntry> &entries) {
  auto result = std::string();
  result += "section .data\n";

  // TODO use replace_all(text,"\n","",10"");
  for (const auto &[label, data] : entries) {
    result += "	       " + data.get() + " db \"" + data.get()+ "\"\n";
  }
  return result;
}

// TODO these are labels not symbols.
inline std::string printTextSection(const std::vector<Label> &labels) {
  auto result = std::string();
  result += "section .text\n";

  for (const auto &label : labels) {
    result += "	       global " + label.get() + "\n";
  }
  return result;
}

inline std::string
printBssSection(const std::vector<BssSectionEntry> &entries) {
  auto result = std::string();
  result += "section .bss\n";

  for (const auto &[label, numberOfBytes] : entries) {
    result += "	        " + label.get() + " resb " +
              std::to_string(numberOfBytes) + "\n";
  }
  return result;
}

inline AsmPiece printStringInline(const std::string &str, Sections &sections) {
  auto entry = sections.addDataSectionEntry({Label("printStr"), Data(str)});

  auto asmPiece = AsmPiece();
  asmPiece += "		mov rax, 1\n"; // Id for sys_write is 1.
  asmPiece += "		mov rdi, 1\n"; // standard output.
  asmPiece += "		mov rsi, " + entry.label.get() + "\n";
  asmPiece +=
      "		mov rdx, " + std::to_string(entry.data.get().size()) + "\n";
  asmPiece += "		syscall\n\n";
  return asmPiece;
}

inline AsmPiece printRaxInline(Sections &sections) {

  auto asmPiece = AsmPiece();
  auto digitString = sections.addBssSectionEntry({Label("digitStr"), 100});

  // TODO using 1: and 2: as labels just inline doesn't work, because what if two printRaxInline calls come after another.
  asmPiece += "    mov rcx, " + digitString.label.get() +
              "\n" // Let rcx point to the begin of the digitStr
              "1:\n"
              "    mov rdx, 0\n"
              // reset rdx because otherwise it will mess up the division
              "    mov rbx, 10\n"
              "    div rbx\n" // divide rax by 10 -> remainder will be in rdx.
              "    add rdx, 48\n" // convert to the ascii representation.
              "    "
              "    mov [rcx], dl\n" // Move the lower byte of rdx to digitStr.
              "    inc rcx\n"
              "    cmp rax, 0\n" // check if we are done
              "    jne 1b\n";

  asmPiece += "2:\n"
              "    dec rcx\n"
              "    mov rax, 1\n"
              "    mov rdi, 1\n"
              "    mov rsi, rcx\n"
              "    mov rdx, 1\n"
              "    syscall\n"
              "    cmp rcx, " +
              digitString.label.get() +
              "\n"
              "    jge 2b\n";

  return asmPiece;
}

inline std::string function(const std::string &name) { return name + ":\n"; }

inline std::string sys_exit(const ErrorCode &errorCode) {
  return "		mov rax, 60\n"
         "		mov rdi, " +
         std::to_string(errorCode.get()) +
         "\n"
         "		syscall\n";
}

inline std::string composeProgram() {

  auto sections = Sections();

  auto result = std::string();

  // TODO wrap this somehow
  // We must should always provide a _start routine.
  result += detail::function("_start");
  sections.addTextSectionEntry(Label("_start"));

  result += printStringInline("I am going to print register rax'", sections);
  result += printRaxInline(sections);
  result += printStringInline("'Done will exit with 0", sections);
  result += sys_exit(ErrorCode(0));
  result += "\n";

  result += printTextSection(sections.textSection());
  result += printDataSection(sections.dataSection());
  result += printBssSection(sections.bssSection());
  return result;
}

} // namespace detail

inline std::string generateAssembly(ExprPtr expr) {
  std::cout << "Generating assembly ...\n";

  return detail::composeProgram();
}
} // namespace code_gen
} // namespace aeeh
