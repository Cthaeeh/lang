#include <iostream>
#include <BinaryExpr.h>
#include <UnaryExpr.h>
#include <LiteralExpr.h>
#include <string>
#include <vector>

namespace aeeh {
namespace code_gen_detail {

	// TODO use better strong types.
	struct ErrorCode{
		explicit ErrorCode(size_t code)
			:code(code) {}

		size_t code;
	};

	struct SectionEntry{
		std::string symbol;
		std::string text;
	};

	inline std::string data_section(
			const std::vector<SectionEntry> &dataSectionEntries)
	{
		auto result = std::string();
		result += "section .data\n";

		for (const auto& [symbol, text] : dataSectionEntries) {
			result += "	       " + symbol + " db \"" + text + "\"\n";
		}
		return result; 	
	}

	// TODO these are labels not symbols.
	inline std::string text_section(const std::vector<std::string> &symbols)
	{
		auto result = std::string();
		result += "section .text\n";

		for (const auto& symbol : symbols) {
			result += "	       global " + symbol + "\n";
		}
		return result; 	
	}

	inline std::string systemWrite(const SectionEntry& sectionEntry) {
		auto result = std::string();
		result += "		mov rax, 1\n";// Id for sys_write is 1.
	    result += "		mov rdi, 1\n";	 // standard output.
	    result += "		mov rsi, " + sectionEntry.symbol + "\n";
	    result += "		mov rdx, " + std::to_string(sectionEntry.text.size()) + "\n";
	    result += "		syscall\n\n";
		return result;
	}

	inline std::string function(const std::string &name) {
		return name + ":\n";
	}

	inline std::string sys_exit(const ErrorCode &errorCode) {
		return "		mov rax, 60\n"
			   "		mov rdi, " + std::to_string(errorCode.code) + "\n"
			   "		syscall\n";
	}

}

namespace code_gen {

	inline std::string generateAssembly(ExprPtr expr) {
		std::cout << "Generating assembly ...\n";
		
		// Make one data section entry with symbol text.
		auto dataSectionEntries = 
							std::vector<code_gen_detail::SectionEntry>
			{{"text","Hello world from aeeh::code_gen!\\n"}};

		auto result = std::string();

		result += code_gen_detail::data_section(dataSectionEntries);
		result += code_gen_detail::text_section({"_start"});

		result += code_gen_detail::function("_start");
		result += code_gen_detail::systemWrite(dataSectionEntries.at(0));
		result += code_gen_detail::sys_exit(code_gen_detail::ErrorCode(0));

		return result;
	}
}
}
