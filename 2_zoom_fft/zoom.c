// args : <input file name> <nscan> <npix>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <malloc.h>
#include "fft.h"

int main(int argc, char const *argv[])
{
  int npix, nscan, i, j, padd;
  char image_fname[100], out_image_fname[100];

  if(argc < 3)
    printf("Invalid arguments...\n");

  strcpy(image_fname,argv[1]);
  nscan = atoi(argv[2]);
  npix = atoi(argv[3]);

  unsigned char image_buff[nscan][npix];
  int fp1,fp2;

  fp1 = open(image_fname, O_RDONLY);
  if(fp1 < 0)
    printf("Error opening image...\n");

  sprintf(out_image_fname,"%s_out",image_fname);

  fp2 = creat(out_image_fname,0667);
	if (fp2<0) {
		printf("Error in creating output %s image\n",out_image_fname);
		exit(1);
	}
  
  read(fp1, &image_buff[0][0], nscan*npix*sizeof(unsigned char));
  
  printf("INPUT:\n");
  for(j=0; j<nscan; j++){
    printf("%d ", image_buff[0][j]);
  }
  printf("\n\n");

  COMPLEX img_cplx[nscan][npix];
  for(i=0; i<nscan; i++) {
    for(j=0; j<npix; j++) {
      img_cplx[i][j].real = pow(-1,i+j)*image_buff[i][j];
      img_cplx[i][j].imag = 0;
    }
  }
  
  int zf;
  printf("Enter zoom factor (2,3,4) : ");
  scanf("%d", &zf);
  
  padd = (nscan * (zf-1)) / 2;
  printf("PADD : %d\n", padd);
  
  COMPLEX fft_ans[nscan][npix];
  fft_mat(nscan, img_cplx, fft_ans);

  int nrow = nscan + 2*padd;
  int ncol = npix + 2*padd;

  COMPLEX buffer[nrow][ncol];
  bzero(buffer, sizeof(COMPLEX)*nrow*ncol);
  
  for(i=padd; i<nscan+padd; i++) {
    for(j=padd; j<npix+padd; j++) {
      buffer[i][j] = fft_ans[i-padd][j-padd];
    }
  }
  
  double zoom[nrow][ncol];
  
  ifft_mat(nrow, buffer, zoom);
  
  unsigned char out_img_buffer[nrow][ncol];
  for(i=0; i<nrow; i++) {
    for(j=0; j<ncol; j++) {
      out_img_buffer[i][j] = (unsigned char)(abs(zoom[i][j]));
      if(i==0)
        printf("%d ",out_img_buffer[i][j]);
    }
  }
  write(fp2, &out_img_buffer[0][0], ncol*nrow*sizeof(unsigned char));

  close(fp1);
  close(fp2);
  return 0;
}

