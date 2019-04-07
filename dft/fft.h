#include "comp.h"

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

void fft_mat(int N, COMPLEX arr[N][N], COMPLEX fft_ans[N][N]) {
  int i,j;
  COMPLEX buff[N];
  for(i=0; i<N; i++) {
    for(j=0; j<N; j++) {
      buff[j].real = (arr[i][j].real);
      buff[j].imag = (arr[i][j].imag);
    }
    fft(buff, N);
    for(j=0; j<N; j++) {
      fft_ans[i][j].real = buff[j].real;
      fft_ans[i][j].imag = buff[j].imag;
    }
  }

  com_transpose(N, fft_ans);

  for(i=0; i<N; i++) {
    for(j=0; j<N; j++) {
      buff[j].real = fft_ans[i][j].real;
      buff[j].imag = fft_ans[i][j].imag;
    }
    fft(buff, N);
    for(j=0; j<N; j++) {
      fft_ans[i][j].real = buff[j].real;
      fft_ans[i][j].imag = buff[j].imag;
    }
  }

  com_transpose(N, fft_ans);
}

void ifft_mat(int N, COMPLEX fft[N][N], double arr[N][N]) {
  int i,j;
  COMPLEX orig[N][N];

  for(i=0;i<N;i++) {
    for(j=0; j<N; j++){
      fft[i][j] = com_conj(fft[i][j]);
    }
  }
 
  fft_mat(N, fft, orig);

  for(i=0;i<N;i++) {
    for(j=0; j<N; j++){
      arr[i][j] = com_conj(orig[i][j]).real;
      arr[i][j] /= 64*64;
    }
  }
}
