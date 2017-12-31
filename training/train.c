#include "fann.h"

int main()
{
 struct fann *ann;
 ann = fann_create_standard(3, 8, 15, 1);
 fann_train_on_file(ann, "data1.txt", 2000, 10, 0.01);
 fann_save(ann, "data1.net");
 fann_destroy(ann);

 ann = fann_create_standard(3, 8, 15, 1);
 fann_train_on_file(ann, "data2.txt", 2000, 10, 0.01);
 fann_save(ann, "data2.net");
 fann_destroy(ann);

 ann = fann_create_standard(3, 8, 15, 1);
 fann_train_on_file(ann, "data3.txt", 2000, 10, 0.01);
 fann_save(ann, "data3.net");
 fann_destroy(ann);

 ann = fann_create_standard(3, 8, 15, 1);
 fann_train_on_file(ann, "data4.txt", 2000, 10, 0.01);
 fann_save(ann, "data4.net");
 fann_destroy(ann);
 return 0;
}
