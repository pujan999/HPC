#include <stdio.h>
#include <math.h>

/******************************************************************************
 * This program takes an initial estimate of m and c and finds the associated 
 * rms error. It is then as a base to generate and evaluate 8 new estimates, 
 * which are steps in different directions in m-c space. The best estimate is 
 * then used as the base for another iteration of "generate and evaluate". This 
 * continues until none of the new estimates are better than the base. This is
 * a gradient search for a minimum in mc-space.
 * 
 * To compile:
 *   cc -o lr_courseworkb_107 lr_courseworkb_107.c
 * 
 * To run:
 *   ./lr_courseworkb_107
 * 
 * Dr Kevan Buckley, University of Wolverhampton, 2018
 *****************************************************************************/


int main(int argc, char **argv) {
  int i;
  double m;
  double c;
  double p;
  double s;
  
  if(argc != 3) {
    fprintf(stderr, "You need to specify a slope and intercept\n");
    return 1;
  }

  sscanf(argv[1], "%lf", &m);
  sscanf(argv[2], "%lf", &c);
  printf("p,s\n");
  for(i=0; i<100; i++) {
    p = i;
    s = (m * p) + c;
    printf("%0.2lf,%0.2lf\n", p, s);
  }
  
  return 0;
}

