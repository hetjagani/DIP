// args : <input image name> <N>

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
	
	for(int i=0; i<N; i++) {
		for(int j=0; j<N; j++) {
			fft_buff[i][j].real = image_buff[i][j];
			fft_buff[i][j].imag = 0;
		}
	}
	
	fft_mat(N, fft_buff, fft_ans);

	COMPLEX (*psf)[N] = malloc(sizeof(COMPLEX[N][N]));
	COMPLEX (*psf_ft)[N] = malloc(sizeof(COMPLEX[N][N]));

	int blur = 5;
	for(int i=0; i<N; i++) {
		for(int j=0; j<N; j++) {
			if(i<blur && j<blur){
				psf[i][j].real = 1/pow(blur,2);
				psf[i][j].imag = 0;
			}else {
				psf[i][j].real = 0;
				psf[i][j].imag = 0;
			}
		}
	}
	
	fft_mat(N, psf, psf_ft);

	COMPLEX (*rec_ft)[N] = malloc(sizeof(COMPLEX[N][N]));
	double K = 0.02;

	for(int i=0; i<N; i++) {
		for(int j=0; j<N; j++) {
			COMPLEX num = com_conj(psf_ft[i][j]);
			COMPLEX den = {pow(com_mag(psf_ft[i][j]), 2) + K, 0};
			rec_ft[i][j] = com_mul(com_div(num, den), fft_ans[i][j]);
		}
	}

	double (*rec_img)[N] = malloc(sizeof(double[N][N]));

	ifft_mat(N, rec_ft, rec_img);

	for(int i=0; i<N; i++) {
		for(int j=0; j<N; j++) {
			image_buff[i][j] = (abs(round(rec_img[i][j])) > 255) ? 255 : (unsigned char)abs(round(rec_img[i][j]));
		}
	}
  
  printf("\nOUTPUT: \n");
	for(int i=0; i<N; i++) {
		printf("%d ", image_buff[0][i]);
	}
  
	write(fp2, &image_buff[0][0], N*N*sizeof(unsigned char));

	close(fp1);
	close(fp2);
	return 0;
}
