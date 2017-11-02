\font\ninett=cmtt9
\def\cite{\begingroup\smallskip
  \leftskip=.5in\rightskip=.5in\parindent=2em\relax}
\def\endcite{\smallskip\endgroup}
\def\POSIX/{{\mc POSIX\spacefactor1000}}

@* Introduction.  This is a file de-duplication program
for ECE~551 assignment~097. The specification in {\tt README}
is cited below.
\cite A de-duplication program searches through a set of
directories and removes files that are exactly identical to other
files. However, to reduce the risk of catastrophe as you test this
program, you are not actually going to remove the files, but instead,
print a shell script, which would remove the files if you ran it.

\def\+{\item{$-$}}
\+ It must take as command line arguments 1 or more directory names,
   find all regular files (recursively) contained within those
   directories, and find duplicates to remove.

\+ As your program's output is a shell script, it should first print
   {\tt \#!/bin/bash}

\+ Whenever your program finds a duplicated file, it must print lines
   of a shell script to standard output. First, it must print a
   comment indicating what file it is removing and what file it is a
   duplicate of, then it should print an {\tt rm} command, which would
   remove the file. The program should print this output using the
   full pathnames of the files. For example:
\begingroup\ninett\def\\{\par\leavevmode}\smallskip
   \#Removing /home/ece551/test/file1 (duplicate of
   /home/ece551/test/dir1/file2).\\
\\
   rm /home/ece551/test/file1
\smallskip\endgroup
   Your script should have exactly the same format.

\+ Of the identical files, you may remove any of the copies, as long
   as you leave one of them.

\+ Your program must work correctly in the presence of multiple copies
   of the same file (so if {\tt file1}, {\tt file2}, {\tt file3},
   and {\tt file4} are the
   same, your program must delete three of them and leave one;
   however, it is considered correct regardless of which one you
   leave).

\+ Your program should ignore symlinks, as they do not actually
   represent another copy of the data.

\+ Your program may {\bf not} perform an $N^2$ comparison of all of the files
   (checking every file's contents against every other file's
   contents). Instead, you must use a hashtable, in which you hash the
   contents of the files, and only compare files that have the same
   hash. You can expect that we may test on a large number of
   large-ish files and will require your program to complete in a
   reasonable time.

\+ You may assume that the contents of the directories/files will not
   change while your program runs (unless you change them).

\+ Provide a {\tt Makefile} that compiles your code into a program called
   dedup 
\endcite

@ This program is written in \CPLUSPLUS/.
The overall structure is very simple.
Interestingly there're no external functions defined.
@q CITE: c++lib.w is from standard CWEB distribution @>
@q used for formatting C++ standard library types @>
@i c++lib.w
@c
@<headers@>@;
@<class definitions@>@;
@<main program@>@;

@ Some standard headers are included here, as they are the most commonly used.
@<headers@>=
#include <iostream>
#include <string>

@* Directories.
\CPLUSPLUS/ has file streams, but it does not have the concept of directories
(or not until \CPLUSPLUS/17).
\POSIX/ has \hbox{|DIR *|}, which looks like a friend of \hbox{|FILE *|}.
@s DIR FILE
@<headers@>+=
#include <dirent.h> /* |DIR *| and related functions are declared here */

@ But I would like to handle directories in an object-oriented manner.
Thus I define a class to represent a directory.

@<class def...@>=
class Directory {
private:
	@<private fields in |Directory|@>@;
protected:
	@<protected fields in |Directory|@>@;
public:
	@<public fields in |Directory|@>@;
};

@ Of course, a |Directory| holds a |DIR *| under the hood.
@<protected fields in |Directory|@>+=
DIR *dir;

@ The default constructor sets |dir| to NULL,
i.e., no directory has been opened.
@<public fields in |Directory|@>+=
Directory() : dir(NULL) {}

@ Use |open| to open a directory.
@<public fields in |Directory|@>+=
void open(const char *dirname)
{
	if (is_open()) {
		close();
	}
	dir = opendir(dirname);
}

@ Accordingly, use |close| to close a directory.
@<public fields in |Directory|@>+=
void close()
{
	closedir(dir);
	dir = NULL;
}

@ To test if a directory is open, use |is_open|.
It simply tests if |dir| is NULL, because |opendir| returns NULL on failure,
and I manually set |dir| to NULL when a directory is closed.

@<public fields in |Directory|@>+=
bool is_open() const { return dir != NULL; }

@ It's handy to have a constructor that automatically calls |open|.
@<public fields in |Directory|@>+=
Directory(const char *dirname) : dir(NULL)
{
	open(dirname);
}

