#include <stdio.h>
#include "fann.h"

int main(int argc, char* argv[])
{
        FILE *fp;
        if(argc < 1) {
                exit(1);
        }
        printf("%s\n", argv[1]);

        if( (fp = fopen("data.txt", "r+")) == NULL)
        {
                printf("o\n");
                exit(1);
        }
if(fp == NULL) {
                printf("p\n");
                exit(1);
}
int s;
float *output;
 struct fann *ann = fann_create_from_file(
 "data.net");
//1276 8 4
//
//0.000000 80 101 182 199 103 44 30 
//
int i, j, k;
 float input[8];
while(fscanf(fp,"%d %d %d", &i, &j, &k) == 3)  
{  
       //printf("%d %d %d\n", i, j, k);  
        // skip line
        fscanf(fp, "%*[^\n]\n", NULL);
        // read input
        fscanf(fp,"%f %f %f %f %f %f %f %f", &input[0], &input[1], &input[2], &input[3], &input[4], &input[5], &input[6], &input[7]);
        // skip line
        fscanf(fp, "%*[^\n]\n", NULL);
       printf("%f %f %f %f %f %f %f %f\n", input[0], input[1], input[2], input[3], input[4], input[5], input[6], input[7]);  
 output = fann_run(ann, input);
 s = sizeof(*output);
 for(i = 0; i < s; i++) {
  printf("%d: %d\n", i, (int)roundf(output[i]));
 }
}  
if(feof(fp))  
{            
         puts("EOF");
}  
 //if(argc != 2) error("Remember to specify an input file");
 return 0;
}
