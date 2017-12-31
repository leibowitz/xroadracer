#include <stdlib.h>
#include <stdio.h>
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

int main()
{
        srand(time(NULL));

        int layers = 3;
        int ninputs = 19;
        //int hidden_n = 7;
        int hidden_n = random_number(3, 40);
        printf("%d\n", hidden_n);
        int noutput = 1;

        int iter = 200000;
        int p = 50;

        float r1 = 0.001;
        float r2 = 0.001;
        //float r3 = 0.15;
        float r3 = 0.08;
        float r4 = 0.08;
 struct fann *ann;
 
 ann = fann_create_standard(layers, ninputs, hidden_n, noutput);
 printf("fw\n");
 fann_train_on_file(ann, "map1_fw.txt", iter, p, r1);
 fann_save(ann, "map1_fw.net");
 //fann_print_parameters(ann);
 fann_destroy(ann);
 

 
 ann = fann_create_standard(layers, ninputs, hidden_n, noutput);
 printf("bw\n");
 fann_train_on_file(ann, "map1_bw.txt", iter, p, r2);
 fann_save(ann, "map1_bw.net");
 //fann_print_parameters(ann);
 fann_destroy(ann);
 

 
 ann = fann_create_standard(layers, ninputs, hidden_n, noutput);
 printf("right\n");
 fann_train_on_file(ann, "map1_right.txt", iter, p, r3);
 fann_save(ann, "map1_right.net");
 //fann_print_parameters(ann);
 fann_destroy(ann);
 

 
 //ann = fann_create_standard(2, ninputs, noutput);
 ann = fann_create_standard(layers, ninputs, hidden_n, noutput);
 printf("left\n");
 //ann = fann_create_standard(4, ninputs, 10, 6, noutput);
 //ann = fann_create_standard(5, ninputs, 10, 10, 12, noutput);
 //ann = fann_create_standard(6, ninputs, 10, 10, 8, 6, noutput);
 //fann_train_on_file(ann, "map1_left.txt", iter, p, 0.168);
 fann_train_on_file(ann, "map1_left.txt", iter, p, r4);
 fann_save(ann, "map1_left.net");
 //fann_print_parameters(ann);
 fann_destroy(ann);
 return 0;
}
