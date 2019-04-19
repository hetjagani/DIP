#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <malloc.h>

void main()
{
	int fp1,fp2,a;
	char input_image[100];
	unsigned char image[1000];
	int npix,nscan,pix,scan,i1,i2;
	printf("Give input image name \n");
	scanf("%s",input_image);
	printf("Give size of image \n");
	scanf("%d%d",&nscan, &npix);
	printf("Input Image is %s npix nscan %d %d \n",input_image, npix, nscan);
	fp1= open(input_image, O_RDONLY);
	FILE *fp;
	fp = fopen("encoded", "w+");

	if (fp1<0){
		printf("Error in opening %s image \n",input_image);
		exit(1);
	}
		
	int curr_gray_count = -1;
	int cnt = 0;

	for (scan=0;scan<nscan;scan++)
	{
		read(fp1,&image[0],npix*sizeof(unsigned char));
		
		for (pix=0;pix<npix;pix++) {
			a=(int)image[pix];
			
			if (curr_gray_count != a) {
				if (curr_gray_count != -1) {
					printf("%d -> %d\n", (int) curr_gray_count, cnt);
					fputc(curr_gray_count, fp);
					putw(cnt, fp);
				}
				
				curr_gray_count = a;
				cnt = 0;
			}
			cnt = cnt + 1;
		}
	}

	printf("%d -> %d\n", (int) curr_gray_count, cnt);
	fputc(curr_gray_count, fp);
	putw(cnt, fp);
	fputc(curr_gray_count, fp);

	fclose(fp);
	close(fp1);
}
