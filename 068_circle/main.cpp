#include "circle.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


//You might notice a LOT of code duplication with repsect
//to the previous problem:  we will see later how we could
//avoid the duplication.

char * skipWs (char * line) {
  while(*line != '\0' && isspace(*line)) {
    line++;
  }
  return line;
}

int readInt(char ** linep, int * out) {
  char * temp;
  *out = strtol(*linep, &temp, 0);
  if (*linep == temp) {
    return 0; //could not read an int!
  }
  *linep = temp; //update line pointer.
  return 1; //succeeded!
}

//this duplication of code is not good, but
//you all have not learned how we could avoid it
//cleanly.
int readDouble(char ** linep, double * out) {
  char * temp;
  *out = strtod(*linep, &temp);
  if (*linep == temp) {
    return 0; //could not read an double!
  }
  *linep = temp; //update line pointer.
  return 1; //succeeded!
}
//line points just past any whitespace after the "m" that designates the command
//first parse an int (circle num) and two doubles (dx, dy)
//then move the appropriate circle.
void doMove (char * line, Circle ** circles, int ncircles) {
  int cnum;
  if (!readInt (&line, &cnum)) {
    fprintf(stderr,"Circle number of move command requires an integer!\n");
    return;
  }
  if (cnum < 0 || cnum >= ncircles) {
    fprintf(stderr, "Invalid circle number! %d is out of range [0,%d)\n", cnum, ncircles);
    return;
  }
  line = skipWs (line);
  double dx;
  if (!readDouble (&line, &dx)) {
    fprintf(stderr,"dx of move command requires a double!\n");
    return;
  }
  line = skipWs (line);
  double dy;
  if (!readDouble (&line, &dy)) {
    fprintf(stderr,"dx of move command requires a double!\n");
    return;    
  }
  circles[cnum]->move(dx,dy);
}
//line points just past any whitespace after the "i" that designates the command
//first parse the two ints that name the circles.
//then print the area of the intersection between them.
void doIntersect (char * line, Circle ** circles, int ncircles) {
  int c1;
  if (!readInt (&line, &c1)) {
    fprintf(stderr,"First circle number of intersect command requires an integer!\n");
    return;
  }
  if (c1 < 0 || c1 >= ncircles) {
    fprintf(stderr, "Invalid circle number! %d is out of range [0,%d)\n", c1, ncircles);
    return;
  }
  
  int c2;
  if (!readInt (&line, &c2)) {
    fprintf(stderr,"Second circle number of distance command requires an integer!\n");
    return;
  }
  if (c2 < 0 || c2 >= ncircles) {
    fprintf(stderr, "Invalid circle number! %d is out of range [0,%d)\n", c2, ncircles);
    return;
  }
  double a = circles[c1]->intersectionArea(*circles[c2]);
  printf("Area of intersection of c%d to c%d is %.2fpi\n", c1, c2, a/3.1415926535);

}
void freeCircles(Circle ** circles, int ncircles) {
  for (int i =0; i < ncircles; i++) {
    delete circles[i];
  }
  delete[] circles;
}
Circle ** readCircles(int ncircles) {
  Circle ** ans = new Circle*[ncircles];
  char * line = NULL;
  size_t sz = 0;

  for (int i = 0; i < ncircles; i++) {
    if (getline (&line, &sz, stdin) < 0) {
      fprintf(stderr,"Could not read information for circle %d\n", i);
      freeCircles(ans,i);
      free(line);
      return NULL;
    }
    double x;
    double y;
    double r;
    char * ptr = skipWs(line);
    if (!readDouble(&ptr, &x) || 
	!readDouble(&ptr, &y) ||
	!readDouble(&ptr, &r)) {
      fprintf(stderr,"Invalid description (%s) for circle %d\n", line,i);
      freeCircles(ans,i);
      free(line);
      return NULL;
    }
    Point center;//would be nicer to have a Point(x,y) constructor
    //but we don't learn overloading until the next lecture
    center.move(x,y);
    ans[i] = new Circle(center, r);
  }
  free(line);
  return ans;
}
//input:
//numcircles  (an int)
//(num circles descriptions of the circles):
//x y radius 
//(0 or more commands):
// m cnum dx dy    move circle cnum by (dx,dy)
// i c1 c2         print the area of the intersection of circles c1 and c2
int main(void) { 
  char * line = NULL;
  size_t sz = 0;
  if (getline (&line, &sz, stdin) < 0) {
    fprintf (stderr, "Could not read number of circles!\n");
    free(line);
    return EXIT_FAILURE;
  }
  int ncircles = atoi(line);
  if (ncircles <= 0) {
    fprintf(stderr, "Number of circles must be a number >= 0 (was %s)\n", line);
    free(line);
    return EXIT_FAILURE;
  }
  Circle ** circles = readCircles(ncircles);
  if (circles == NULL) {
    free(line);
    return EXIT_FAILURE;
  }
  while (getline(&line, &sz, stdin) >= 0) {
    switch(tolower(*line)) {
    case 'm':
      doMove (skipWs(line+1), circles, ncircles);
      break;
    case 'i':
      doIntersect (skipWs(line+1), circles, ncircles);
      break;
    default:
      fprintf(stderr, "Unrecognizned command on %s\n", line);
    }
  }
  free(line);
  freeCircles(circles,ncircles);
  return EXIT_SUCCESS;
}

