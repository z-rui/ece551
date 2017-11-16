#include <stdlib.h> /* for getenv */

#include "myshell.h"

/* Constructer
 */
MyShell::MyShell() : parser(varTab)
{
	const char *path = getenv("PATH");
	if (path != NULL) {
		varTab.setVar("PATH", path);
		pathSearcher.setPath(path);
	}
}

int main()
{
	MyShell().runREPL();
}
