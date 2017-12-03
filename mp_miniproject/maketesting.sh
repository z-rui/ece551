#!/bin/bash

prologue="\
Please start myShell by running

    ./myshell

or

    valgrind ./myshell

if detecting memory errors is desired.

You should see that myShell prints \"myShell:...\$\", where \"...\" is
the current working directory.

If you simply hit the enter key without typing anything,
you will see another prompt string.

All the following commands are run inside myShell.

"

epilogue="
That's all.  Now please quit myShell by typing

    exit

or by hitting Ctrl-D to send an EOF.  The program will quit gracefully,
and if valgrind was running, you will see messages that there is no
leaked memory except the suppressed one."

testcaseno=0

Run()
{
	theprogram="$1"
	theoutput="$2"
	thereason="$3"
	testcaseno=$((testcaseno+1))
	echo -e "($testcaseno) Run\n\n    $theprogram\n"
	echo -e "You will see $theoutput\n"
	echo -e "This is correct because $thereason\n"
}

##### Let's start testing! #####

echo "$prologue"

##### Step 1 #####

Run "/bin/ls" \
	"a list of files and directories in the current directory,
followed by a line

    Program exited with status 0

(unless /bin/ls exited with errors for some strange reason).
You will also see the prompt \"myShell:...\$\" again.  (I will omit this
description in the subsequent test cases.)" \
	"myShell runs the program /bin/ls, and that program
does the listing.
The exit status is captured and printed by myShell.  After the program exits,
myShell prints another prompt string and waits for your input."
the_binls_testcase=$testcaseno

Run "/usr/bin/gcc" "\n
    gcc: fatal error: no input files
    compilation terminated.
    Program exited with status 1" \
	"myShell runs the program /usr/bin/gcc, which is
unhappy about the command line arguments, and exits with status 1."
the_gcc_testcase=$testcaseno

echo "NOTE: the next test case uses a program called segfault.
It is in the same directory as myshell.
So please make sure you are in the same directory so that
myShell can find it.
"

Run "./segfault" "\n
    Program was killed by signal 11" \
	"myShell runs the program ./segfault, which simply
segfaults, and is killed by signal 11 (SIGSEGV)."

##### Step 2 #####

Run "ls" "the same output as in ($the_binls_testcase).
(unless /bin was somehow removed from PATH)." \
	"\"ls\" does not contain a slash, so myShell searches for
an executable named \"ls\" in the directories specified by the environment
variable PATH.  Because /bin is in PATH, myShell eventually finds /bin/ls,
and runs the program."

nosuchprogram="myShell"
Run "$nosuchprogram

(can be any name that does not name an executable in PATH)." "\n
    Command $nosuchprogram not found" \
	"myShell searches for \"$nosuchprogram\", and
it cannot be found (unless there is an executable named myShell somewhere
in PATH directories).  As a result, an error message is printed."

Run "../bin/ls" "\n
    Command ../bin/ls not found

(unless the file ../bin/ls happens to exist, e.g, the working directory
is /bin)." \
	"\"../bin/ls\" contains a slash, so myShell does not
search in PATH, and therefore will not find a path like \"/bin/../bin/ls\"
that actually names an executable."

Run "." "\n
    Command . not found" \
	". is a directory, and myShell will not execute
a directory."

echo "NOTE: the next test case uses a file named README.
So please make sure myShell can find it.
"

Run "./README" "\n
    execve: Permission denied" \
	"./README does not have the execution permission,
so it cannot be executed."

Run "echo hello, world" "\n
    hello, world
    Program exited with status 0" \
	"/bin/echo is run, with two arguments:
\"hello,\" and \"world\".  echo prints the arguments and
exits with status 0."
the_echo_testcase=$testcaseno

Run "echo  hello,    world

(the spaces can also be replaced with tabs)." \
	"the same output as in ($the_echo_testcase)." \
	"consecutive spaces and tabs is equivalent to a single
space when myShell parses the command line."

Run 'echo  hello,\ \ world' "\n
    hello,  world
    Program exited with status 0" \
	"only one argument is passed to echo:
\"hello,  world\".  The spaces are escaped, so they do not split
arguments, and go into the argument as is."
the_echo_testcase2=$testcaseno

Run 'echo  hello,\      world' \
	"the same output as in ($the_echo_testcase2)." \
	"two arguments are passed to echo:
\"hello, \" and \"world\".  The backslash escapes the space that immediately
follows, but the subsequent non-escaped spaces separate two arguments.
There are two spaces in the output, between ',' and 'w',
because one is from the first argument and the other is inserted by echo."

Run 'echo \\\\' "\n
    \\\\

