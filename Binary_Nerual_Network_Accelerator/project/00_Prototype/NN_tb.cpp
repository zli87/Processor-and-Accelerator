#include "NN.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;

//#define PRINT
uint16 string2uint(char* str){
	uint16 sum=0;
	for(int i=0;i< WORDSIZE;++i){
		if(str[15-i]-48) sum += pow(2,i);
		//printf("ggg %d %d \n",i,sum);
	}
	return sum;
}

void printmatrix(bit** M, uint16 dim1, uint16 dim2){
	for(int i=0;i<dim1;++i){
		for(int j=dim2-1;j>=0;--j){
			printf("%d ",(*M)[i*dim2+j]);
		}
		printf("[%d] \n",i);
	}
}

int main(){
	FILE * fpw = fopen("./sram/weight_sram.dat","r");
	FILE * fpi = fopen("./sram/input_sram.dat","r");
	FILE * fpo = fopen("./sram/golden_outputs.dat","r");

	char temp[WORDSIZE];
	char temp2[WORDSIZE];
	char *c[100];
	uint16 sizew;
	uint16 dim1,dim2,dim1o,dim2o;
	bit* weight;
	bit* input;
	bit* output;
	bit *ans;

		fscanf(fpw, "%s %s", &c, &temp );
		sizew = string2uint(temp);
		cout<<"weight size: "<<sizew<<endl;

		fscanf(fpw, "%s %s", &c, &temp );

		weight = (bit*)malloc(sizew*sizew*sizeof(bit));
                cout<<"w[0] ~ w[max]: ";
		for(int i=0;i<sizew*sizew;++i){
			weight[i] = temp[15-i]-48;
			cout<<weight[i]<<" ";
		}
		cout<<endl<<endl;

	for(int k=0;k<3;k++){
		fscanf(fpi, "%s %s", &c, &temp );
		dim1 = string2uint(temp);

		fscanf(fpi, "%s %s", &c, &temp );
		dim2 = string2uint(temp);

		input = (bit*)malloc(dim1*WORDSIZE*sizeof(bit));
		dim1o = dim1-sizew+1; // 14 = 16 -3 +1
		dim2o = dim2-sizew+1;
		output = (bit*)malloc(dim1o*WORDSIZE*sizeof(bit));
		memset(output,'\0',dim1o*WORDSIZE*sizeof(bit));
		ans = (bit*)malloc(dim1o*WORDSIZE*sizeof(bit));

		for(int i=0;i<dim1;++i){
			fscanf(fpi, "%s %s", &c, &temp );
			for(int j=0;j<WORDSIZE;++j){
				input[i*WORDSIZE+j] = temp[15-j]-48;
			}
		}
		cout<<"Input Matrix:"<<" dim1 ="<<dim1<<" dim2 ="<<dim2<<endl;
#ifdef PRINT
		for(int i=0;i<dim1;++i){
			for(int j=WORDSIZE-1;j>=0;--j){
				cout<<input[i*WORDSIZE+j]<<" ";
			}
			cout<<"["<<i<<"] "<<endl;
		}
		cout<<endl;
#endif

		conv2d( sizew, dim1, dim2, weight, input, output);
#ifdef PRINT
		cout<<"Output Matrix:"<<" dim1o ="<<dim1o<<" dim2o ="<<dim2o<<endl;
		for(int i=0;i<dim1o;++i){
			for(int j=WORDSIZE-1;j>=0;--j){
				cout<<output[i*WORDSIZE+j]<<" ";
			}
			cout<<"["<<i<<"]"<<endl;
		}
		cout<<endl;
#endif
		for(int i=0;i<dim1o;++i){
			fscanf(fpo, "%s %s", &c, &temp2 );
			for(int j=0;j<WORDSIZE;++j){
				ans[i*WORDSIZE+j] = temp2[15-j]-48;
				if( output[i*WORDSIZE+j] != ans[i*WORDSIZE+j]){
					cout<<"===================================================\n";
					cout<<"ERROR at output["<<i<<"]["<<j<<"]="<<output[i*dim2o+j];
					cout<<", ans["<<i<<"]["<<j<<"]="<<ans[i*dim2o+j]<<endl;
					cout<<"===================================================\n";
                    exit(1);
				}
			}
		}
		printf("==================================================\n");
		printf("               PASS TEST CASE %d    !!!\n",k+1);
		printf("==================================================\n");
	}
		printf("==================================================\n");
		printf("               PASS ALL TEST CASE !!!\n");
		printf("==================================================\n");
		free(ans);
		free(output);
		free(input);
		free(weight);

}
