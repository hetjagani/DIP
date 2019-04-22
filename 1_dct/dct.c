// args : <image file name> <nscan> <npix>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <malloc.h>
#include <sys/types.h>
#include "fft.h"

int main(int argc, char* argv[])
{ 
  int npix, nscan, i, j;
  char image_fname[100];

  if(argc < 3)
    printf("Invalid arguments...\n");

  strcpy(image_fname,argv[1]);
  nscan = atoi(argv[2]);
  npix = atoi(argv[3]);

  unsigned char image_buff[nscan][npix];
  int fp1;

  fp1 = open(image_fname, O_RDONLY);
  if(fp1 < 0)
    printf("Error opening image...\n");
  
  read(fp1, &image_buff[0][0], nscan*npix*sizeof(unsigned char));
  
  printf("INPUT:\n");
  for(j=0; j<nscan; j++){
    printf("%d ", image_buff[0][j]);
  }
  printf("\n");

  COMPLEX buffer[nscan][nscan];

  for(i=0; i<nscan; i++) {
    for(j=0; j<nscan; j++) {
      buffer[i][j].real = ((double)image_buff[i][j]);
      buffer[i][j].imag = 0;
    }
  }
  
  for(i=0; i<nscan; i++) {
    COMPLEX arr[2*nscan];
    for(int k=0; k<nscan; k++){
      arr[k] = buffer[i][k];
      arr[k+nscan] = buffer[i][nscan-k-1];
    }
    fft(arr, 2*nscan);
    for(int k=0; k<nscan; k++) {
      buffer[i][k] = arr[k];
    }
    
    for(j=0; j<nscan; j++) {
      COMPLEX temp;
      temp.real = cos(-M_PI * j * (1 / (double)(2*nscan)));
      temp.imag = sin(-M_PI * j * (1 / (double)(2*nscan)));
      
      buffer[i][j] = com_mul(temp, buffer[i][j]);
      
      if(j==0){
        buffer[i][j].real *= 0.5*pow(nscan, -0.5);
        buffer[i][j].imag *= 0.5*pow(nscan, -0.5);
      }else{
        buffer[i][j].real *= 0.5*pow(2/(double)nscan, 0.5);
        buffer[i][j].imag *= 0.5*pow(2/(double)nscan, 0.5);
      }
    }
  }
  com_transpose(nscan, buffer);
  
  for(i=0; i<nscan; i++) {
    COMPLEX arr[2*nscan];
    for(int k=0; k<nscan; k++){
      arr[k] = buffer[i][k];
      arr[k+nscan] = buffer[i][nscan-k-1];
    }
    fft(arr, 2*nscan);
    for(int k=0; k<nscan; k++) {
      buffer[i][k] = arr[k];
    }
    for(j=0; j<nscan; j++) {
      COMPLEX temp;
      temp.real = cos((-M_PI * j)*(1 / (double)(2*nscan)));
      temp.imag = sin((-M_PI * j)*(1 / (double)(2*nscan)));
      
      buffer[i][j] = com_mul(temp, buffer[i][j]);
      if(j==0){
        buffer[i][j].real *= 0.5*pow(nscan, -0.5);
        buffer[i][j].imag *= 0.5*pow(nscan, -0.5);
      }else{
        buffer[i][j].real *= 0.5*pow(2/(double)nscan, 0.5);
        buffer[i][j].imag *= 0.5*pow(2/(double)nscan, 0.5);
      }
    }
  }
  com_transpose(nscan, buffer);

  printf("\nANSWER (1st row) : \n");
  for(i=0; i<nscan; i++){
    printf("%lf ", buffer[0][i].real);
  }
  return 0;
}
