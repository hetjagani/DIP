// args : <input image name> <nscan> <npix>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <float.h>
#define WINDOW_SIZE 4

void main(int argc, char* argv[]){
        char input_image[100];
        int width = 512, height = 512;

        strcpy(input_image, argv[1]);
        height = atoi(argv[2]);
        width = atoi(argv[3]);
        
        printf("\nInput Image is %s. width : %d height : %d \n",input_image,width,height);


        unsigned char input[height][width];
        int input_image_fp= open(input_image,O_RDONLY);
        if (input_image_fp<0)
        {
                printf("Error in opening %s image \n",input_image);
                exit(1);
        }
        read(input_image_fp, &input[0][0], width*height*sizeof(unsigned char));

        double noise_var = 0.01;
        printf("\nEnter the variance of the noise : ");
        scanf("%lf", &noise_var);

        for (int scan=0; scan<height; scan+=WINDOW_SIZE){
                for (int pix=0; pix<width; pix+=WINDOW_SIZE){
                        int cnt=0;
                        double region[WINDOW_SIZE][WINDOW_SIZE] = {0}, sum = 0;

                        for(int i=0; i<WINDOW_SIZE; ++i){
                                for(int j=0; j<WINDOW_SIZE; ++j){
                                        if((scan+i)<height && (pix+j)<width){
                                                region[i][j] = (double) input[scan+i][pix+j];
                                                sum += region[i][j];
                                                cnt++;
                                        }
                                }
                        }
                        sum /= (double)cnt;

                        double reg_var = 0;
                        for(int i=0; i<WINDOW_SIZE; ++i){
                                for(int j=0; j<WINDOW_SIZE; ++j){
                                        if((scan+i)<height && (pix+j)<width){
                                                reg_var += pow(region[i][j] - sum, 2);
                                        }
                                }
                        }
                        reg_var /= (double)cnt;

                        //printf("%lf %lf \n", sum, reg_var);
                        for(int i=0; i<WINDOW_SIZE; ++i){
                                for(int j=0; j<WINDOW_SIZE; ++j){
                                        input[scan+i][pix+j] = (unsigned char)(region[i][j] - (noise_var/reg_var)*(region[i][j] - sum));
                                        //printf("%u ", input[scan+i][pix+j]);
                                }
                                //printf("\n");
                        }
                }
        }
        char out_img_name[100];
        sprintf(out_img_name,"%s_out",input_image);
        int output_image_fp=creat(out_img_name,0667);
        if (output_image_fp<0){
        printf("Error in creating output %s image\n",out_img_name);
        exit(1);
        }
        write(output_image_fp,&input[0][0],width*height*sizeof(unsigned char));
        close(output_image_fp);
        printf("%s has been generated...", out_img_name);
}