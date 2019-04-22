// args : <input file name> <N>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include "fft.h"

int main(int argc, char *argv[])
{
  int N;
  char in_image_fname[100], out_image_fname[100];

  if(argc < 2)
    printf("Invalid arguments...\n");

  strcpy(in_image_fname,argv[1]);
  N = atoi(argv[2]);

  unsigned char (*image_buff)[N] = malloc(sizeof(unsigned char[N][N]));

  int fp1,fp2;

  fp1 = open(in_image_fname, O_RDONLY);
  if(fp1 < 0)
    printf("Error opening image...\n");

  sprintf(out_image_fname,"%s_out",in_image_fname);

  fp2 = creat(out_image_fname,0667);
	if (fp2<0) {
		printf("Error in creating output %s image\n",out_image_fname);
		exit(1);
	}
  
	read(fp1, &image_buff[0][0], N*N*sizeof(unsigned char));
	printf("INPUT : \n");
	for(int i=0; i<N; i++) {
		printf("%d ", image_buff[0][i]);
	}

	COMPLEX (*fft_buff)[N] = malloc(sizeof(COMPLEX[N][N]));
	COMPLEX (*fft_ans)[N] = malloc(sizeof(COMPLEX[N][N]));;

	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++) {
			fft_buff[i][j].real = pow(-1, i+j) * (double)image_buff[i][j];
			fft_buff[i][j].imag = 0;
		}
	}
	
	fft_mat(N, fft_buff, fft_ans);
	free(fft_buff);

	int R;
	printf("\nEnter Filter Radius : ");
	scanf("%d", &R);
	
	char ftype[2];
	printf("\nEnter Filter Type (hp/lp/bp) : ");
	scanf("%s", ftype);
	
	if(strcmp(ftype, "lp") == 0) {
    printf("LOW PASS...\n");
	  for(int i=0; i<N; i++) {
		  for(int j=0; j<N; j++) {
			  if(pow(i-(N/2),2)+pow(j-(N/2),2) > R*R){
				  fft_ans[i][j].real = 0;
				  fft_ans[i][j].imag = 0;
			  }else {
				  continue;
			  }
		  }
	  }
	} else if(strcmp(ftype, "hp") == 0) {
    printf("HIGH PASS...\n");
	  for(int i=0; i<N; i++) {
		  for(int j=0; j<N; j++) {
			  if(pow(i-(N/2),2)+pow(j-(N/2),2) < R*R){
				  fft_ans[i][j].real = 0;
				  fft_ans[i][j].imag = 0;
			  }else {
				  continue;
			  }
		  }
	  }
	} else if(strcmp(ftype, "bp") == 0) {
    printf("BAND PASS...\n");
    int R1,R2;
    printf("\nEnter Band Radius (LC HC): ");
    scanf("%d%d", &R1, &R2);
	  for(int i=0; i<N; i++) {
		  for(int j=0; j<N; j++) {
			  if(pow(i-(N/2),2)+pow(j-(N/2),2) < R1*R1 || pow(i-(N/2),2)+pow(j-(N/2),2) > R2*R2){
				  fft_ans[i][j].real = 0;
				  fft_ans[i][j].imag = 0;
			  }else {
				  continue;
			  }
		  }
	  }
	} else{
	  printf("\nInvalid filter type..");
	}
	
	double (*filtered)[N] = malloc(sizeof(double[N][N]));
	
	ifft_mat(N, fft_ans, filtered);
	
  unsigned char (*out_buffer)[N] = malloc(sizeof(unsigned char[N][N]));
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			out_buffer[i][j] = (unsigned char)abs(filtered[i][j]);	
		}
	}
	printf("\nOUTPUT : \n");
	for(int i=0; i<N; i++) {
		printf("%d ", out_buffer[0][i]);
	}
	write(fp2, &out_buffer[0][0], N*N*sizeof(unsigned char));
	printf("writing out image %s... \n", out_image_fname);
  return 0;
}
