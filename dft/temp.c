#include <stdio.h>
#include <stdlib.h>
#include "comp.h"


void transpose(int n, COMPLEX buff[n][n]){
  int i,j;
  for(i = 0; i < n; i++){
    for(j = i+1; j < n; j++){
      COMPLEX temp = buff[i][j];
      buff[i][j] = buff[j][i];
      buff[j][i] = temp;
    }
  }
}


void main() {
  int nscan = 4;
  int npix = 4;
  int i,j;

  COMPLEX a[4][4];
  int cnt = 1;
  for (i=0; i<nscan; i++) {
    for (j=0; j<npix; j++) {
      a[i][j].real = cnt;
      a[i][j].imag = cnt;
      cnt++;
    }
  }

  printf("INPUT:\n");
  for(i=0; i<nscan; i++){
    for(j=0; j<npix; j++){
      disp_com(a[i][j]);
      printf("\t");
    }
    printf("\n");
  }

  transpose(4,a);

  printf("OUTPUT:\n");
  for(i=0; i<nscan; i++){
    for(j=0; j<npix; j++){
      disp_com(a[i][j]);
      printf("\t");
    }
    printf("\n");
  }
}
