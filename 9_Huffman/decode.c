#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <float.h>
#include "codegen.h"
#include "manage_table.h"

void write_encodings();
void read_encodings(enc_info []);
int decode_symbols(char []);

enc_info lut[SIZE] = {0};

int main(){
	read_table(lut);
	read_encodings(lut);
	
	return 0;
}

void read_encodings(enc_info lut[]){
	char output_image[100] = "decoded.raw";
	int width = 512,height = 512;
	
	printf("\nGive width and height of image : ");
	scanf("%d%d",&width,&height);
	printf("\nOutput decoded image is %s. width : %d height : %d \n",output_image,width,height);

	unsigned char output[height][width];
	int pixel_count = 0, row_loc=0;
	int output_image_fp=creat(output_image,0667);
  if (output_image_fp<0) {
    printf("Error in creating output %s image\n",output_image);
    exit(1);
  }

	FILE *fp;
	fp = fopen("encodings.txt", "r");
	if (fp == NULL) 
		printf("Can't read file..."); 
	
	long lSize;
	fseek( fp , 0L , SEEK_END);
	lSize = ftell(fp);
	rewind(fp);
	char *buffer;
	buffer = (char *)calloc(1, lSize);
	if(1!=fread( buffer , lSize, 1 , fp) ){
		printf("Can't read encodings...");
		free(buffer);
		fclose(fp);
		exit(1);
	}
	fclose(fp);
	printf("\n");
	char *code_buffer = (char *)calloc(1, lSize*8);
	for(int i=0; i<lSize; ++i){
		unsigned char temp = (unsigned char)buffer[i];
		for(int j=0;j<8; ++j){
			unsigned char bit;
			bit = (temp&(0x80>>j))/(int)pow(2, 7-j);
			bit += 48;
			code_buffer[i*8+j] = bit;
		}
	}
	free(buffer);
	int j=0, i, decoded_val,flag=0;
	char symbols[20];
	printf("\n");
	while(j<8*lSize){
		for(i=0; i<5; ++i){
			symbols[i] = code_buffer[j];
			++j;
			if(j>=8*lSize){
				flag=1;
				break;
			}
		}
		if(flag==1){
			break;
		}
		decoded_val = decode_symbols(symbols);
		while(i<20 && (!decoded_val)){
			symbols[i] = code_buffer[j];
			decoded_val = decode_symbols(symbols);
			++i;
			++j;
			if(j>=8*lSize){
				flag=1;
				break;
			}
		}
		if(flag==1){
			break;
		}

		output[row_loc][pixel_count%width] = (unsigned char)decoded_val;
		if((pixel_count+1)%width==0){
			row_loc++;
		}
		pixel_count++;
		memset(symbols, 0, sizeof(symbols));
	}
	free(code_buffer);
	write(output_image_fp,&output[0][0],width*height*sizeof(unsigned char));
        close(output_image_fp);
}

int decode_symbols(char symbols[]){
	int val = code_to_int(symbols);
	unsigned char length = strlen(symbols);

	for(int i=0; i<SIZE; ++i){
		if(lut[i].code_val == val && lut[i].code_len == length){
			return ((int)lut[i].pix_val + (1));
		}
	}
	return 0;
}