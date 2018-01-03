#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "fann.h"

int random_number(int min_num, int max_num)
{
    int result = 0, low_num = 0, hi_num = 0;

    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1; // include max_num in output
    } else {
        low_num = max_num + 1; // include max_num in output
        hi_num = min_num;
    }

    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}

int main(int argc, char *argv[])
{
        srand(time(NULL));

        //char *name = "map1\0";
        int i = 0;
        while(i < argc) {

                printf("argv: %s\n", argv[i]);
                i++;
        }
        if (argc < 4) {
                exit(1);
        }
        char *name = argv[1];
        char *data_type = argv[2];
        char *output_name = argv[3];


        int layers = 3;
        int ninputs = 18;
        //int hidden_n = 7;
        int hidden_n = random_number(12, 30);
        //int hidden_n = 14;
        int hidden_n_2 = random_number(1, hidden_n);
        printf("%d %d\n", hidden_n, hidden_n_2);
        int noutput = 1;

        int iter = 200000;
        int p = 50;

 struct fann *ann;
 
 char *full_name = NULL;
 char *tmp_name = NULL;
        float r = 0;

 //char *data_type = "offroad\0";
// char *data_type = "road\0";
 if(strncmp(data_type, "road", 4) == 0) {
        if(strncmp(output_name, "fw", 2) == 0) {
                r = 0.03;
        }
        else if(strncmp(output_name, "bw", 2) == 0) {
                r = 0.09;
        }
        else if(strncmp(output_name, "right", 5) == 0) {
                r = 0.035;
        }
        else if(strncmp(output_name, "left", 4) == 0) {
                r = 0.09;
        }
        else {
                exit(1);
        }
 } else {
        if(strncmp(output_name, "fw", 2) == 0) {
                r = 0.02;
        }
        else if(strncmp(output_name, "bw", 2) == 0) {
                r = 0.02;
        }
        else if(strncmp(output_name, "right", 5) == 0) {
                r = 0.07;
        }
        else if(strncmp(output_name, "left", 4) == 0) {
                r = 0.09;
        }
        else {
                exit(1);
        }
 }

 tmp_name = malloc (sizeof (char) * strlen(name) + strlen(data_type) + 10);
 full_name = malloc (sizeof (char) * strlen(name) + strlen(data_type) + 15);

 //output_name
 sprintf(tmp_name, "%s_%s_%s", name, data_type, output_name);

 if (layers == 4) {
        ann = fann_create_standard(layers, ninputs, hidden_n, hidden_n_2, noutput);
 } else {
         ann = fann_create_standard(layers, ninputs, hidden_n, noutput);
 }
 printf("%s %s\n", output_name, tmp_name);
 sprintf(full_name, "%s.txt", tmp_name);
 fann_train_on_file(ann, full_name, iter, p, r);
 sprintf(full_name, "%s.net", tmp_name);
 printf("saving %s\n", full_name);
 fann_save(ann, full_name);
 //fann_print_parameters(ann);
 fann_destroy(ann);

 /*sprintf(tmp_name, "%s_%s_bw", name, data_type);
 
 if (layers == 4) {
 ann = fann_create_standard(layers, ninputs, hidden_n, hidden_n_2, noutput);
 } else {
 ann = fann_create_standard(layers, ninputs, hidden_n, noutput);
 }
 printf("bw %s\n", tmp_name);
 sprintf(full_name, "%s.txt", tmp_name);
 fann_train_on_file(ann, full_name, iter, p, r2);
 sprintf(full_name, "%s.net", tmp_name);
 printf("saving %s\n", full_name);
 fann_save(ann, full_name);
 //fann_print_parameters(ann);
 fann_destroy(ann);
 

 sprintf(tmp_name, "%s_%s_right", name, data_type);
 
 if (layers == 4) {
 ann = fann_create_standard(layers, ninputs, hidden_n, hidden_n_2, noutput);
 } else {
 ann = fann_create_standard(layers, ninputs, hidden_n, noutput);
 }
 printf("right %s\n", tmp_name);
 sprintf(full_name, "%s.txt", tmp_name);
 fann_train_on_file(ann, full_name, iter, p, r3);
 sprintf(full_name, "%s.net", tmp_name);
 printf("saving %s\n", full_name);
 fann_save(ann, full_name);
 //fann_print_parameters(ann);
 fann_destroy(ann);
 

 sprintf(tmp_name, "%s_%s_left", name, data_type);
 
 //ann = fann_create_standard(2, ninputs, noutput);
 if (layers == 4) {
 ann = fann_create_standard(layers, ninputs, hidden_n, hidden_n_2, noutput);
 } else {
 ann = fann_create_standard(layers, ninputs, hidden_n, noutput);
 }
 printf("left %s\n", tmp_name);
 sprintf(full_name, "%s.txt", tmp_name);
 //ann = fann_create_standard(4, ninputs, 10, 6, noutput);
 //ann = fann_create_standard(5, ninputs, 10, 10, 12, noutput);
 //ann = fann_create_standard(6, ninputs, 10, 10, 8, 6, noutput);
 //fann_train_on_file(ann, "map1_left.txt", iter, p, 0.168);
 fann_train_on_file(ann, full_name, iter, p, r4);
 sprintf(full_name, "%s.net", tmp_name);
 printf("saving %s\n", full_name);
 fann_save(ann, full_name);
 //fann_print_parameters(ann);
 fann_destroy(ann);*/



 free(tmp_name);
 free(full_name);
 return 0;
}
