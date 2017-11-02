#define HASH_INISIZ 64
/*2:*/
#line 72 "./dedup.w"

/*3:*/
#line 78 "./dedup.w"

#include <iostream> 
#include <string> 

/*:3*//*4:*/
#line 87 "./dedup.w"

#include <dirent.h>  

/*:4*//*20:*/
#line 232 "./dedup.w"

#include <sys/stat.h>  

/*:20*//*31:*/
#line 365 "./dedup.w"

#include <fstream> 
#include <cstring> 

/*:31*/
#line 73 "./dedup.w"

/*5:*/
#line 93 "./dedup.w"

class Directory{
private:
/*13:*/
#line 163 "./dedup.w"

Directory(const Directory&);
Directory&operator= (const Directory&);

/*:13*/
#line 96 "./dedup.w"

protected:
/*6:*/
#line 104 "./dedup.w"

DIR*dir;

/*:6*//*14:*/
#line 170 "./dedup.w"

struct dirent entry;

/*:14*/
#line 98 "./dedup.w"

public:
/*7:*/
#line 109 "./dedup.w"

Directory():dir(NULL){}

/*:7*//*8:*/
#line 113 "./dedup.w"

void open(const char*dirname)
{
if(is_open()){
close();
}
dir= opendir(dirname);
}

/*:8*//*9:*/
#line 123 "./dedup.w"

void close()
{
closedir(dir);
dir= NULL;
}

/*:9*//*10:*/
#line 134 "./dedup.w"

bool is_open()const{return dir!=NULL;}

/*:10*//*11:*/
#line 138 "./dedup.w"

Directory(const char*dirname):dir(NULL)
{
open(dirname);
}

/*:11*//*12:*/
#line 147 "./dedup.w"

~Directory()
{
if(is_open()){
close();
}
}

/*:12*//*15:*/
#line 180 "./dedup.w"

char*read()
{
struct dirent*res;
if(dir==NULL){
return NULL;
}
readdir_r(dir,&entry,&res);
if(res==NULL){
return NULL;
}
return entry.d_name;
}

/*:15*/
#line 100 "./dedup.w"

};

/*:5*//*16:*/
#line 202 "./dedup.w"

class FileTreeWalker{
protected:
/*18:*/
#line 219 "./dedup.w"

std::string path;

/*:18*//*21:*/
#line 241 "./dedup.w"

int walk_internal()
{
struct stat st;
int rc= 0;

lstat(path.c_str(),&st);
if(S_ISREG(st.st_mode)){
rc= work(path);
}else if(S_ISDIR(st.st_mode)){
/*22:*/
#line 259 "./dedup.w"

Directory d(path.c_str());
char*p;
size_t oldlen= path.size();
if(path[oldlen-1]!='/'){
path.push_back('/');
oldlen++;
}

while((p= d.read())!=NULL){
/*23:*/
#line 281 "./dedup.w"

if(p[0]=='.'&&(p[1]=='\0'||(p[1]=='.'&&p[2]=='\0'))){
continue;
}

/*:23*/
#line 269 "./dedup.w"

path.append(p);
rc= walk_internal();
if(rc!=0){
break;
}
path.resize(oldlen);
}

/*:22*/
#line 251 "./dedup.w"

}

return rc;
}

/*:21*//*24:*/
#line 289 "./dedup.w"

virtual int work(const std::string&path)
{
std::cerr<<path<<std::endl;
return 0;
}

/*:24*/
#line 205 "./dedup.w"

public:
/*19:*/
#line 223 "./dedup.w"

int walk(const char*root)
{
path.assign(root);
return walk_internal();
}

/*:19*/
#line 207 "./dedup.w"

};

/*:16*//*25:*/
#line 302 "./dedup.w"

class FileSet{
private:/*26:*/
#line 311 "./dedup.w"

static size_t hash(std::istream&);
static bool equal(std::istream&,std::istream&);

/*:26*//*29:*/
#line 339 "./dedup.w"

struct Slot{
char*path;
size_t hash;
};
Slot*hvec;
size_t hsiz;
size_t hcnt;

/*:29*/
#line 304 "./dedup.w"

public:/*30:*/
#line 352 "./dedup.w"

FileSet()
:hvec(new Slot[HASH_INISIZ]())
,hsiz(HASH_INISIZ)
,hcnt(0)
{}
~FileSet(){
for(size_t i= 0;i<hsiz;i++){
delete[]hvec[i].path;
}
delete[]hvec;
}

/*:30*//*32:*/
#line 374 "./dedup.w"

const char*findDuplicate(const char*path)
{
Slot*p,*pos,*end;
std::ifstream f(path);
size_t h= hash(f);
p= pos= &hvec[h%hsiz];
end= &hvec[hsiz];
while(p->path!=NULL){
/*33:*/
#line 393 "./dedup.w"

if(p->hash==h){
if(std::strcmp(p->path,path)==0){
return NULL;
}
std::ifstream fs(p->path);
f.clear();
f.seekg(0);
if(equal(fs,f)){
return p->path;
}
}

/*:33*/
#line 383 "./dedup.w"

p++;
if(p==end){
p= hvec;
}
}
/*34:*/
#line 406 "./dedup.w"

size_t len= std::strlen(path);
p->path= new char[len+1];
std::memcpy(p->path,path,len+1);
p->hash= h;
hcnt++;
if(hcnt> hsiz/2){
/*35:*/
#line 422 "./dedup.w"

Slot*nvec= new Slot[hsiz*2]();
size_t nsiz= hsiz*2;
for(size_t i= 0;i<hsiz;i++){
Slot*p= &hvec[i];
if(p->path==NULL){
continue;
}
Slot*q= &nvec[p->hash%nsiz];
Slot*end= &nvec[nsiz];
while(q->path!=NULL){
q++;
if(q==end){
q= nvec;
}
}
*q= *p;
}
delete[]hvec;
hvec= nvec;
hsiz= nsiz;

/*:35*/
#line 413 "./dedup.w"

}

/*:34*/
#line 389 "./dedup.w"

return NULL;
}

/*:32*/
#line 305 "./dedup.w"

};

/*:25*//*37:*/
#line 451 "./dedup.w"

class Dedup:FileTreeWalker{
protected:
FileSet fset;
virtual int work(const std::string&);
public:
void run(const char*path){walk(path);}
};

/*:37*/
#line 74 "./dedup.w"

/*39:*/
#line 472 "./dedup.w"

int main(int argc,char*argv[])
{
if(argc<2){
std::cerr<<"I need at least one argument, sorry.\n";
return 1;
}
std::cout<<"#!/bin/bash\n";
Dedup dd;
for(int i= 1;i<argc;i++){
dd.run(argv[i]);
}
return 0;
}

/*:39*/
#line 75 "./dedup.w"


/*:2*//*27:*/
#line 315 "./dedup.w"

size_t FileSet::hash(std::istream&f)
{
size_t h= 6549;
while(!f.eof()){
h= h*1558+((unsigned char)f.get()^233);
}
return h;
}

/*:27*//*28:*/
#line 325 "./dedup.w"

bool FileSet::equal(std::istream&f1,std::istream&f2)
{
while(!f1.eof()&&!f2.eof()){
if(f1.get()!=f2.get()){
return false;
}
}
return f1.eof()&&f2.eof();
}

/*:28*//*38:*/
#line 460 "./dedup.w"

int Dedup::work(const std::string&path)
{
const char*dup= fset.findDuplicate(path.c_str());
if(dup!=NULL){
std::cout<<"#Removing "<<path
<<" (duplicate of "<<dup
<<").\n\nrm "<<path<<"\n\n";
}
return 0;
}

/*:38*/
