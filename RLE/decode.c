#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <malloc.h>

void main()
{
    char filename[] = {"decoded.raw"};
    
    FILE *fp = fopen("encoded", "r");
    int fp2;
    fp2=creat(filename,O_CREAT);
    
    if (fp2<0){
        printf("Error in creating output %s image\n", "decoded.raw");
        exit(1);
	}
    
    unsigned char ch;
    int cnt;
    int pix, scan;
    int npix, nscan;
    
    printf("Enter npix, nscan:- ");
    scanf("%d%d", &npix, &nscan);
    
    char arr[npix][nscan];
    
    pix = 0;
    scan = 0;
    while (!feof(fp)) {
        ch = fgetc(fp);
        cnt = getw(fp);
        
        if (cnt != -1) {
            printf("%d -> %d\n", (int) ch, cnt);
            for (int i = 0; i < cnt; ++i) {
                if (pix < npix) {
                    pix += 1;
                    arr[scan][pix] = ch;
                }
                else {
                    scan += 1;
                    pix = -1;
                }
            }
        }
    }
    
    for (scan=0;scan<nscan;scan++) {
        write(fp2,&arr[scan][0],npix*sizeof(unsigned char));
    }
    
    fclose(fp);
    close(fp2);
}
