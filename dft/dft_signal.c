#include<stdlib.h>
#include<stdio.h>
#include "comp.h"

void main(){
  int i,j;
  COMPLEX mat[4][4],ans[4][1],a[4][1];

  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      mat[i][j].real = cos(-2 * M_PI * i * j / 4);
      mat[i][j].imag = sin(-2 * M_PI * i * j / 4);
    }
  }
  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      a[i][j].real = 0;
      a[i][j].imag = 0;
    }
  }
  printf("Enter the sequence : \n");
  for(i=0; i<4; i++){
      scanf("%lf%lf", &a[i][0].real, &a[i][0].imag);
  }

  com_mat_mul(4,4,mat,4,1,a,ans);

  for(i=0;i<4;i++) {
    disp_com(ans[i][0]);
  }

}
