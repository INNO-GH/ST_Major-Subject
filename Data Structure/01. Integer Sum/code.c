#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

long long sum_between_two(int a, int b) {
    long long k=0;
    if (a>b) {
        while (a>=b) {
            k=k+b;
            b=b+1;
        }
    }
    else if (a==b) {
            k=a;
    }
    else {
        while (b>=a) {
             k=k+a;
             a=a+1;
        }
    }
    return k;
}

int main(int argc, char **argv) {
    fin=fopen(argv[1], "r");
    fout=fopen(argv[2], "w");
    
    int a, b;
    long long sum;

    while(fscanf(fin, "%d%d", &a, &b)!=EOF) {
        sum = sum_between_two(a,b);
        fprintf(fout, "sum of (%d) and (%d) is %lld\n", a, b, sum);
    }

    fclose(fin);
    fclose(fout);

    return 0;
}

