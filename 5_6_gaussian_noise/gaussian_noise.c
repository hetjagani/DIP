// args : <input file name> <nscan> <npix>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdbool.h>

void main(int argc, char* argv[])
{
  int fp1,fp2;
  char input_image[100],output_image[100];
  unsigned char image[1000],image1[1000];
  int npix,nscan,pix,scan,i1,i2;
  int gaussian_freq[100], i;
  float x = 1, sum = 1;
  
  strcpy(input_image,argv[1]);
  nscan = atoi(argv[2]);
  npix = atoi(argv[3]);
  
  printf("Input Image is %s npix nscan %d %d \n",input_image,npix,nscan);
  sprintf(output_image,"%s_out",input_image);
  fp1= open(input_image,O_RDONLY);

  char image_matrix[nscan][npix];
  bool flag[nscan][npix];

  if (fp1<0) {
    printf("Error in opening %s image \n",input_image);
    exit(1);
  }
  fp2=creat(output_image,0667);
  if (fp2<0) {
    printf("Error in creating output %s image\n",output_image);
    exit(1);
  }

  for (scan=0;scan<nscan;scan++) {
    read(fp1,&image[0],npix*sizeof(unsigned char));
    for (pix=0;pix<npix;pix++) {
        flag[scan][pix] = 0;
        image_matrix[scan][pix] = image[pix];
    }
  }

  for (i = 0; (int)sum != 0; ++i) {
    sum = 0;
    for (float p = i - 0.5; p < i + 0.5; p += 0.001) {
      sum += ( 1 / sqrt(2 * M_PI * 100) ) * exp(- ( pow(p, 2) / (2 * 100) ) ) * 0.001;
    }
    sum *= 512 * 512;
    gaussian_freq[i] = sum;
  }

  for (int j = 0; j <= i; j++) {
    while (gaussian_freq[j] != 0) {
      pix = rand() % npix;
      scan = rand() % nscan;

      if (flag[scan][pix] != 0) {
        continue;
      }

      flag[scan][pix] = 1;
      image_matrix[scan][pix] += j;
      gaussian_freq[j] -= 1;
    }
  }

  for (int j = -1; -j <= i; j--) {
    while (gaussian_freq[-j] != 0) {
      pix = rand() % npix;
      scan = rand() % nscan;
        
      if (flag[scan][pix] != 0) {
        continue;
      }
    
      flag[scan][pix] = 1;
      image_matrix[scan][pix] += j;
      gaussian_freq[j] -= 1;
    }
  }

  for (scan=0;scan<nscan;scan++) {
    write(fp2,&image_matrix[scan],npix*sizeof(unsigned char));
  }

  close(fp1);
  close(fp2);
}
