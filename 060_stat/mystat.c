#define MODESTR_LEN 10
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
#line 90 "./mystat.w"

#include <assert.h>  
#include <string.h>  
#include <errno.h>   

/*:6*/
#line 55 "./mystat.w"

/*14:*/
#line 199 "./mystat.w"

char*get_modestr(char*buf,int mode);
/*:14*//*17:*/
#line 235 "./mystat.w"

void get_permissions(char*buf,int mode,int rmask,int wmask,int xmask);
/*:17*/
#line 56 "./mystat.w"

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
#line 108 "./mystat.w"

int printstat(const char*filename)
{
struct stat st;

/*8:*/
#line 124 "./mystat.w"

if(lstat(filename,&st)!=0){

fprintf(stderr,"stat: cannot stat '%s': %s\n",
filename,strerror(errno));
return errno;


}

/*:8*/
#line 113 "./mystat.w"

/*9:*/
#line 136 "./mystat.w"

printf("  File: '%s'\n",filename);

/*:9*//*10:*/
#line 141 "./mystat.w"

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
#line 164 "./mystat.w"

printf("Device: %lxh/%lud\tInode: %-10lu  Links: %lu\n",
st.st_dev,st.st_dev,st.st_ino,st.st_nlink);


/*:11*//*12:*/
#line 174 "./mystat.w"

/*13:*/
#line 188 "./mystat.w"

{
char modestr[MODESTR_LEN+1];

printf("Access: (%04o/%s)",st.st_mode&~S_IFMT,
get_modestr(modestr,st.st_mode));
}

/*:13*//*21:*/
#line 266 "./mystat.w"

{
struct passwd*pw;

pw= getpwuid(st.st_uid);
printf("  Uid: (%5d/%8s)",st.st_uid,pw->pw_name);
}

/*:21*//*22:*/
#line 275 "./mystat.w"

{
struct group*gr;

gr= getgrgid(st.st_gid);
printf("   Gid: (%5d/%8s)",st.st_gid,gr->gr_name);
}


/*:22*/
#line 175 "./mystat.w"

printf("\n");

/*:12*/
#line 114 "./mystat.w"

return 0;
}

/*:7*//*15:*/
#line 201 "./mystat.w"

char*get_modestr(char*buf,int mode)
{
/*16:*/
#line 216 "./mystat.w"

{
char c= 0;
switch(mode&S_IFMT){
case S_IFBLK:c= 'b';break;
case S_IFCHR:c= 'c';break;
case S_IFDIR:c= 'd';break;
case S_IFIFO:c= 'p';break;
case S_IFLNK:c= 'l';break;
case S_IFREG:c= '-';break;
case S_IFSOCK:c= 's';break;
}
assert(c!=0);
buf[0]= c;
}

/*:16*/
#line 204 "./mystat.w"

/*19:*/
#line 249 "./mystat.w"

get_permissions(buf+1,mode,S_IRUSR,S_IWUSR,S_IXUSR);
get_permissions(buf+4,mode,S_IRGRP,S_IWGRP,S_IXGRP);
get_permissions(buf+7,mode,S_IROTH,S_IWOTH,S_IXOTH);


/*:19*/
#line 205 "./mystat.w"

buf[MODESTR_LEN]= '\0';
return buf;
}

/*:15*//*18:*/
#line 237 "./mystat.w"

void get_permissions(char*buf,int mode,int rmask,int wmask,int xmask)
{
buf[0]= (mode&rmask)?'r':'-';
buf[1]= (mode&wmask)?'w':'-';
buf[2]= (mode&xmask)?'x':'-';
}

/*:18*/
#line 57 "./mystat.w"

/*5:*/
#line 71 "./mystat.w"

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
#line 58 "./mystat.w"


/*:4*/
