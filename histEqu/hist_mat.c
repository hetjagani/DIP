/* 
args : <input image> <target image>
baby image : 663w X 497h
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>

void hist_eq(int npix, int nscan, unsigned char image[][npix], long int cum_freq[]);

void main(int argc, char* argv[])
{
	int fp1,fp2,fp3;
	char input_image[100] ,target_image[100] ,output_image[100];

	int npix_i,nscan_i, npix_t, nscan_t ,scan ,pix;
	
	
	strcpy(input_image,argv[1]);
	strcpy(target_image, argv[2]);
	printf("Enter npix nscan for input_image : ");
	scanf("%d%d", &npix_i, &nscan_i);
	printf("Enter npix nscan for target_image : ");
	scanf("%d%d", &npix_t, &nscan_t);
	
	sprintf(output_image,"%s_out",input_image);
 
	fp1= open(input_image,O_RDONLY);
	fp2= open(target_image, O_RDONLY);

	unsigned char i_image[nscan_i][npix_i], t_image[nscan_t][npix_t];
	unsigned char outbuff[npix_i];

	if (fp1<0 || fp2<0) {
		printf("Error in opening %s image \n",input_image);
		exit(1);
	}

	fp3=creat(output_image,0667);
	if (fp3<0) {
		printf("Error in creating output %s image\n",output_image);
		exit(1);
	}

	for (scan=0;scan<nscan_i;scan++) {
		read(fp1, &i_image[scan], npix_i*sizeof(unsigned char));
	}

	for (scan=0;scan<nscan_t;scan++) {
		read(fp2, &t_image[scan], npix_t*sizeof(unsigned char));
	}

	// obtain histogram equalised mapping of both the images
	long int map_i[256], map_t[256];
	hist_eq(npix_i, nscan_i, i_image,map_i);
	hist_eq(npix_t, nscan_t, t_image,map_t);

	int i,j,m;
	int match[256];

	for(i=0; i<256; i++) {
		m=0;
		for(j=0;j<256;j++){
			if(map_i[i] > map_t[j])
				continue;
			else {
				m=j;
				break;
			}
		}
		match[i] = m;
	}

	printf("\nInput map \t Target map \t Match map\n");
	for(i=0; i<=255;i++) {
		printf("%d => %ld \t %d => %ld \t %d => %d\n", i, map_i[i], i, map_t[i],i, match[i]);
	}

	for (i=0; i<nscan_i; i++) {
		for(j=0; j<npix_i; j++) {
			outbuff[j] = (unsigned char)match[i_image[i][j]];
		}	
		write(fp3, &outbuff[0], npix_i*(sizeof(unsigned char)));
	}
	
	close(fp1);
	close(fp2);
	close(fp3);
}

void hist_eq(int npix, int nscan, unsigned char image[][npix], long int cum_freq[]) {
	int frequency[256], i, j;

	for (i=0; i<=255; i++) {
		frequency[i] = 0;
		cum_freq[i] = 0;
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
}
