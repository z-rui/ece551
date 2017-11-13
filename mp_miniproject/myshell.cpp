#include <iostream>
#include <string>

#include <stdlib.h> /* for getenv */

#include "myshell.h"

/* PS1 is the prompt string.
 * I don't see a need to make it modifiable at run time.
 * I believe a space after the dollar sign is correct.
 */
#define PS1 "myShell $ "

/* Constructer
 */
MyShell::MyShell()
{
	pathSearcher.setPath(getenv("PATH"));
}

/* prompt_input first writes the prompt string,
 * then reads a logical line for the shell.
 *
 * It returns true if std::cin is still good.
 */
static bool prompt_input(const char *prompt, std::string &line)
{
	std::cout << prompt;
	// Currently a logical line is just a physical line
	// (that is teminated by a '\n').
	// But I can make extensions in the future.
	std::getline(std::cin, line);
	return std::cin.good();
}

#include <string.h> /* for strchr */

/* MyShell::runREPL runs the Read-Evaluate-Print Loop.
 */
void MyShell::runREPL()
{
	std::string line;
	while (prompt_input(PS1, line)) {
		if (line == "exit") {
			break;
		}
		// Now (in step 1), no parsing is necessary.
		const char *cmdname = line.c_str();
		const char *progname = pathSearcher.search(cmdname);
		if (progname == NULL) {
			std::cout << "Command " << cmdname <<
				" not found" << std::endl;
			continue;
		}
		// And no arguments or environment variables.
		const char *argv[] = { progname, NULL };
		const char *envp[] = { NULL };
		int status;
		bool exited;
		exited = runProgram(progname, argv, envp, &status);
		std::cout << "Program " <<
			(exited ? "exited with status "
				: "was killed by signal ") <<
			status << std::endl;
	}
}

int main()
{
	MyShell().runREPL();
}
