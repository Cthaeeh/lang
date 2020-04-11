#include <BinaryExpr.h>
#include <LiteralExpr.h>
#include <UnaryExpr.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace aeeh {
namespace code_gen {
namespace detail {

// TODO use better strong types. (There are libraries for this)
struct ErrorCode {
  explicit ErrorCode(size_t code) : code(code) {}

  size_t code;
};

struct Symbol {
  explicit Symbol(const std::string &symbol) : symbol(symbol) {}

  std::string symbol;
};

struct Text {
  explicit Text(const std::string &text) : text(text) {}

  std::string text;
};

struct DataSectionEntry {
  Symbol symbol;
  Text text;
};

struct BssSectionEntry {
  Symbol symbol;
  size_t numberOfBytes;
};

using TextSectionEntry = std::string;

// TODO move this whole sections stuff to seperate file.
// TODO maybe could we only work on symbols ?
DataSectionEntry
uniquify(const DataSectionEntry &entry,
         const std::vector<DataSectionEntry> &existingEntries) {

  auto hasSymbol = [&](const Symbol &symbol) {
    return [&symbol](const DataSectionEntry &e) {
      return e.symbol.symbol == symbol.symbol;
    };
  };

  auto uniquifyiedSymbol = entry.symbol;

  while (std::any_of(existingEntries.begin(), existingEntries.end(),
                     hasSymbol(uniquifyiedSymbol))) {
    uniquifyiedSymbol.symbol += "_u";
  }
  return DataSectionEntry{uniquifyiedSymbol, entry.text};
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
  for (const auto &[symbol, text] : entries) {
    result += "	       " + symbol.symbol + " db \"" + text.text + "\"\n";
  }
  return result;
}

// TODO these are labels not symbols.
inline std::string printTextSection(const std::vector<std::string> &symbols) {
  auto result = std::string();
  result += "section .text\n";

  for (const auto &symbol : symbols) {
    result += "	       global " + symbol + "\n";
  }
  return result;
}

inline std::string
printBssSection(const std::vector<BssSectionEntry> &entries) {
  auto result = std::string();
  result += "section .bss\n";

  for (const auto &[symbol, numberOfBytes] : entries) {
    result += "	        " + symbol.symbol + " resb " +
              std::to_string(numberOfBytes) + "\n";
  }
  return result;
}

inline AsmPiece printStringInline(const std::string &str, Sections &sections) {
  auto entry = sections.addDataSectionEntry({Symbol("printStr"), Text(str)});

  auto asmPiece = AsmPiece();
  asmPiece += "		mov rax, 1\n"; // Id for sys_write is 1.
  asmPiece += "		mov rdi, 1\n"; // standard output.
  asmPiece += "		mov rsi, " + entry.symbol.symbol + "\n";
  asmPiece +=
      "		mov rdx, " + std::to_string(entry.text.text.size()) + "\n";
  asmPiece += "		syscall\n\n";
  return asmPiece;
}

inline AsmPiece printRaxInline(Sections &sections) {

  auto asmPiece = AsmPiece();
  auto digitString = sections.addBssSectionEntry({Symbol("digitStr"), 100});

  // TODO using 1: and 2: as labels just inline doesn't work, because what if two printRaxInline calls come after another.
  asmPiece += "    mov rcx, " + digitString.symbol.symbol +
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
              digitString.symbol.symbol +
              "\n"
              "    jge 2b\n";

  return asmPiece;
}

inline std::string function(const std::string &name) { return name + ":\n"; }

inline std::string sys_exit(const ErrorCode &errorCode) {
  return "		mov rax, 60\n"
         "		mov rdi, " +
         std::to_string(errorCode.code) +
         "\n"
         "		syscall\n";
}

inline std::string composeProgram() {

  auto sections = Sections();

  auto result = std::string();

  // TODO wrap this somehow
  // We must should always provide a _start routine.
  result += detail::function("_start");
  sections.addTextSectionEntry("_start");

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
