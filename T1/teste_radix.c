#include <stdio.h>
#include <stdlib.h>

void printArray(int *array, int size){
  
  int i;
  printf("[ ");
  for (i = 0; i < size; i++)
    printf("%d ", array[i]);
  printf("]\n");
}

int findLargestNum(int *array, int size){
  
  int i;
  int largestNum = -1;
  
  for(i = 0; i < size; i++){
    if(array[i] > largestNum)
      largestNum = array[i];
  }
  
  return largestNum;
}

// Radix Sort
void radixSort(int *array, int size){
  
  //printf("\n\nRunning Radix Sort on Unsorted List!\n\n");

  // Base 10 is used
  int i;
  int semiSorted[size];
  int significantDigit = 1;
  int largestNum = findLargestNum(array, size);
  
  // Loop until we reach the largest significant digit
  while (largestNum / significantDigit > 0){
    
    //printf("\tSorting: %d's place ", significantDigit);
    //printArray(array, size);
    
    int bucket[10] = { 0 };
    
    // Counts the number of "keys" or digits that will go into each bucket
    for (i = 0; i < size; i++)
      bucket[(array[i] / significantDigit) % 10]++;
    
    /**
     * Add the count of the previous buckets,
     * Acquires the indexes after the end of each bucket location in the array
     * Works similar to the count sort algorithm
     **/
    for (i = 1; i < 10; i++)
      bucket[i] += bucket[i - 1];
    
    // Use the bucket to fill a "semiSorted" array
    for (i = size - 1; i >= 0; i--)
      semiSorted[--bucket[(array[i] / significantDigit) % 10]] = array[i];
    
    
    for (i = 0; i < size; i++)
      array[i] = semiSorted[i];
    
    // Move to next significant digit
    significantDigit *= 10;
    
    //printf("\n\tBucket: ");
    //printArray(bucket, 10);
  }
}

int main(){
  int cont = 0;
  int i = 1000000;
  int size = 1000000;
  //int list[1000000];
  //int *list;
  //list[0] = 1;
  //printf("LIST[0]%d\n", list[0]);
  
  int *list = malloc (1000000 * sizeof(int));
  while (cont < 1000000){
    list[cont] = i;
    i--;
    cont ++;
  }

  radixSort(list, 1000000);
  printf("\nSorted List:");
  printArray(list, 1000000);
  printf("\n");
  
  return 0;
}