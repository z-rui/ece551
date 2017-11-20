#ifndef MYSHELL_H
#define MYSHELL_H

#include <vector>
#include <string>
#include <exception>

#include "path.h"
#include "parse.h"

class MyShell {
	class Bug : public std::exception {
		const char *msg;
		const char *what() const throw() { return msg; }
	public:
		Bug(const char *msg) : msg(msg) {}
	};
protected:
	PathSearcher pathSearcher;
	Parser parser;
	VarTab varTab;
	static bool runProgram(const char *progname,
		const char *const *argv,
		const char *const *envp,
		int *status);
	void executeSet(const Parser::Command&);
	void executeExport(const Parser::Command&);
	void executePipes(const Parser::Pipes&);
	void executeCD(const Parser::Command&);
	bool promptInput(std::string &line) const;
	int chdir(const char *);
	void updatePWD();
public:
	MyShell();
	void runREPL();
};

#endif /* MYSHELL_H */
