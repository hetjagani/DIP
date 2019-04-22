#include<math.h>

typedef struct {
	double real;
	double imag;
}COMPLEX;


void disp_com(COMPLEX num) {
	printf("(%lf + %lfi)",num.real, num.imag);
}

COMPLEX com_conj(COMPLEX num) {
  COMPLEX ans;
  ans.real = num.real;
  ans.imag = -num.imag;
  return ans;
}

COMPLEX com_mul(COMPLEX num1, COMPLEX num2) {
	COMPLEX ans;
	ans.real = (num1.real*num2.real) - (num1.imag*num2.imag);
	ans.imag = (num1.real*num2.imag) + (num2.real*num1.imag);
	return ans;
}

COMPLEX com_div(COMPLEX num1, COMPLEX num2) {
  COMPLEX ans;
  ans.real = ((num1.real*num2.real)+(num1.imag*num2.imag)) / (pow(num2.real,2)+pow(num2.imag,2));
  ans.imag = ((num1.imag*num2.real)-(num1.real*num2.imag)) / (pow(num2.real,2)+pow(num2.imag,2));
  return ans;
}

COMPLEX com_addi(COMPLEX num1, COMPLEX num2) {
	COMPLEX ans = {0.0, 0.0};
	ans.real = num1.real + num2.real;
	ans.imag = num1.imag + num2.imag;
	return ans;
}

COMPLEX com_sub(COMPLEX z1,COMPLEX z2)
{
	COMPLEX ans;
	ans.real = z1.real - z2.real;
	ans.imag = z1.imag - z2.imag;
	return ans;
}

double com_mag(COMPLEX num) {
  return sqrt(pow(num.real, 2) + pow(num.imag, 2));
}

void com_mat_mul(int ra, int ca, COMPLEX a[][ca], int rb, int cb, COMPLEX b[][cb], COMPLEX mul[ra][cb]) {
	int c,d,k;
	COMPLEX sum={0.0,0.0};
	for (c = 0; c < ra; c++) {
		for (d = 0; d < cb; d++) {
			for (k = 0; k < rb; k++) {
				sum = com_addi(sum, com_mul(a[c][k], b[k][d]));
			}
			mul[c][d] = sum;
			sum.real = 0;
			sum.imag = 0;
		}
	}
}

void com_transpose(int n, COMPLEX buff[n][n]){
  int i,j;
  for(i = 0; i < n; i++){
    for(j = i+1; j < n; j++){
      COMPLEX temp = buff[i][j];
      buff[i][j] = buff[j][i];
      buff[j][i] = temp;
    }
  }
}