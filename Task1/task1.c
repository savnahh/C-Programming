//Savona Shrestha
//2330777
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void l_reg(FILE *fp, float *x_summ, float *y_summ, float *xy_summ, float *sq_xsumm, int *n){
    float x, y;
    //Reading data from all files and calculating summation
    while (fscanf(fp, "%f, %f", &x, &y) != EOF){
        (*x_summ) += x;
        (*y_summ) += y;
        (*xy_summ) += (x * y);
        (*sq_xsumm) += pow(x, 2);
        (*n)++;
    }
}

int main(){
    FILE *fp[4];
    float x_summ = 0;
    float y_summ = 0;
    float xy_summ = 0;
    float sq_xsumm = 0;
    float A, B, x_val, Y;
    int n = 0;
    int i;
    
    //Open files for reading
    fp[0] = fopen("datasetLR1.txt", "r");
    if(fp[0] == NULL){
		printf("File not found!!!");
		exit(EXIT_FAILURE);
	}
    fp[1] = fopen("datasetLR2.txt", "r");
    if(fp[1] == NULL){
		printf("File not found!!!");
		exit(EXIT_FAILURE);
	}
    fp[2] = fopen("datasetLR3.txt", "r");
    if(fp[2] == NULL){
		printf("File not found!!!");
		exit(EXIT_FAILURE);
	}
    fp[3] = fopen("datasetLR4.txt", "r");
    if(fp[3] == NULL){
		printf("File not found!!!");
		exit(EXIT_FAILURE);
	}
	
	//Calculating linear regression for each dataset
    for (i = 0; i < 4; i++){
        l_reg(fp[i], &x_summ, &y_summ, &xy_summ, &sq_xsumm, &n);
        fclose(fp[i]);
    }
    
    //Formulas for A and B
    A = ((y_summ * sq_xsumm) - (x_summ * xy_summ)) / ((n * sq_xsumm) - pow(x_summ, 2));
    B = ((n * xy_summ) - (x_summ * y_summ)) / ((n * sq_xsumm) - pow(x_summ, 2));

    printf("The values of A = %f and B = %f\n", A, B);
	printf("The linear regression obtained from the calculation is: ");
	printf("y = %.2f*x + %.2f\n", B, A);
	
	printf("Enter a value for x: ");
	scanf("%f", &x_val);
	
	Y = B * x_val + A;
	printf("The value of y from equation: %.2f", Y);

    return 0;
}
