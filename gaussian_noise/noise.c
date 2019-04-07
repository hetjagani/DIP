#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>

#define PI 3.141592654

double gaussrand1();
double gaussrand2();

int main(int argc, char *argv[])
{
  int npix, nscan;
  char in_image_fname[100], out_image_fname[100];

  if(argc < 3)
    printf("Invalid arguments...\n");

  strcpy(in_image_fname,argv[1]);
  nscan = atoi(argv[2]);
  npix = atoi(argv[3]);

  unsigned char image_buff[npix];
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
  
	for (int i=0; i<nscan; i++) {
		read(fp1, image_buff, npix*sizeof(unsigned char));
		
		for(int j=0; j<npix; j++) {
			// if(i==0)
			// 	printf("%d ", image_buff[j]);
			image_buff[j] = (unsigned char)round((double)image_buff[j] + 5*gaussrand2());
		}
		write(fp2, image_buff, npix*sizeof(unsigned char));
		bzero(image_buff, npix*sizeof(unsigned char));
	}
	printf("%s file with noise created...\n", out_image_fname);

  return 0;
}
///////////////////////////////////////////////////////////////////////////
double gaussrand1()
{
	static double V1, V2, S;
	static int phase = 0;
	double X;

	if(phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
			} while(S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	} else
		X = V2 * sqrt(-2 * log(S) / S);

	phase = 1 - phase;

	return X;
}

///////////////////////////////////////////////////////////////////////////
double gaussrand2()
{
	static double U, V;
	static int phase = 0;
	double Z;

	if(phase == 0) {
		U = (rand() + 1.) / (RAND_MAX + 2.);
		V = rand() / (RAND_MAX + 1.);
		Z = sqrt(-2 * log(U)) * sin(2 * PI * V);
	} else
		Z = sqrt(-2 * log(U)) * cos(2 * PI * V);

	phase = 1 - phase;

	return Z;
}