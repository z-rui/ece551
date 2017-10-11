#include "point.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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
//first parse an int (point num) and two doubles (dx, dy)
//then move the appropriate point.
void doMove (char * line, Point * points, int npoints) {
  int pnum;
  if (!readInt (&line, &pnum)) {
    fprintf(stderr,"Point number of move command requires an integer!\n");
    return;
  }
  if (pnum < 0 || pnum >= npoints) {
    fprintf(stderr, "Invalid point number! %d is out of range [0,%d)\n", pnum, npoints);
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
  points[pnum].move(dx,dy);
}
//line points just past any whitespace after the "d" that designates the command
//first parse the two ints that name the points.
//then print the distance between them.
void doDistance (char * line, Point * points, int npoints) {
  int p1;
  if (!readInt (&line, &p1)) {
    fprintf(stderr,"First point number of distance command requires an integer!\n");
    return;
  }
  if (p1 < 0 || p1 >= npoints) {
    fprintf(stderr, "Invalid point number! %d is out of range [0,%d)\n", p1, npoints);
    return;
  }
  
  int p2;
  if (!readInt (&line, &p2)) {
    fprintf(stderr,"Second point number of distance command requires an integer!\n");
    return;
  }
  if (p2 < 0 || p2 >= npoints) {
    fprintf(stderr, "Invalid point number! %d is out of range [0,%d)\n", p2, npoints);
    return;
  }
  double d = points[p1].distanceFrom(points[p2]);
  printf("Distance from p%d to p%d is %.2f\n", p1, p2, d);
}

//input:
//numpoints  (an int)
//0 or more commands:
// m pnum dx dy    move point pnum by (dx,dy)
// d p1 p2         print the distance between points p1 and p2
int main(void) { 
  char * line = NULL;
  size_t sz = 0;
  if (getline (&line, &sz, stdin) < 0) {
    fprintf (stderr, "Could not read number of points!\n");
    free(line);
    return EXIT_FAILURE;
  }
  int npoints = atoi(line);
  if (npoints <= 0) {
    fprintf(stderr, "Number of points must be a number >= 0 (was %s)\n", line);
    free(line);
    return EXIT_FAILURE;
  }
  Point * points = new Point[npoints];
  while (getline(&line, &sz, stdin) >= 0) {
    switch(tolower(*line)) {
    case 'm':
      doMove (skipWs(line+1), points, npoints);
      break;
    case 'd':
      doDistance (skipWs(line+1), points, npoints);
      break;
    default:
      fprintf(stderr, "Unrecognizned command on %s\n", line);
    }
  }
  free(line);
  delete[] points;
  return EXIT_SUCCESS;
}

