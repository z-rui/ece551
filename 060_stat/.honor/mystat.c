#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
//This function is for Step 4
char * time2str(const time_t * when, long ns) {
  char * ans = malloc(128 * sizeof(*ans));
  char temp1[64];
  char temp2[32];
  const struct tm * t= localtime(when);
  strftime(temp1,512,"%Y-%m-%d %H:%M:%S",t);  
  strftime(temp2,32,"%z",t);
  snprintf(ans,128,"%s.%09ld %s", temp1, ns, temp2);
  return ans;
}


