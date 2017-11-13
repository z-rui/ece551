#ifndef MYSHELL_H
#define MYSHELL_H

#include <vector>
#include <string>
#include <exception>

class MyShell {
	class Bug : public std::exception {
		const char *msg;
		const char *what() const throw() { return msg; }
	public:
		Bug(const char *msg) : msg(msg) {}
	};
protected:
	static bool runProgram(const char *, const char *const *, const char *const *, int *);
public:
	void runREPL();
};

#endif /* MYSHELL_H */
