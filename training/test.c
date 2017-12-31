#include <stdio.h>
#include "fann.h"

int main(int argc, char* argv[])
{
        FILE *fp;
        if(argc < 2) {
                exit(1);
        }
        printf("%s\n", argv[1]);
        char name[20];
        sprintf(name, "data%s.txt", argv[1]);
        printf("%s\n", name);

        if( (fp = fopen(name, "r+")) == NULL)
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
        sprintf(name, "data%s.net", argv[1]);
 struct fann *ann = fann_create_from_file(
 name);
//1276 8 4
//
//0.000000 80 101 182 199 103 44 30 
//
int i, j, k;
 float input[8];
fscanf(fp,"%d %d %d\n", &i, &j, &k);
        // skip line
        fscanf(fp, "%*[^\n]\n", NULL);
while(fscanf(fp,"%f %f %f %f %f %f %f %f \n", &input[0], &input[1], &input[2], &input[3], &input[4], &input[5], &input[6], &input[7]) == 8)  
{  
       printf("%f %f %f %f %f %f %f %f\n", input[0], input[1], input[2], input[3], input[4], input[5], input[6], input[7]);  
 output = fann_run(ann, input);
 //s = sizeof(*output);
 s = 1;
 for(i = 0; i < s; i++) {
  printf("%d: %d\n", i, (int)roundf(output[i]));
 }
       //printf("%d %d %d\n", i, j, k);  
        // skip line
        fscanf(fp, "%*[^\n]\n", NULL);
        // skip line
        //fscanf(fp, "%*[^\n]\n", NULL);
        // skip line
        fscanf(fp, "%*[^\n]\n", NULL);
}  
if(feof(fp))  
{            
         puts("EOF");
}  
printf("done\n");
 //if(argc != 2) error("Remember to specify an input file");
 return 0;
}
