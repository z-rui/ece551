#include <stdlib.h> /* for getenv */

#include "myshell.h"

/* Constructer
 */
MyShell::MyShell()
{
	pathSearcher.setPath(getenv("PATH"));
}

int main()
{
	MyShell().runREPL();
}
