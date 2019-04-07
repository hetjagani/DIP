#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <malloc.h>

void main(int argc, char* argv[])
{
	int fp1,fp2;
	char input_image[100],output_image[100];
	int npix=512,nscan=512,scan;
	int frequency[256],vals[256], i, j, k,v;
	FILE* freq;
 
	strcpy(input_image,argv[1]);
	sprintf(output_image,"%s_out",input_image);
 
	fp1= open(input_image,O_RDONLY);
	unsigned char image[nscan][npix];

	freq = fopen("freq.txt", "w");
	if(freq == NULL)
		printf("error opening frequency file... \n");

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
		vals[i] = i;
	}
	
	for (i=0; i<nscan; i++) {
		for(j=0; j<npix; j++) {
			frequency[image[i][j]]++;	
		}	
	}

	for (i=0; i<=255; i++) {
		fprintf(freq, "%d %d\n", i, frequency[i]);
	}

	int n = 256;
	for (j = 0 ; j < n - 1; j++)
  	{
	    for (k = 0 ; k < n - j - 1; k++)
	    {
			if (frequency[k] < frequency[k+1]) 
			{
				v = vals[k];
				i = frequency[k];

				frequency[k] = frequency[k+1];
				vals[k] = vals[k+1];

				frequency[k+1] = i;
				vals[k+1] = v;
			}
	    }
  	}
	
	for (i=0; i<=255; i++) {
		printf("%d %d \n", vals[i], frequency[i]);
	}
	close(fp1);
	close(fp2);
	close(freq);
}