@ The advantage of using an directory object is that
no resource will be leaked even in strange situations.

@<public fields in |Directory|@>=
~Directory()
{
	if (is_open()) {
		close();
	}
}

@ According to the rule of three, I also need a copy constructor
and an assignment operator.
But I cannot think of any proper way to copy a \hbox{|DIR *|}.
After looking into the implementation of |std::ios_base|
(which is in a similar situation as it does not make much sense
to copy a stream),
I learned a trick to disallow copy-construction and assignment.

@<private fields in |Directory|@>=
Directory(const Directory&);
Directory& operator=(const Directory&);

@ \POSIX/ offers |readdir_r| to read one entry in the directory.
It writes the entry into a |struct dirent|.

@<protected fields in |Directory|@>+=
struct dirent entry;

@ Wrap the call to |readdir_r| in a method.

I can keep on implementing other methods, such as rewind,
seek and tell, but I don't think I'll need them in this
program.  So stop here.

@s dirent int
@<public fields in |Directory|@>+=
char *read()
{
	struct dirent *res;
	if (dir == NULL) {
		return NULL;
	}
	readdir_r(dir, &entry, &res);
	if (res == NULL) {
		return NULL;
	}
	return entry.d_name;
}

@* File tree walker.  There is a library function named {\tt ftw} that
(as its name suggests) does exactly the job.
With a global hashtable to detect duplication, this assignment is done.
This sounds too easy, and makes my just-written |Directory| class useless.
Also, global variables are bad.

So, write a directory walker on my own.

@<class definitions@>=
class FileTreeWalker {
protected:
	@<protected fields in |FileTreeWalker|@>@;
public:
	@<public fields in |FileTreeWalker|@>@;
};

@ The main thing the file tree walker would do is to recursively
access all files.  
I will need to maintain the ``current path'' during the process.

On Linux, there is a constant |PATH_MAX|---no path will have more
bytes than that.
I can store an |char [PATH_MAX]| in my class, but why arbitrary limits?
\CPLUSPLUS/ has |std::string| so there's really no space limitations.

@ @<protected fields in |FileTreeWalker|@>=
std::string path;

@ |walk| will start the tree walking starting from |root|.
@<public fields in |FileTreeWalker|@>=
int walk(const char *root)
{
	path.assign(root);
	return walk_internal();
}

@ Different actions are taken depending on the file type.
I'll need to call |lstat| to get the file type.
@<headers@>+=
#include <sys/stat.h> /* for |struct stat| and related functions */

@ The actual walking process happens in |walk_internal|.
The reason to use another layer of function call is that
I maintain the current path in the field named |path|,
instead of creating temporary strings and pass them to |walk|.

@s stat int
@<protected fields in |FileTreeWalker|@>=
int walk_internal()
{
	struct stat st;
	int rc = 0;
	
	lstat(path.c_str(), &st);
	if (S_ISREG(st.st_mode)) {
		rc = work(path);
	} else if (S_ISDIR(st.st_mode)) {
		@<walk on children in the directory@>@;
	}
	// don't handle other fancy file types
	return rc;
}

@ For directories, I need to go through all its children.

@<walk on children...@>=
Directory d(path.c_str());
char *p;
size_t oldlen = path.size();
if (path[oldlen-1] != '/') {
	path.push_back('/'); /* path separator */
	oldlen++;
}

while ((p = d.read()) != NULL) {
	@<|continue| if |p| is |"."| or |".."|@>@;
	path.append(p); /* child name */
	rc = walk_internal();
	if (rc != 0) {
		break;
	}
	path.resize(oldlen);
}

@  |readdir_r| returns |"."| and |".."| as children of the directory,
but don't recursively walk on them, for an obvious reason.

@<|continue| if |p| is |"."| or |".."|@>=
if (p[0] == '.' && (p[1] == '\0' || (p[1] == '.' && p[2] == '\0'))) {
	continue;
}

@ Another method called |work| actually work on the files.
For debugging purpose, I chose to print out the file path.

@<protected fields in |FileTreeWalker|@>=
virtual int work(const std::string &path)
{
	std::cerr << path << std::endl;
	return 0;
}

@* Set of unique files.
This is a specialized version of a hash table.
Only file path and hash value is stored.
When comparing for equalty,
the content of the file will be re-read on demand.

@<class def...@>=
class FileSet {
private: @<private fields in |FileSet|@>@;
public: @<public fields in |FileSet|@>@;
};

@ To store files in the hash table, I need a hash function
and an equal function.

