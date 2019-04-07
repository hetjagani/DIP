// args : <input image> <height> <width>

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<malloc.h>

typedef struct {
	double real;
	double imag;
}COMPLEX;

void display_comp(COMPLEX num); 
void com_mat_mul(int ra, int ca, COMPLEX a[][ca], int rb, int cb, COMPLEX b[][cb], COMPLEX mul[ra][cb]);
COMPLEX com_add(COMPLEX num1, COMPLEX num2);
COMPLEX com_multiply(COMPLEX num1, COMPLEX num2);
double com_mag(COMPLEX num);


void main(int argc, char* argv[]){

  int fp1,fp2;
	int i,j,m,n,npix,nscan;
  char input_image[100], output_image[100];

  strcpy(input_image, argv[1]);
  npix = atoi(argv[3]);
  nscan = atoi(argv[2]);
  sprintf(output_image, "%s_out", input_image);
  
  fp1 = open(input_image, O_RDONLY);
  if(fp1 < 0)
    printf("Error opening image...\n");

  fp2 = creat(output_image, 0667);
  if(fp2 < 0)
    printf("Error creating output image...\n");

  unsigned char image[nscan][npix];
  unsigned char image_out[nscan][npix];
  read(fp1, &image[0][0], nscan*npix*sizeof(unsigned char));

	COMPLEX ans[nscan][npix];

	printf("INPUT:\n");
	for(i=0; i<nscan; i++){
		for(j=0; j<npix; j++){
			printf("%d ", image[i][j]);
		}
		printf("\n");
	}

  printf("Calculating dft...\n");
	for(i=0;i<nscan;i++) {
		for(j=0; j<npix; j++) {
      COMPLEX sum = {0.0, 0.0};
			for(m=0; m<nscan; m++) {
				for(n=0; n<npix; n++) {
          COMPLEX k = {0.0,0.0};
					k.real = image[m][n] * cos(-2 * M_PI * ((1.0 * i * m / nscan) + (1.0 * j * n / npix)));
					k.imag = image[m][n] * sin(-2 * M_PI * ((1.0 * i * m / nscan) + (1.0 * j * n / npix)));
					sum = com_add(sum, k); 
				}
			}
			ans[i][j].real = sum.real;
			ans[i][j].imag = sum.imag;
		}
	}

	printf("OUTPUT:\n");
	for(i=0; i<nscan; i++){
		for(j=0; j<npix; j++){
			display_comp(ans[i][j]);
			printf("\t");
		}
		printf("\n");
	}

  double val;
  int height = nscan;
  int width = npix;
  printf("\n\nFinding absolute values of FT...");
  for(int i=0;i<height;i++){
      for(int j=0;j<width;j++){
          val = com_mag(ans[i][j]);
          if(val<=255){
              image_out[i][j] = (unsigned char)val;
          }
          else {
              image_out[i][j] = 255;
          }
      }
  }

  printf("\n\nFinding Magnitude Spectrum of FT...");
  double temp;
  for(int i=0,k=(width/2);i<(width/2),k<width;i++,k++){
      for(int j=0,l=(width/2);j<(width/2),l<width;j++,l++){
          temp = image_out[i][j];
          image_out[i][j] = image_out[k][l];
          image_out[k][l] = temp;
      }
  }

  for(int i=0,k=(width/2);i<(width/2),k<width;i++,k++){
      for(int j=(width/2),l=0;j<width,l<(width/2);j++,l++){
          temp = image_out[i][j];
          image_out[i][j] = image_out[k][l];
          image_out[k][l] = temp;
      }
  }
  
  //Visulization of magnitude spectrum of FT

  double max,min;
  max=log10(1+image_out[0][0]);
  min=log10(1+image_out[0][0]);
  double temp_2darray[height][width];
  for(int i=0;i<height;i++){
      for(int j=0;j<width;j++){
          temp_2darray[i][j] = log(1+image_out[i][j]);
          if(max < temp_2darray[i][j]){
              max=temp_2darray[i][j];
          }
          if(min > temp_2darray[i][j]){
              min=temp_2darray[i][j];
          }
      }
  }
  unsigned char mag_ft[height][width];
  for(int i=0;i<height;i++){
      for(int j=0;j<width;j++){
          val = round(255*((temp_2darray[i][j]-min)/(max - min)));
          if(val<=255){
              mag_ft[i][j] = (unsigned char)val;
          }
          else
          {
              mag_ft[i][j] = 255;
          }
          
      }
  }
  

  write(fp2,&mag_ft[0][0],width*height*sizeof(unsigned char));
  close(fp1);
  close(fp2);
}

void display_comp(COMPLEX num) {
	printf("(%lf + %lfi)",num.real, num.imag);
}

COMPLEX com_multiply(COMPLEX num1, COMPLEX num2) {
	COMPLEX ans;
	ans.real = (num1.real*num2.real) - (num1.imag*num2.imag);
	ans.imag = (num1.real*num2.imag) + (num2.real*num1.imag);
	return ans;
}

COMPLEX com_add(COMPLEX num1, COMPLEX num2) {
	COMPLEX ans = {0.0, 0.0};
	ans.real = num1.real + num2.real;
	ans.imag = num1.imag + num2.imag;
	return ans;
}

double com_mag(COMPLEX num) {
  return sqrt(pow(num.real, 2) + pow(num.imag, 2));
}

void com_mat_mul(int ra, int ca, COMPLEX a[][ca], int rb, int cb, COMPLEX b[][cb], COMPLEX mul[ra][cb]) {
	int c,d,k;
	COMPLEX sum;
	for (c = 0; c < ra; c++) {
		for (d = 0; d < cb; d++) {
			for (k = 0; k < rb; k++) {
				sum = com_add(sum, com_multiply(a[c][k], b[k][d]));
			}
			mul[c][d] = sum;
			sum.real = 0;
			sum.imag = 0;
		}
	}
}

