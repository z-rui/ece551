#include <iostream>
#include <string>

#include "parse.h"
#include "var.h"

const char *maybeNULL(const char *s)
{
	return (s == NULL) ? "(null)" : s;
}

int main()
{
	std::string line;
	VarTab vtab;
	Parser p(vtab);

	vtab.setVar("x", "README");
	p.debug.os = &std::cerr;
	while (std::getline(std::cin, line)) {
		Parser::Pipes c;
		if (!p.parse(line.c_str(), c)) {
			p.reportSyntaxError(std::cerr);
			continue;
		}
		for (Parser::Pipes::iterator it = c.begin(); it != c.end(); ++it) {
			std::cout << "prog:\t" << maybeNULL(it->argv[0]) << std::endl
				<< "stdin:\t" <<  maybeNULL(it->redir[0]) << std::endl
				<< "stdout:\t" << maybeNULL(it->redir[1]) << std::endl
				<< "stderr:\t" << maybeNULL(it->redir[2]) << std::endl
				<< "args: " << std::endl;
			for (const char *const *arg = &it->argv[1]; *arg != NULL; ++arg) {
				std::cout << '\t' << *arg << std::endl;
			}
		}
	}
}