@<private fields in |FileSet|@>=
static size_t hash(std::istream&);
static bool equal(std::istream&, std::istream&);

@ @c
size_t FileSet::hash(std::istream& f)
{
	size_t h = 6549;
	while (!f.eof()) {
		h = h * 1558 + ((unsigned char) f.get() ^ 233);
	}
	return h;
}

@ @c
bool FileSet::equal(std::istream& f1, std::istream& f2)
{
	while (!f1.eof() && !f2.eof()) {
		if (f1.get() != f2.get()) {
			return false;
		}
	}
	return f1.eof() && f2.eof();
}

@ The hash table is open addressing. I have an array of hash slots.
Each slot contains the file path and the hash value of the file content.

@<private fields in |FileSet|@>=
struct Slot {
	char *path;
	size_t hash;
};
Slot *hvec; /* array of slots */
size_t hsiz; /* size of the above array */
size_t hcnt; /* entries in the hash table */

@ The array of slots is dynamically allocated,
because I do not know in advance how many files I will be processing.

@d HASH_INISIZ 64
@<public fields in |FileSet|@>=
	FileSet()
		: hvec(new Slot[HASH_INISIZ]())
		, hsiz(HASH_INISIZ)
		, hcnt(0)
	{}
	~FileSet() {
		for (size_t i = 0; i < hsiz; i++) {
			delete[] hvec[i].path;
		}
		delete[] hvec;
	}

@ @<headers@>+=
#include <fstream>
#include <cstring>

@ In fact, |FileSet| has only one operation, |findDuplicate|.
It tries to find a file that has been seen and is the duplicate
of the new file.  If found, return the path of the duplicate;
otherwise, return NULL.

@<public fields in |FileSet|@>=
const char *findDuplicate(const char *path)
{
	Slot *p, *pos, *end;
	std::ifstream f(path);
	size_t h = hash(f);
	p = pos = &hvec[h % hsiz];
	end = &hvec[hsiz];
	while (p->path != NULL) {
		@<if found a duplicate, return the stored path@>@;
		p++; /* linear probing */
		if (p == end) {
			p = hvec; /* wrap around */
		}
	}
	@<add |path| to hash table@>@;
	return NULL;
}

@ @<if found a duplicate...@>=
if (p->hash == h) { /* very likely two files are identical */
	if (std::strcmp(p->path, path) == 0) {
		return NULL; /* same file does not count as duplicate */
	}
	std::ifstream fs(p->path);
	f.clear();
	f.seekg(0);
	if (equal(fs, f)) { /* yes, they are indeed identical */
		return p->path;
	}
}

@ @<add |path|...@>=
size_t len = std::strlen(path);
p->path = new char[len+1];
std::memcpy(p->path, path, len+1);
p->hash = h;
hcnt++;
if (hcnt > hsiz / 2) {
	@<rehash@>@;
}

@ When the load factor is greater than $0.5$, a rehashing takes place
and the size of the array is doubled.

There's no need to call |hash| during rehashing since the hash values
are stored.

@<rehash@>=
Slot *nvec = new Slot[hsiz * 2]();
size_t nsiz = hsiz * 2;
for (size_t i = 0; i < hsiz; i++) {
	Slot *p = &hvec[i];
	if (p->path == NULL) {
		continue;
	}
	Slot *q = &nvec[p->hash % nsiz];
	Slot *end = &nvec[nsiz];
	while (q->path != NULL) {
		q++;
		if (q == end) {
			q = nvec;
		}
	}
	*q = *p;
}
delete[] hvec;
hvec = nvec;
hsiz = nsiz;

@* The main program.
It's easy to see how things will work now:
Override |FileTreeWalker::work|, and
in each invocation of |work|, call |FileSet::add|
to add that file into the set (or be told it is a duplicate).


@ @<class def...@>+=
class Dedup : FileTreeWalker {
protected:@/
	FileSet fset;
	virtual int work(const std::string&);
public:@/
	void run(const char *path) { walk(path); }
};

@ @c
int Dedup::work(const std::string &path)
{
	const char *dup = fset.findDuplicate(path.c_str());
	if (dup != NULL) {
		std::cout << "#Removing " << path
			<< " (duplicate of " << dup
			<< ").\n\nrm " << path << "\n\n";
	}
	return 0;
}

@ @<main program@>=
int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cerr << "I need at least one argument, sorry.\n";
		return 1;
	}
	std::cout << "#!/bin/bash\n";
	Dedup dd;
	for (int i = 1; i < argc; i++) {
		dd.run(argv[i]);
	}
	return 0;
}

@* Index.
