#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <malloc.h>
#include <unistd.h>


typedef struct  complex_num{
    double real;
    double imag;
}comp;
comp add_comp_nums(comp num1, comp num2);
comp mult_comp_nums(comp num1, comp num2);

void display_comp(comp num) {
	printf("(%lf + %lfi)",num.real, num.imag);
}

void main()
{
      
    int i,j,input_image_fp,width,height,output_image_fp;
    char inp_img_name[100], out_img_name[100];
    
    setbuf(stdout, NULL);
    printf("Give input image name : ");
    scanf("%s",inp_img_name);
    printf("\nGive width and height of image : ");
    scanf("%d%d",&width,&height);
    printf("\nInput Image is %s width : %d height : %d \n",inp_img_name,width,height);
    
    unsigned char input_image[height][width]; //,output_image[height][width];
    double output_image[height][width];
    
    printf("\n\nReading input...");
    int cnt=0;
    /*unsigned char input_image[height][width],output_image[height][width];
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            input_image[i][j] = cnt+1;
            cnt++;
        }
    }
    printf("\n\n");
    */
    
    
    input_image_fp= open(inp_img_name,O_RDONLY);
    if (input_image_fp<0)
    {
        printf("Error in opening %s image \n",inp_img_name);
        exit(1);
    }
    read(input_image_fp,&input_image[0][0],width*height*sizeof(unsigned char));
    close(input_image_fp);
    

    printf("\n\nCreating output file...");
    sprintf(out_img_name,"%s_out",inp_img_name);
    //printf("\nOutput Image is %s width : %d height : %d \n\n",out_img_name,width,height);
    output_image_fp=creat(out_img_name,0667);
    if (output_image_fp<0)
    {
        printf("Error in creating output %s image\n",out_img_name);
        exit(1);
    }
    
    printf("\n\nDoing DFT...");
    comp ft[height][width];
    for(int k=0;k<height;k++){
        for(int l=0;l<width;l++){
            comp val = {0.0,0.0};
            for(int i=0;i<height;i++){
                for(int j=0;j<width;j++){
                    comp temp = {0,0};
                    double s1 = ((1.0*k*i)/(double)width);
                    double s2 = ((1.0*l*j)/(double)width);
                    temp.real = input_image[i][j]*cos(2*M_PI*(s1+s2));
                    temp.imag = input_image[i][j]*-sin(2*M_PI*(s1+s2));
                    val.real += temp.real;
                    val.imag += temp.imag;              
                }
            }
            ft[k][l] = val;
        }
    }
    printf("OUTPUT:\n");
      for(i=0; i<height; i++){
        for(j=0; j<width; j++){
          display_comp(ft[i][j]);
          printf("\t");
        }
        printf("\n");
      }

    double val;
    printf("\n\nFinding absolute values of FT...");
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            val = (sqrt(pow(ft[i][j].real, 2)+pow(ft[i][j].imag, 2)));
            if(val<=255){
                output_image[i][j] = (unsigned char)val;
            }
            else
            {
                output_image[i][j] = 255;
            }
            output_image[i][j] = val;
            
        }
    }

    printf("\n\nFinding Magnitude Spectrum of FT...");
    double temp;
    for(int i=0,k=(width/2);i<(width/2),k<width;i++,k++){
        for(int j=0,l=(width/2);j<(width/2),l<width;j++,l++){
            temp = output_image[i][j];
            output_image[i][j] = output_image[k][l];
            output_image[k][l] = temp;
        }
    }

    for(int i=0,k=(width/2);i<(width/2),k<width;i++,k++){
        for(int j=(width/2),l=0;j<width,l<(width/2);j++,l++){
            temp = output_image[i][j];
            output_image[i][j] = output_image[k][l];
            output_image[k][l] = temp;
        }
    }
    
    //Visulization of magnitude spectrum of FT

    double max,min;
    max=log10(1+output_image[0][0]);
    min=log10(1+output_image[0][0]);
    double temp_2darray[height][width];
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            temp_2darray[i][j] = log(1+output_image[i][j]);
            if(max < temp_2darray[i][j]){
                max=temp_2darray[i][j];
            }
            if(min > temp_2darray[i][j]){
                min=temp_2darray[i][j];
            }
        }
    }
    unsigned char mag_ft[height][width];
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            val = round(255*((temp_2darray[i][j]-min)/(max - min)));
            if(val<=255){
                mag_ft[i][j] = (unsigned char)val;
            }
            else
            {
                mag_ft[i][j] = 255;
            }
            
        }
    }
    

    write(output_image_fp,&mag_ft[0][0],width*height*sizeof(unsigned char));
    //write(output_image_fp,&output_image[0][0],width*height*sizeof(unsigned char)); where output_image is unsigned char
    close(output_image_fp);
    printf("\n\nCompleted...");
}

comp add_comp_nums(comp num1, comp num2){
    comp result={0,0};
    result.real = num1.real + num2.real;
    result.imag = num1.imag + num2.imag;
    return result;
}

comp mult_comp_nums(comp num1, comp num2){
    comp result={0,0};
    result.real = (num1.real * num2.real) - (num1.imag * num2.imag);
    result.imag = (num1.real * num2.imag) + (num1.imag * num2.real);
    return result;
}
