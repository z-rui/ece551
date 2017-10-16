#include "IntArray.h"
#include <iostream>
#include <cstdlib>

int maxIndex(const IntArray & array) {
  if (array.size() == 0) {
    return -1;
  }
  int answer = 0;
  for (int i = 1; i < array.size(); i++) {
    if (array[i] > array[answer]) {
      answer = i;
    }
  }
  return answer;
}

void randomize(IntArray & array) {
  for (int i = 0; i < array.size(); i++) {
    int temp(array[i]);
    int swapInd = random() % array.size();
    array[i] = array[swapInd];
    array[swapInd] = temp;
  }
}

void randomizeAndPrint(IntArray & array) {
  randomize(array);
  std::cout << array << std::endl;
  std::cout << "Largest element at index " << maxIndex(array) << std::endl;

}

int main(void) {
  IntArray intArray(10);
  for (int i  = 0; i < intArray.size(); i++) {
    intArray[i] = i*i;
  }
  IntArray intArray2(intArray);
  
  if (intArray == intArray2) {
    std::cout << "The two int arrays are the same\n";
  }
  else {
    std::cout << "The two int arrays are different\n";
  }

  randomizeAndPrint(intArray);

  std::cout << "intArray2 is " << intArray2 << std::endl;
  if (intArray == intArray2) {
    std::cout << "The two int arrays are the same\n";
  }
  else {
    std::cout << "The two int arrays are different\n";
  }

  intArray2 = intArray;
  std::cout << "Now intArray2 is "<< intArray2 << std::endl;
  if (intArray != intArray2) {
    std::cout << "The two int arrays are different\n";

  }
  else {
    std::cout << "The two int arrays are the same\n";
  }

  return EXIT_SUCCESS;
}
