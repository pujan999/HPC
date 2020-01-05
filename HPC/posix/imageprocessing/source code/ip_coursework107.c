#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <malloc.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include "time_diff.h"
/******************************************************************************
  The variable names and the function names of this program is same as provided by the university.
The added variable and function are the only changes made to this program.
  
  To compile adapt the code below wo match your filenames:  
    cc -o ip_coursework107 ip_coursework107.c time_diff.c -lglut -lGL -lm -pthread
   
******************************************************************************/
#define width 100 
#define height 72
typedef struct arguments_t {
  int start;
  int stride;
}
 arguments_t;
unsigned char image[], results[width * height];



void detect_edges(unsigned char *in, unsigned char *out, arguments_t *args) {
  int i;
  int n_pixels = width * height;

  for(i=args->start;i<n_pixels;i+=args->stride){
    int p, s; // the pixel of interest
    int b, d, f, h; // the pixels adjacent to x,y used for the calculation
    int r; // the result of calculate
    
    s = i / width;
    p = i - (width * s);

    if (p == 0 || s == 0 || p == width - 1 || s == height - 1) {
      results[i] = 0;
    } else {
      b = i + width;
      d = i - 1;
      f = i + 1;
      h = i - width;

      r = (in[i] * 4) + (in[b] * -1) + (in[d] * -1) + (in[f] * -1)
          + (in[h] * -1);

      if (r > 0) { // if the result is positive this is an edge pixel
        out[i] = 255;
      } else {
        out[i] = 0;
      }
    }
  }
}

void *detector(void *args){
	detect_edges(image,results,args);
}

void tidy_and_exit() {
  exit(0);
}

void sigint_callback(int signal_number){
  printf("\nInterrupt from keyboard\n");
  tidy_and_exit();
}

static void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glRasterPos4i(-1, -1, 0, 1);
  glDrawPixels(width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, image);
  glRasterPos4i(0, -1, 0, 1);
  glDrawPixels(width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, results);
  glFlush();
}

static void key_pressed(unsigned char key, int x, int y) {
  switch(key){
    case 27: // escape
      tidy_and_exit();
      break;
    default: 
      printf("\nPress escape to exit\n");
      break;
  }
}


