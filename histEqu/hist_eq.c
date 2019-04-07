/* 
args : <input image> <width> <height>
baby image : 663w X 497h
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>

void main(int argc, char* argv[])
{
	int fp1,fp2;
	char input_image[100],output_image[100];
	int npix,nscan,scan;
	int frequency[256],vals[256], i, j, k,v;
	long int cum_freq[256];
	
	
	strcpy(input_image,argv[1]);
	npix = atoi(argv[2]);
	nscan = atoi(argv[3]);
	sprintf(output_image,"%s_out",input_image);
 
	fp1= open(input_image,O_RDONLY);
	unsigned char image[nscan][npix];
	unsigned char outbuff[npix];

	if (fp1<0)
	{
		printf("Error in opening %s image \n",input_image);
		exit(1);
	}
	fp2=creat(output_image,0667);
	if (fp2<0)
	{
		printf("Error in creating output %s image\n",output_image);
		exit(1);
	}

	for (scan=0;scan<nscan;scan++)
	{
		read(fp1, &image[scan], npix*sizeof(unsigned char));
	}

	for (i=0; i<=255; i++) {
		frequency[i] = 0;
		cum_freq[i] = 0;
		vals[i] = i;
	}
	
	int max = 0;

	for (i=0; i<nscan; i++) {
		for(j=0; j<npix; j++) {
			frequency[image[i][j]]++;
			if(max < image[i][j])
				max = image[i][j];	
		}	
	}

	cum_freq[0] = frequency[0];
	for (i=1; i<=255; i++) {
		cum_freq[i] = frequency[i] + cum_freq[i-1];
	}
	for (i=0; i<=255; i++) {
		cum_freq[i] = cum_freq[i]*max / (npix*nscan);
	}
	printf("max = %d\n", max);
	for(i=0; i<=255;i++) {
		printf("%d => %ld\n", i, cum_freq[i]);
	}

	for (i=0; i<nscan; i++) {
		for(j=0; j<npix; j++) {
			outbuff[j] = (unsigned char)cum_freq[image[i][j]];
		}	
		write(fp2, &outbuff[0], npix*(sizeof(unsigned char)));
	}
	
	close(fp1);
	close(fp2);
}
