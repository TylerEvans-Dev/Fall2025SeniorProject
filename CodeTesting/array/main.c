#include <stdio.h>
#include <stdlib.h>

int main(){
    //creates an array of 100 items;
    int numberOfItems= 100;
    int total = 0;
    float *arr = (float *)malloc(sizeof(float)*numberOfItems);
    //fills up an array with diffrent values depending on random time
    for(int i =0; i < numberOfItems; i++){
        arr[i] = rand() % 3;
        printf("the value is %.2f\n", arr[i]);
        total+=1;
    }
    if(total == numberOfItems){
        numberOfItems *= 10;
        float *tempray = (float *)malloc(sizeof(float)*numberOfItems);
        for(size_t j = 0; j < total; j++){
            tempray[j] = arr[j];
        }
        arr = &tempray;
        arr[101] = 50.0f;
        arr[500] = 80.0f;
        free(tempray);
    }
    printf("the last new item is %.2f\n", arr[101]);
    printf("the 500 new item is %.2f\n", arr[101]);

    //free the mem.
    free(arr);
    return 0;
}
