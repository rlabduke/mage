/* Name: I88.h                                                                              */
/* Author: J. Michael Word             DateCreated: 11/16/95 */
/* Purpose: definitions for Illustrator88 format output      */
 
#ifndef I88H
#define I88H 1

#include <stdio.h>

#define I88PORTRAIT       (0)
#define I88LANDSCAPE   (1)

typedef struct {
 FILE *fp;
 int llx, lly, urx, ury;         /* bounding box in output coordinates*/
 int orientation;
  double scaleA, scaleB, scaleC, scaleD, transX, transY;  /* transform matrix */
  double scaleLength;                   /* length scale */
  double txtA, txtB, txtC, txtD;          /* text transform matrix */
 int groupCount;
} I88globals;

extern I88globals I88;

void I88header(FILE *fp, char* cre, char* usr, char* fn, char *dt,
      double llx, double lly, double urx, double ury, int orientation);
void I88transform(double x, double y, double *x2, double *y2);
void I88rect(double x, double y, double w, double h);
void I88circle(double x, double y, double r);
int I88makePSstring(char *s, char **outputstring);
void I88text(char *str, char *fnt, double x, double y, double sz);
void I88indexcolor(int idx);
void I88drawline(double fx, double fy, double tx, double ty);
void I88linewidth(double wid);
void I88beginGroup(void);
void I88endGroup(void);
void I88trailer(void);
#endif