int main(int argc, char **argv) {
 
  struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
  signal(SIGINT, sigint_callback);
 
  printf("image dimensions %dx%d\n", width, height);
  
  pthread_t t1, t2, t3, t4;

  arguments_t t1_arguments;
  t1_arguments.start = 0;
  t1_arguments.stride = 4;

  arguments_t t2_arguments;
  t2_arguments.start = 1;
  t2_arguments.stride = 4;

  arguments_t t3_arguments;
  t3_arguments.start = 2;
  t3_arguments.stride = 4;

  arguments_t t4_arguments;
  t4_arguments.start = 3;
  t4_arguments.stride = 4;
  
  pthread_create(&t1, NULL, detector, &t1_arguments);
  pthread_create(&t2, NULL, detector, &t2_arguments);
  pthread_create(&t3, NULL, detector, &t3_arguments);
  pthread_create(&t4, NULL, detector, &t4_arguments);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  pthread_join(t4, NULL);


 clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9));
  glutInit(&argc, argv);
  glutInitWindowSize(width * 2,height);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_LUMINANCE);
      
  glutCreateWindow("6CS005 Image Progessing Courework");
  glutDisplayFunc(display);
  glutKeyboardFunc(key_pressed);
  glClearColor(0.0, 1.0, 0.0, 1.0); 

  glutMainLoop(); 

 
  tidy_and_exit();
 
  return 0;

}unsigned char image[] = {255,255,255,255,0,0,255,255,255,255,255,255,255,0,255,255,0,0,
  255,0,0,255,255,255,255,0,0,0,0,255,255,255,255,255,0,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,0,0,0,0,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,0,255,255,255,255,255,0,0,255,255,255,255,255,255,255,
  0,0,255,0,0,255,255,0,255,255,255,255,255,0,0,0,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,0,255,255,0,0,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,0,0,255,255,255,255,255,0,255,255,
  255,255,0,0,255,255,0,0,0,0,255,255,0,0,255,255,255,255,255,
  0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,255,
  255,255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,255,255,
  255,255,0,0,0,0,0,0,0,255,255,0,0,0,0,255,255,255,0,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,255,
  255,255,0,0,255,255,255,0,0,255,255,0,0,0,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,0,0,255,255,255,255,255,0,0,0,255,255,255,255,255,255,0,0,
  0,255,255,255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,
  255,255,0,0,255,255,255,0,0,255,255,255,255,0,0,0,0,0,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,0,0,0,255,255,255,255,0,0,255,255,255,255,
  255,255,255,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,0,0,255,0,0,255,255,255,0,0,255,255,255,255,0,
  0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,0,255,0,255,255,255,255,255,
  0,255,255,255,255,0,0,255,255,0,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,0,0,255,255,255,255,0,0,0,255,255,0,0,0,0,0,0,
  0,0,255,255,255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,
  0,255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,255,255,0,
  0,0,0,255,255,255,0,0,255,255,0,0,255,255,0,0,0,0,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,0,0,255,255,255,255,0,0,0,0,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,0,255,255,255,255,255,255,255,255,255,255,0,0,0,255,255,
  0,0,255,255,255,0,0,255,255,255,255,0,0,255,255,0,0,0,0,
  0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,
  255,0,0,255,255,0,0,255,255,255,0,0,255,255,255,255,0,0,255,
  255,255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,0,0,255,255,255,0,0,255,255,
  255,255,255,255,255,255,255,0,0,255,0,0,255,255,255,255,0,0,255,
  0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,0,0,0,255,255,255,0,0,255,255,255,
  255,255,0,255,255,255,255,255,255,255,255,255,255,0,0,0,0,255,255,
  255,255,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,255,
  255,0,0,255,255,255,255,0,0,255,255,255,255,255,255,255,255,255,0,
  0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,255,
  255,255,0,0,0,255,0,0,255,255,255,255,255,0,255,255,255,255,255,
  255,255,255,255,255,0,0,0,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,255,255,
  255,0,0,255,255,255,255,255,0,0,255,0,0,255,255,255,255,255,0,
  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,0,0,0,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,0,255,255,255,255,0,0,255,255,255,255,255,255,0,0,255,0,0,
  255,255,255,255,255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,0,0,255,255,0,0,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,0,0,255,255,255,0,0,255,255,255,255,255,255,
  0,0,255,255,0,255,255,255,255,255,0,0,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,255,255,255,0,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,0,255,255,255,255,0,0,
  255,255,255,255,255,255,0,255,255,0,0,255,255,255,255,255,0,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,0,255,0,0,255,255,255,255,0,
  0,255,255,255,0,0,255,0,0,0,0,255,255,255,255,255,255,255,255,
  255,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,0,0,
  255,255,255,0,0,255,255,255,255,255,0,0,255,255,0,0,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,255,255,0,
  0,255,255,255,0,0,255,255,255,0,0,0,0,0,255,255,255,255,255,
  255,255,255,255,255,0,0,0,255,255,0,0,255,255,255,255,255,255,255,
  255,255,255,0,0,0,0,255,255,0,0,255,255,255,255,0,0,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,255,
  255,255,255,255,0,0,255,255,0,0,0,255,255,255,255,0,0,255,255,
  255,255,255,255,255,255,0,255,255,255,0,0,255,255,255,255,0,0,255,
  255,255,255,255,255,255,255,255,0,0,0,0,0,255,255,0,0,0,0,
  0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,255,
  255,0,0,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,255,
  255,0,0,255,255,255,255,255,255,255,255,0,0,0,255,0,0,255,255,
  255,255,0,0,255,255,255,255,255,255,255,255,255,0,255,255,255,0,0,
  255,255,255,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,0,0,255,255,0,0,255,255,255,255,255,255,255,0,0,0,255,
  255,255,255,0,0,255,255,0,0,255,0,0,0,0,255,255,255,0,0,
  255,255,255,255,255,0,0,0,255,255,255,255,255,255,255,255,255,255,0,
  255,255,255,255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,0,255,255,255,255,255,0,0,255,255,0,0,255,0,0,0,0,
  255,255,0,0,255,255,255,255,0,0,255,255,0,0,0,0,0,255,255,
  255,255,255,255,0,0,255,255,255,0,0,0,255,255,255,255,255,255,255,
  255,255,255,255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,0,0,255,255,255,255,0,0,255,255,0,0,
  0,0,0,255,255,255,255,0,0,255,255,255,255,0,0,255,255,255,255,
  255,255,255,255,255,255,255,255,0,0,255,255,255,0,0,0,255,255,255,
  255,255,255,255,255,255,255,255,255,0,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,0,0,0,255,255,255,255,255,255,255,0,0,255,255,255,255,0,
  0,0,255,255,0,0,255,255,255,255,255,255,255,0,0,0,0,0,0,
  255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,255,255,255,0,
  0,255,255,255,255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,0,
  0,0,0,0,0,0,0,255,255,0,0,255,255,255,255,255,255,255,0,
  0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,
  255,255,255,255,255,0,0,255,255,0,0,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,0,0,255,255,255,255,0,0,255,255,
  255,255,255,255,0,0,0,0,0,255,255,0,0,255,255,0,0,255,0,
  0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,0,255,255,255,0,0,0,0,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,0,0,255,255,255,0,0,255,255,255,
  255,0,0,255,255,255,255,255,255,255,0,0,255,255,255,255,0,0,255,
  255,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,0,0,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,255,255,
  0,0,255,255,255,0,0,0,255,255,255,255,255,255,255,0,0,255,255,
  255,255,255,0,0,255,255,0,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,0,255,255,0,0,255,255,255,
  0,0,0,255,255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,
  0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,255,0,0,255,
  255,0,255,255,255,0,0,0,0,255,0,0,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,0,0,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,
  255,255,0,0,255,255,0,0,255,255,0,0,255,0,255,255,0,0,255,
  255,255,255,255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,
  255,255,255,0,0,255,255,255,0,0,255,255,0,255,255,0,255,255,0,
  0,255,0,0,255,255,255,255,255,0,0,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  0,255,255,0,0,255,255,255,0,0,255,255,255,0,0,255,255,0,0,
  0,0,255,255,255,0,255,255,0,0,0,0,0,0,0,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,0,0,255,255,0,0,255,255,255,0,0,255,255,255,255,
  0,0,255,0,0,0,0,255,255,255,0,0,255,255,255,0,0,0,0,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,0,255,255,255,255,0,0,255,255,255,0,0,0,255,0,
  0,255,255,255,255,0,0,255,255,0,0,0,255,255,255,255,0,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,0,255,255,0,0,255,255,255,0,0,255,255,255,
  0,0,0,0,0,0,255,255,255,255,255,0,0,255,0,0,0,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,0,255,255,0,0,255,255,255,
  0,0,255,255,255,255,0,0,0,0,0,0,255,255,255,255,0,0,255,
  255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,
  0,255,255,255,255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,0,0,255,255,255,255,0,0,0,255,
  255,0,0,0,0,0,0,0,0,255,255,0,0,255,255,0,0,0,0,
  255,255,255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,0,0,255,255,0,0,255,255,255,0,0,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,
  0,0,0,0,255,255,0,0,0,0,255,255,0,0,0,255,255,0,0,
  255,255,255,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,0,255,255,255,0,0,0,255,255,0,0,0,255,255,0,0,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,0,0,255,255,255,255,255,
  255,0,0,0,0,0,255,0,0,255,255,255,0,0,255,255,255,255,0,
  0,255,255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,0,0,255,0,0,255,255,255,0,0,0,0,255,255,0,0,0,
  0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,
  0,255,255,255,255,255,255,0,0,0,255,255,0,0,255,255,255,0,0,
  255,255,255,255,0,0,255,255,255,0,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,0,255,255,0,0,255,255,0,0,255,255,0,0,0,0,
  255,255,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,0,0,255,255,255,255,255,255,255,0,0,255,255,0,0,
  255,255,255,255,0,0,255,0,0,0,0,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,0,0,255,255,0,0,255,0,0,255,
  255,0,0,255,0,0,255,255,0,0,255,0,0,0,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,255,255,
  0,0,255,0,0,255,255,255,255,0,0,0,0,0,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,0,0,255,255,255,255,255,0,255,255,0,
  0,255,255,0,0,255,0,0,255,255,0,255,255,0,0,255,255,255,0,
  0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,255,
  255,255,0,0,255,255,0,0,0,0,255,255,255,255,255,0,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,255,255,255,255,
  255,0,0,255,255,0,0,255,255,0,0,0,0,255,255,0,0,255,255,
  0,0,255,255,255,255,0,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,0,0,255,255,255,255,0,255,255,255,0,0,0,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,255,255,255,255,
  0,0,255,255,255,255,0,0,255,255,0,0,255,255,0,0,0,0,255,
  255,255,0,255,255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,0,0,255,255,255,0,0,0,255,255,255,0,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,
  0,255,255,255,255,255,0,0,255,0,0,0,0,0,255,255,0,0,255,
  0,0,0,0,255,255,255,0,0,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,255,0,0,0,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,0,0,255,255,255,255,0,0,0,0,0,255,0,0,
  255,255,0,0,255,255,0,0,0,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,
  0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,
  0,255,255,255,255,255,255,255,255,255,0,0,255,255,255,255,255,0,0,
  255,255,255,255,0,0,255,255,0,0,255,0,0,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,0,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,0,0,0,255,255,255,255,255,255,255,255,255,255,0,0,255,
  255,255,255,0,0,255,255,255,255,0,0,255,255,0,0,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,
  0,0,255,0,0,255,255,255,0,0,0,0,255,255,255,255,255,255,255,
  255,255,0,0,255,255,255,255,0,0,255,255,255,255,255,0,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,0,0,0,0,0,255,255,255,0,0,255,255,0,0,0,0,255,255,
  255,255,255,255,255,255,255,255,0,0,255,255,255,255,0,0,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,0,0,0,255,255,255,255,255,255,0,0,255,255,0,
  0,255,0,0,255,255,255,255,255,255,255,255,255,0,0,0,0,0,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,0,255,255,255,255,255,0,0,255,255,255,255,255,255,
  255,0,0,255,0,0,255,255,0,255,255,255,255,255,255,255,255,0,0,
  0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,0,0,0,255,255,255,255,0,0,255,
  255,255,255,255,255,255,0,0,0,0,0,255,255,0,0,255,255,255,255,
  255,255,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,0,0,255,255,255,255,0,0,0,0,255,
  255,255,255,0,0,0,0,0,0,0,255,255,0,0,0,0,255,255,0,
  0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,255,255,255,255,
  0,0,0,0,0,255,255,255,0,0,0,0,0,255,255,255,255,0,0,
  0,255,255,255,255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,
  0,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,255,255,
  255,255,255,255,255,0,0,255,255,0,0,255,255,255,0,0,255,255,255,
  255,255,255,255,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,
  0,0,255,255,255,255,255,255,255,255,255,0,255,255,255,0,0,255,255,
  0,0,255,255,255,255,0,0,255,0,0,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,0,255,255,255,255,255,255,255,255,255,0,0,255,
  255,255,0,0,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,
  0,255,255,255,255,255,255,255,255,255,255,0,0,255,0,0,0,0,255,
  255,255,0,0,255,255,255,0,0,0,255,0,0,0,0,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,0,
  0,0,255,255,255,0,0,255,255,255,255,255,255,255,255,255,0,0,0,
  0,0,255,255,255,255,255,0,0,255,255,255,255,0,0,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  255,255,255,255,0,0,0,255,255,255,255,0,255,255,255,255,255,255,255,
  255,255,255,0,0,255,255,255,255,255,255,255,0,0,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  255,255,0,0,0,255,255,255,255,255,0,255,255,255,255,255,0,0,255,
  255,255,255,255,255,255,255,255,0,0,255,255,255,255,255,0,255,0,0,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,255,255,255,0,0,0,255,255,255,255,255,0,255,255,255,
  255,255,255,0,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,
  0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,255,255,255,0,0,255,255,255,0,0,0,255,255,255,255,
  255,0,255,255,255,255,255,255,0,0,255,255,255,255,255,255,255,255,255,
  0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,255,255,255,0,255,255,255,255,0,255,255,255,255,0,
  0,255,255,255,255,255,255,0,255,255,255,255,255,255,0,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,255,255,255,255,0,255,255,255,255,0,
  255,255,255,255,0,255,255,255,255,255,255,255,0,255,255,255,255,255,255,
  0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,255,255,0,0,0,255,255,255,255,0,0,
  255,255,255,255,0,0,255,255,255,0,255,255,255,255,255,255,255,0,255,
  0,255,255,255,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,0,0,0,255,
  255,255,255,0,0,0,255,255,255,0,0,255,255,255,255,255,255,255,255,
  255,255,255,0,255,0,0,255,0,0,0,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,0,0,255,255,
  255,255,0,0,255,255,255,255,0,0,0,255,255,255,255,0,255,255,255,
  255,255,255,255,0,255,255,255,255,255,0,0,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,
  255,0,0,255,255,255,255,0,0,255,255,255,255,0,0,0,255,255,255,
  255,0,0,255,255,255,255,255,255,0,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,
  0,0,255,255,255,255,0,0,255,255,255,255,255,255,255,255,255,255,0,
  0,0,255,255,255,255,0,0,255,255,255,255,255,255,0,0,255,255,255
};