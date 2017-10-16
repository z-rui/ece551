/*4:*/
#line 54 "./mystat.w"

/*2:*/
#line 26 "./mystat.w"

#include <sys/types.h> 
#include <sys/stat.h> 
#include <time.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <pwd.h> 
#include <grp.h> 
#include <unistd.h> 

/*:2*//*6:*/
#line 89 "./mystat.w"

#include <assert.h>  
#include <string.h>  
#include <errno.h>   

/*:6*/
#line 55 "./mystat.w"

/*3:*/
#line 39 "./mystat.w"


char*time2str(const time_t*when,long ns){
char*ans= malloc(128*sizeof(*ans));
char temp1[64];
char temp2[32];
const struct tm*t= localtime(when);
strftime(temp1,512,"%Y-%m-%d %H:%M:%S",t);
strftime(temp2,32,"%z",t);
snprintf(ans,128,"%s.%09ld %s",temp1,ns,temp2);
return ans;
}

/*:3*//*7:*/
#line 107 "./mystat.w"

int printstat(const char*filename)
{
struct stat st;

/*8:*/
#line 123 "./mystat.w"

if(lstat(filename,&st)!=0){

fprintf(stderr,"stat: cannot stat '%s': %s\n",
filename,strerror(errno));
return errno;


}

/*:8*/
#line 112 "./mystat.w"

/*9:*/
#line 135 "./mystat.w"

printf("  File: '%s'\n",filename);

/*:9*//*10:*/
#line 140 "./mystat.w"

{
const char*filetype= NULL;

switch(st.st_mode&S_IFMT){
case S_IFBLK:filetype= "block special file";break;
case S_IFCHR:filetype= "character special file";break;
case S_IFDIR:filetype= "directory";break;
case S_IFIFO:filetype= "fifo";break;
case S_IFLNK:filetype= "symbolic link";break;
case S_IFREG:filetype= "regular file";break;
case S_IFSOCK:filetype= "socket";break;
}

assert(filetype!=NULL);


printf("  Size: %-10lu\tBlocks: %-10lu IO Block: %-6lu %s\n",
st.st_size,st.st_blocks,st.st_blksize,filetype);
}

/*:10*//*11:*/
#line 163 "./mystat.w"

printf("Device: %lxh/%lud\tInode: %-10lu  Links: %lu\n",
st.st_dev,st.st_dev,st.st_ino,st.st_nlink);

/*:11*/
#line 113 "./mystat.w"

return 0;
}

/*:7*/
#line 56 "./mystat.w"

/*5:*/
#line 70 "./mystat.w"

int main(int argc,char**argv)
{
if(argc<2){

fprintf(stderr,
"stat: missing operand\n"
"Try 'stat --help' for more information.\n");
return EXIT_FAILURE;
}

if(printstat(argv[1])!=0){
return EXIT_FAILURE;
}
return EXIT_SUCCESS;
}

/*:5*/
#line 57 "./mystat.w"


/*:4*/