(And the exit status, which I will omit in subsequent test cases
if it's unimportant.)" \
	"in my implementation, a backslash followed by
any character translates to the that character, with no special meaning
attached to it.  (Same behavior as in other shells I have experimented with.)"
the_backslash_testcase=$testcaseno

Run 'echo \

(the backslash is the last character on the line).' \
	"a blank line is printed." \
	"in my implementation, a backslash followed by nothing
translates to nothing."

##### Step 3 #####

Run 'cd  /bin' \
	"the current working directory as shown in the prompt
string changes to /bin." \
	"the cd command changes the working directory.
The amount of spaces between cd and the directory does not matter."

Run 'cd' "\n
    Syntax error: unexpected <newline>" \
	"the cd command takes exactly one argument.
Since no argument is given, myShell complains."

Run 'cd a b c' "\n
    Syntax error: unexpected 'b'" \
	"the cd command takes exactly one argument.
Since three arguments are given, myShell complains."

Run 'cd /root' "\n
    chdir: Permission denied

and the working directory is not changed (unless you have the permission
to access /root)." \
	"when myShell fails to change the directory,
it reports an error and stays where it was."

Run 'set x world
    echo hello, $x' \
	"the same output as in ($the_echo_testcase)." \
	"when executing \"set x world\", the value
of x is set to \"world\".  When executing \"echo hello, \$x\",
\$x is expanded to its value \"world\".
Therefore it's essentially the same as ($the_echo_testcase)."

Run 'set x echo hello,
    $x world' \
	"the same output as in ($the_echo_testcase)." \
	"in the first command, x is set to
\"echo hello,\".  Note that the space also goes into the variable.
In the second command, \$x is first expanded to its value,
then myShell reads \"echo\" as the program name, and \"hello,\"
as the first argument for echo.  When the expansion finishes,
\"world\" is read as the second argument for echo.
Therefore it's essentially the same as ($the_echo_testcase)."

Run 'set  x   hello,    world
    echo $x' \
	"the same output as in ($the_echo_testcase)." \
	"in the first command, x is set to
\"hello,    world\".  Note that consecutive spaces are preserved.
In the second command, \$x is expanded to its value.
Inside expansion, spaces still separate arguments, so \$x expands
to two arguments to echo, namely \"hello,\" and \"world\".
(Same behavior as in other shells I have experimented with.)
Therefore it's essentially the same as ($the_echo_testcase)."

Run 'set x hello,\  world
    echo $x' \
	"the same output as in ($the_echo_testcase)." \
	"in the first command, the escape sequence '\ ' is
first translated to a single space (following the rule described in
($the_backslash_testcase)), then x is set to \"hello,  world\".
Note that there are two consecutive spaces.
The remaining explanation is the same as in the last test case."

Run 'set x x
    set x $x$x
    echo $x' "\n
    xx" \
	"the first command sets x to \"x\".
In the second command, each \$x is expanded to its value \"x\".
So it is the same as \"set x xx\".
In the third command, \$x is expanded to its value \"xx\".
So it is the same as \"echo xx\"."
the_set_testcase=$testcaseno

Run 'set x x
    set $x $x$x
    echo $x' \
	"the same output as in ($the_set_testcase)." \
	"in the second command, each \$x is expanded to \"x\".
So the second command is the same as \"set x xx\".
Therefore it's essentially the same as ($the_set_testcase)."

Run 'set x x
    set $x$x $x$x
    echo $xx' \
	"\n
    xx" \
	"in the second command, each \$x is expanded to \"x\".
So the second command is the same as \"set xx xx\".
In the third command \$xx is expanded to its value \"xx\"."

Run 'set x y
    set $x$x $x$x
    echo $x$x' \
	"\n
    yy" \
	"in the second command, each \$x is expanded to \"y\"
(instead of: x\$x -> \"xy\", then \$xy -> \"\").
So the second command is the same as \"set yy yy\".
In the third command, each \$x is expanded to its value \"y\",
so it's the same as \"echo yy\"."

Run 'echo $y' \
	"a blank line is printed (assuming the variable y
was not previously defined)." \
	"a non-existent variable expands to an empty string."

Run 'set x
    echo $x' \
	"a blank line is printed." \
	"the command \"set x\" sets x to an empty string."

Run 'set my_var hello, world
    echo $my_var' "the same output as in ($the_echo_testcase)." \
	"multi-character variable names work the same way
as single-character ones.  Also, \"my_var\" is a valid variable name."

Run 'set _myvar hello, world
    echo $_myvar' "the same output as in ($the_echo_testcase)." \
	"\"_myvar\" is also a valid variable name."

Run 'set 42 hello, world
    echo $42' "the same output as in ($the_echo_testcase)." \
	"\"42\" is also a valid variable name."

Run 'set 4.2' "\n
    Syntax error: unexpected '.'" \
	"'.' is not a valid character in the variable name."

Run 'set 4\2' "\n
    Syntax error: unexpected '\\'" \
	"the backslash is not a valid character
in the variable name.  myShell does not translate \\2 to 2 when parsing
a variable name."

Run 'set x y
    echo $x...' "\n
    y..." \
	"in the second command, the longest sequence
following \$ that can be a valid variable name is \"x\", so \$x is expanded
to its value \"y\", and the second command is the same as \"echo y...\"."

Run 'echo $...' "\n
    $..." \
	"the characters following \$ cannot compose
a valid variable name, so \$ is interpreted as a plain dollar sign,
and no expansion takes place."

Run 'echo $' "\n
    $" \
	"of essentially the same reason for the last test case."

Run 'set x $
    echo $x' "\n
    $" \
	"in the first command, the \$ does not introduce
an expansion, as no valid characters follow, so x is set to \"\$\"."

Run 'set x y
    set x \$x
    echo $x' "\n
    \$x" \
	"in the second command, '\\\$' translates to a plain
dollar sign, so x is set to \"\$x\".  In the third command,
\$x is expanded to \"\$x\", and is not further expanded,
because in my implementation, there is only one-level expansion.
(Same behavior as in other shells I have experimented with.)"

Run 'set x x
    echo $$x' "\n
    \$x" "the first $ does not introduce an expansion,
and is translated to a plain dollar sign.  The \$x that immediately follows
is expanded to \"x\", but the resulting \"\$x\" is not further expanded,
because in my implementation, there is only one-level expansion."

Run 'bash -c kill\ $$' "\n
    Program was killed by signal 15" \
	"bash is run with two arguments: \"-c\" and 
\"kill \$\$\".  When bash is running it kills itself.  Therefore myShell
reports accordingly."

Run 'set x x
    env' \
	"a list of environment variables, but x is not in the list
(assuming x was not in the environment when myShell started, and has not been
exported)." \
	"variables that are not exported does not show up
in the environment."

Run 'set x x
    export x
    env' \
	"a list of environment variables, and x is in the list,
and its value is \"x\"." \
	"the export command exports the variable
to the environment."

Run 'set x x
    export x
    set x xx
    env' \
	"a list of environment variables, and x is in the list,
and its value is \"xx\"." \
	"if the value of an exported variable is modified,
its value is updated in the environment."

Run 'export' "\n
    Syntax error: unexpected <newline>" \
	"the export command takes exactly one argument.
Since no argument is given, myShell complains."

Run 'export x y z' "\n
    Syntax error: unexpected 'y'" \
	"the export command takes exactly one argument.
Since three arguments are given, myShell complains."

Run 'export x...' "\n
    Syntax error: unexpected '.'" \
	"'.' is not a valid character in a variable name."

Run 'export x\y', "\n
    Syntax error: unexpected '\\'" \
	"the backslash is not a valid character
in a variable name."

Run 'set PATH
    ls' "\n
    Command ls not found" \
	"the value of PATH determines where myShell searches
for an executable.  When PATH is set to empty, it cannot find the executable
named \"ls\"."

echo -e "NOTE: please restore the PATH variable now.\n"

##### Step 4 #####

echo "NOTE: the next test case, and a few others that follow,
uses a file named README.  So please make sure myShell can find it.
"

Run 'cat < README' \
	"the content of README." \
	"when run with no arguments, cat copies from
the standard input to the standard output.  In this case, the standard input
is redirected so that cat will read the content of README, and write to
the standard output."

Run 'ls > ls_output
    cat ls_output' \
	"only the exit status for the first command,
and a list of files in the current directory for the second command." \
	"the output of ls is redirected to the file
\"ls_output\", so there is no output when running the first command.
The second command shows the content of \"ls_output\"."

Run 'gcc > gcc_output' "the same output as in ($the_gcc_testcase)." \
	"only the standard output is redirected,
but in this case gcc writes the error messages to the standard error,
so they still shows up."

Run 'gcc 2> gcc_output
    cat gcc_output' \
	"only the exit status for the first command,
and the error messages from gcc for the second command." \
	"in the first command, the standard error is redirected
to \"gcc_output\", so the error messages go to the file, and does not
show up until the second command is run."

Run 'wc -l < README > wc_output
    cat wc_output' \
	"only the exit status for the first command, and

    62

(i.e., the number of lines in README) for the second command." \
	"wc reads from README, and writes to \"wc_output\"
due to the redirections."
the_wc_testcase=$testcaseno

Run 'wc -l<README>wc_output
    cat wc_output' "the same output as in ($the_wc_testcase)." \
	"the spaces around '<' and '>' do not matter."

Run 'wc -l > wc_output < README
    cat wc_output' "the same output as in ($the_wc_testcase)." \
	"the order of '<' and '>' does not matter."

Run 'wc < README > wc_output -l
    cat wc_output' "the same output as in ($the_wc_testcase)." \
	"the order of redirections and the remaining part
of the command does not matter.  (Same behavior as other shells I have
experimented with.)"

Run '< README > wc_output wc -l
    cat wc_output' "the same output as in ($the_wc_testcase)." \
	"of the same reason for the last test case."

Run 'cat <' "\n
    Syntax error: unexpected <newline>" \
	"the filename for input redirection is missing."

Run 'ls >> ls_output' "\n
    Syntax error: unexpected '>'" \
	"after the first '>', the filename for
output redirection is missing. "

Run 'echo 3> echo_output' \
	"only the exit status, but the content
of the file \"echo_output\" is changed to \"3\"." \
	"the command is interpreted as:
  program: echo
  argument: 3
  output: echo_output."

Run '< README' "no output." \
	"no program name is given, so myShell
treats the input as an empty command."

Run 'set x README
    wc -l < $x > wc_output
    cat wc_output' "the same output as in ($the_wc_testcase)." \
	"in the second command, \$x is expanded to \"README\"."

Run 'set x hello, world < /dev/null > /dev/null | cat
    echo $x' "\n
    hello, world < /dev/null > /dev/null | cat" \
	"the characters '<', '>' and '|' do not have their
special meanings in the contexts of:
(1) variable definition.  As a result, in the first command,
    the entire part of the line after the variable name goes into
    the variable x.  (i.e., the command is not interpreted as
    \"set x hello, world\" and then redirected and piped.)
(2) variable expansion.  As a result, there is no redirections or pipes here.
    (Same behavior as in other shells I have experimented with.)"

Run 'echo 518518518 / 12345678 | bc' "\n
    Program exited with status 0 (PID = ...)
    42
    Program exited with status 0 (PID = ...)

(\"...\" stand for the actual PIDs for \"echo\" and \"bc\")." \
	"\"echo\" writes a line \"518518518 / 12345678\",
and the line of output is piped to \"bc\", which does the calculation."
the_bc_testcase=$testcaseno

Run 'echo 518518518 / 12345678|bc' "similar output as in ($the_bc_testcase)." \
	"the spaces around '|' do not matter."

Run 'echo 518518518 / 12345678 | cat | bc' "\n
    Program exited with status 0 (PID = ...)
    Program exited with status 0 (PID = ...)
    42
    Program exited with status 0 (PID = ...)

(\"...\" stand for the actual PIDs for \"echo\", \"cat\" and \"bc\")." \
	"\"echo\" writes a line \"518518518 / 12345678\",
and the line of output is piped to \"cat\", and then to \"bc\",
which does the calculation."

Run 'cat -u < README | cat | wc -l > wc_output' "\n
    Program exited with status 0 (PID = ...)
    Program exited with status 0 (PID = ...)
    Program exited with status 0 (PID = ...)

(\"...\" stand for the actual PIDs for \"cat\", \"cat\" and \"wc\")." \
	"the content of README is read by the first cat,
then piped to the second cat, then piped to wc.  Then wc writes its output
to \"wc_output\".  If you check the content of \"wc_output\", it should be
the number of lines of README."

Run 'set x /dev/null
    cat -u < README > $x | cat < $x > $x | wc -l < $x > wc_output' "\n
    Program exited with status 0 (PID = ...)
    Program exited with status 0 (PID = ...)
    Program exited with status 0 (PID = ...)

(\"...\" stand for the actual PIDs for \"cat\", \"cat\" and \"wc\")." \
	"myShell silently accepts this nonsensical command
and assumes redirections override the pipes.
If you check the content of \"wc_output\", it should be 0."

Run 'dd if=/dev/zero of=/dev/stdout bs=4096 count=4096 | cat | wc -c' "\n
    4096+0 records in
    4096+0 records out
    16777216 bytes (17 MB, 16 MiB) copied, ...
    Program exited with status 0 (PID = ...)
    16777216
    Program exited with status 0 (PID = ...)
    Program exited with status 0 (PID = ...)

(\"...\" stand for output that may vary in different runs)." \
	"16777216 bytes are piped from dd to cat and
then piped to wc."

Run 'cat /dev/zero | true' "\n
    Program was killed by signal 13 (PID = ...)
    Program exited with status 0 (PID = ...)

(\"...\" stand for the actual PIDs for \"cat\", and \"true\")." \
	"true exits before cat finishes reading the file.
So the pipe is no longer valid, and cat is terminated by signal 13 (SIGPIPE)."

Run 'cat /dev/zero |' "\n
    Syntax error: unexpected <newline>" \
	"the command after '|' is missing."

Run 'cat /dev/zero |<README' "\n
    Program was killed by signal 13" \
	"the right hand side of '|' is an empty command,
no program is actually run.  cat receives a SIGPIPE because the pipe is closed."

##### We are done! #####

echo "$epilogue"
