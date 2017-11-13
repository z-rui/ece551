#include <string>

#include "myshell.h"

void MyShell::parseLine(std::string& line)
{
	expand_specials(line);
}
