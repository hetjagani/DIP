#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <malloc.h>

void main()
{
	int fp1,fp2;
	char input_image[100],output_image[100];
	int npix,nscan,scan;
 
	printf("Give input image name \n");
	scanf("%s",input_image);
	printf("Give size of image \n");
	scanf("%d%d",&npix,&nscan);
	printf("Input Image is %s npix nscan %d %d \n",input_image,npix,nscan);
	sprintf(output_image,"%s_out",input_image);
 
	fp1= open(input_image,O_RDONLY);
	unsigned char image[nscan][npix];

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

	for (scan=0;scan<nscan;scan++) {
		write(fp2, &image[nscan-scan-1], npix*sizeof(unsigned char));
	}
	close(fp1);
	close(fp2);
}
