// args : <input image> <height> <width>

#include <stdio.h>
#include "fft.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <malloc.h>

void main(int argc, char* argv[]){
  int fp1,fp2;
  int i,j,m,n,npix,nscan;
  char input_image[100], output_image[100];

  strcpy(input_image, argv[1]);
  npix = atoi(argv[3]);
  nscan = atoi(argv[2]);
  // sprintf(output_image, "%s_out", input_image);
  
  fp1 = open(input_image, O_RDONLY);
  if(fp1 < 0)
    printf("Error opening image...\n");

  //fp2 = creat(output_image, 0667);
  //if(fp2 < 0)
  //  printf("Error creating output image...\n");

  unsigned char image[nscan][npix];
  read(fp1, &image[0][0], nscan*npix*sizeof(unsigned char));

  printf("INPUT:\n");
  for(j=0; j<npix; j++){
    printf("%d ", image[0][j]);
  }
  printf("\n");
  
  COMPLEX fft_ans[nscan][npix], buff[nscan][npix];
  for(i=0;i<nscan;i++) {
    for(j=0;j<nscan;j++) {
      buff[i][j].real = image[i][j];
      buff[i][j].imag = 0;
    }
  }
  fft_mat(64, buff, fft_ans);

  double original[nscan][npix];
  ifft_mat(64, fft_ans, original);

  printf("OUTPUT:\n");
  for(j=0; j<npix; j++){
    printf("%lf ",(original[0][j]));
  }
  printf("\n");
}


