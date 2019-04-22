#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <float.h>
#include <sys/resource.h>
#include "codegen.h"
#include "manage_table.h"

enc_info lut[SIZE] = {0};

int main(int argc, char* argv[]) {
	node_list = (node *)calloc(2*SIZE -1, sizeof(node));
	
	int input_image_fp,a;
	char input_image[100] = "cam2_128_128.raw";
	int width = 128,height = 128,pix,scan;
	
	
	printf("Give input image name : ");
	scanf("%s",input_image);
	printf("\nGive width and height of image : ");
	scanf("%d%d",&width,&height);
	printf("\nInput Image is %s. width : %d height : %d \n",input_image,width,height);
	
	unsigned char read_buffer[width], input[height][width];


	input_image_fp= open(input_image,O_RDONLY);
	if (input_image_fp<0)	{
		printf("Error in opening %s image \n",input_image);
		exit(1);
	}
	for (scan=0;scan<height;scan++)	{
		read(input_image_fp,&read_buffer[0],width*sizeof(unsigned char));
		for (pix=0;pix<width;pix++)
		{
			input[scan][pix] = read_buffer[pix];
			node_list[read_buffer[pix]].freq++;
		}
	}
	
	for(int i=0; i<2*SIZE-1; ++i){
		node_list[i].val = (i+1);
		strcpy(node_list[i].code, "");
		if(i<SIZE){
			node_list[i].freq /= (double)(width*height);
			//printf("%hu %lf\n", node_list[i].val, node_list[i].freq);
		}
	}

	gen_code();

	unsigned char pixel_to_idx[SIZE];
	for(int i=0; i<SIZE; ++i){
		pixel_to_idx[i] = (unsigned char)find_id(i+1);
	}
	store_table();
	
	

	FILE *fp = fopen("encodings.txt","w"); 
	
	if (fp == NULL) 
		printf("Can't create file..."); 

	unsigned char cnt=7;
	unsigned char temp=0;
	for(int x=0; x<height; ++x){
		for(int y=0; y<width; ++y){
			int i = pixel_to_idx[input[x][y]];
			for(int j=0; node_list[i].code[j]!='\0'; ++j){
				int s=(int)node_list[i].code[j];
				s=s-48;
				temp += (s*pow(2, cnt));
				if(cnt==0){
					cnt=7;
					fwrite(&temp, 1, 1, fp);
					temp = 0;
					continue;
				}
				cnt--;
			}
		}
	}
	
	fwrite(&temp, 1, 1, fp);
	fclose(fp);
	close(input_image_fp);
	
	return 0;
}