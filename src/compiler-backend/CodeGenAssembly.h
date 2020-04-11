#include "underlying_functionalities.hpp"
#include <BinaryExpr.h>
#include <LiteralExpr.h>
#include <UnaryExpr.h>
#include <algorithm>
#include <iostream>
#include <named_type.hpp>
#include <string>
#include <utility>
#include <vector>
#include <BinaryExpr.h>
#include <UnaryExpr.h>
#include <LiteralExpr.h>

namespace aeeh {
namespace code_gen {
namespace detail {

// TODO strong types are broken == and method calling does not work and such.

using ErrorCode = fluent::NamedType<size_t, struct ErrorCodeParameter>;

using Label =
    fluent::NamedType<std::string,
                      struct LabelParameter>; //, fluent::Addable,
                                              // fluent::MethodCallable>;

using Data = fluent::NamedType<std::string, struct DataParameter,
                               fluent::Comparable>; //, fluent::MethodCallable>;

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
// TODO maybe could we only work on labels ?
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

Label uniquify(const Label &label,
               const std::vector<Label> &alreadyDefinedLabels) {

  auto uniquifyiedLabel = label;

  while (std::any_of(
      alreadyDefinedLabels.begin(), alreadyDefinedLabels.end(),
      [&](const Label &l) { return l.get() == uniquifyiedLabel.get(); })) {
    uniquifyiedLabel.get() += "_u";
  }
  return uniquifyiedLabel;
}

class Sections {
private:
  std::vector<DataSectionEntry> dataSection_;
  std::vector<BssSectionEntry> bssSection_;
  std::vector<TextSectionEntry> textSection_;

  // TODO this is so hacky ...
  std::vector<Label> alreadyDefinedLabels;

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

  void addTextSectionEntry(const TextSectionEntry &textSectionEntry) {
    // TODO uniquify as well
    textSection_.push_back(textSectionEntry);
  }

  Label addLabel(const Label &label) {
    auto uniquifyied = uniquify(label, alreadyDefinedLabels);
    alreadyDefinedLabels.push_back(uniquifyied);
    return uniquifyied;
  }
};

using AsmPiece = std::string;

inline std::string
printDataSection(const std::vector<DataSectionEntry> &entries) {
  auto result = std::string();
  result += "section .data\n";

  // TODO use replace_all(text,"\n","",10"");
  for (const auto &[label, data] : entries) {
    result += "    " + label.get() + " db \"" + data.get() + "\"\n";
  }
  return result;
}

inline std::string printTextSection(const std::vector<Label> &labels) {
  auto result = std::string();
  result += "section .text\n";

  for (const auto &label : labels) {
    result += "    global " + label.get() + "\n";
  }
  return result;
}

inline std::string
printBssSection(const std::vector<BssSectionEntry> &entries) {
  auto result = std::string();
  result += "section .bss\n";

  for (const auto &[label, numberOfBytes] : entries) {
    result +=
        "    " + label.get() + " resb " + std::to_string(numberOfBytes) + "\n";
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
  auto loopToGenerateStr = sections.addLabel(Label("loop"));
  auto looptoPrintStr = sections.addLabel(Label("loop"));

  asmPiece += "    mov rcx, " + digitString.label.get() +
              "\n" // Let rcx point to the begin of the digitStr
              + loopToGenerateStr.get() +
              ":\n"
              "    mov rdx, 0\n"
              // reset rdx because otherwise it will mess up the division
              "    mov rbx, 10\n"
              "    div rbx\n" // divide rax by 10 -> remainder will be in rdx.
              "    add rdx, 48\n"   // convert to the ascii representation.
              "    mov [rcx], dl\n" // Move the lower byte of rdx to digitStr.
              "    inc rcx\n"
              "    cmp rax, 0\n" // check if we are done
              "    jne " +
              loopToGenerateStr.get() + "\n";

  asmPiece += looptoPrintStr.get() +
              ":\n"
              "    dec rcx\n"
              "    mov rax, 1\n"
              "    mov rdi, 1\n"
              "    mov rsi, rcx\n"
              "    mov rdx, 1\n"
              "    push rcx\n" // We use rcx as a pointer too the character we
                               // printing. But syscalls overwrite rcx:
              "    syscall\n"
              "    pop rcx\n"
              "    cmp rcx, " +
              digitString.label.get() +
              "\n"
              "    jge " +
              looptoPrintStr.get() + "\n";

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

class ExprToAssembly : public Visitor{
  public:
  // TODO get rid of this it is horrid;
  Sections & sections_;
  AsmPiece asmPiece;

  ExprToAssembly(Sections &sections)
    :sections_(sections) {}

inline AsmPiece walkTree(ExprPtr expr) {
  expr->accept(*this);
  return asmPiece;
}

void visit(BinaryExpr &el) 
{
    el.left->accept(*this);
    el.right->accept(*this);

    asmPiece += "    pop rax\n";
    asmPiece += "    pop rbx\n";

    switch (el.op.type()) {
        case Token::PLUS:
            asmPiece += "    add rax, rbx\n";
            asmPiece += "    push rax\n";
            break;
        case Token::MINUS:
            asmPiece += "    sub rax, rbx\n";
            asmPiece += "    push rax\n";
            break;
        case Token::STAR:
            asmPiece += "    mul rbx\n";
            asmPiece += "    push rax\n";
            break;
        case Token::SLASH:
            asmPiece += "    mov rdx, 0\n"; // Otherwise will mess up the division
            asmPiece += "    div rbx\n";
            asmPiece += "    push rax\n";
            break;
        default:
            // TODO no exceptions!
            throw std::runtime_error("Invalid operand of binary expr -> parser what did u do ?");
    }
}

void visit(UnaryExpr &el)
{
    el.right->accept(*this);
    switch (el.op.type()) {
        case Token::MINUS:
            asmPiece += "    pop rax\n";
            asmPiece += "    neg rax\n";
            asmPiece += "    push rax\n";
            break;
        default:
            throw std::runtime_error("Invalid operand of binary expr -> parser what did u do ?");
    }
}

void visit(LiteralExpr &el)
{
  // TODO seems rather shaky to assume this works.
  asmPiece += "    mov rax, " + el.literal.lexeme() + "\n";
  asmPiece += "    push rax\n";
}

};


inline std::string composeProgram(ExprPtr expr) {
  auto sections = Sections();

  auto result = std::string();

  // TODO wrap this somehow
  // We must should always provide a _start routine.
  result += detail::function("_start");
  sections.addTextSectionEntry(Label("_start"));

  result += printStringInline("Launching program", sections);

  auto treeWalker = ExprToAssembly(sections);
  result += treeWalker.walkTree(expr);

  result += "    pop rax\n";

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

  return detail::composeProgram(expr);
}
} // namespace code_gen
} // namespace aeeh
