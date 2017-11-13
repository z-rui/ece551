#include <iostream>
#include <string>

#include "myshell.h"

/* PS1 is the prompt string.
 * I don't see a need to make it modifiable at run time.
 * I believe a space after the dollar sign is correct.
 */
#define PS1 "myShell $ "

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

/* MyShell::runREPL runs the Read-Evaluate-Print Loop.
 */
void MyShell::runREPL()
{
	std::string line;
	while (prompt_input(PS1, line)) {
		if (line == "exit") {
			break;
		}
		Parser::Command cmd;
		bool ok = parser.parse(line.c_str(), cmd);
		if (!ok) {
			std::cout << "Syntax Error\n";
			continue;
		}
		if (cmd.size() == 0) {
			// empty command
			continue;
		}
		if (cmd.size() > 1) {
			std::cout << "Pipes are not implmented yet.\n";
			continue;
		}
		Parser::Command::const_iterator it;
		it = cmd.begin();
		const char *progname = it->argv[0];
		progname = pathSearcher.search(progname);
		if (progname == NULL) {
			std::cout << "Command " << it->argv[0] <<
				" not found" << std::endl;
			continue;
		}
		// No environment variables at the moment.
		const char *envp[] = { NULL };
		int status;
		bool exited;
		exited = runProgram(progname, &it->argv[0], envp, &status);
		std::cout << "Program " <<
			(exited ? "exited with status "
				: "was killed by signal ") <<
			status << std::endl;
	}
}
