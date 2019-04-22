#include <stdio.h>

typedef struct Encoding_data
{
  unsigned char pix_val;
  int code_val;
	unsigned char code_len;
}enc_info;

void store_table();
void read_table(enc_info []);
int code_to_int(char str[]);

void store_table(){
	FILE *fp = fopen("table.txt","w"); 
	enc_info lut={0};
	if (fp == NULL) 
		printf("Can't create file..."); 

	for(int i=0;i<SIZE;i++){
		lut.pix_val = node_list[i].val-1;
		lut.code_len = strlen(node_list[i].code);
		lut.code_val = code_to_int(node_list[i].code);
		fwrite(&lut, sizeof(enc_info), 1, fp);
	}
	fclose(fp);
}

void read_table(enc_info table[]){
	enc_info buffer={0};
        FILE *fp;
	fp = fopen("table.txt", "r");
	int i=0;
	if (fp == NULL) 
		printf("Can't read file..."); 
	
	printf("Pixel Code Value Code len\n");
	while(fread(&buffer, sizeof(enc_info), 1, fp)){       
		table[i] = buffer;
		printf("%u \t %d \t %u\n", table[i].pix_val, table[i].code_val, table[i].code_len);
		++i;
	}
	fclose(fp);
}

int code_to_int(char str[]){
	int length = strlen(str);
	int num = 0;
	for(int i=length-1; i>=0; --i){
		num += ((int)pow(2, i)*(((int)str[length-1-i])-48));
	}
	return num;
}