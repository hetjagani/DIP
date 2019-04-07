#include <stdio.h>
#include "comp.h"


void _fft(COMPLEX buff[], COMPLEX out[], int N, int step);
void fft(COMPLEX buf[], int n);


void main(int argc, char* argv[]){
  COMPLEX buff[8];
  int i;

  printf("Enter the sequence : \n");
  for(i=0; i<8; i++){
      scanf("%lf%lf", &buff[i].real, &buff[i].imag);
  }
  fft(buff, 8);

  printf("Answer \n");
  for(i=0;i<8; i++){
    disp_com(buff[i]);
    printf("\n");
  }
}


void _fft(COMPLEX buff[], COMPLEX out[], int N, int step){
  if (step < N){
    _fft(out, buff, N, step * 2);
    _fft(out + step, buff + step, N, step * 2);

    for(int i = 0; i < N; i += 2 * step) {
      COMPLEX t = {0.0,0.0};
      t.real = cos(-i * M_PI / N);
      t.imag = sin(-i * M_PI / N);
      COMPLEX temp = com_mul(t, out[i+step]);
      buff[i/2] = com_addi(out[i], temp);
      buff[(i+N)/2] = com_sub(out[i], temp);
    }
  }
}

void fft(COMPLEX buf[], int n) {
	COMPLEX out[n];
	for (int i = 0; i < n; i++) out[i] = buf[i];
 
	_fft(buf, out, n, 1);
}
