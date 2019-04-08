// Include header files (gives you access to standard library functions).
#include <stdio.h>
#include <stdlib.h>

// Foward declaration of functions.                
// A function must be either declared or defined before it is used.
// This lets you use a function that you will define later in the file.
// Each declaration gives the return type, the name, and the parameters.
void swap1(int x, int y);
void swap2(int *x, int *y);
int function3(int x, int y);

// Every C program has one main function. This is where the program starts.
int main(int argc, char** argv) {

  int x = 0; // Always initialize a variable before using it
  int y = 0;
    
  int *p;
  p = &x; // Make p a pointer to x

  *p = 1; // Set the value the p points to (x) to 1

  // printf is declared in stdio.h
  printf("The value of x is: %d\n", x);

  x = 0;
  y = 1;
  swap1(x, y);
  printf("swap1(0, 1) = (%d, %d)\n", x, y);
  
  x = 0;
  y = 1;
  swap2(&x, &y);
  printf("swap2(0, 1) = (%d, %d)\n", x, y);


  // The return value from main is typically used to indicate if the program succeeded
  // 0 means successful, anything else means there was an error
  return 0;
}

// Other function definitions.
// This is where we implement the functions we declared above.
void swap1(int x, int y) {
  int temp;
  temp = x;
  x = y;
  y = temp;
  return;
}

void swap2(int *x, int *y) {
  int temp;
  temp = *x;
  *x = *y;
  *y = temp;
  return;
}

int function3(int x, int y) {
  switch (x) {
    case 0:
      x = x + 0 * y;
      break;
    case 1:
      x = x + 1 * y;
      break;
    case 2:
      x = x + 2 * y;
      break;
    default:
      x = x + 4 * y;
      break;
  }

  return x;
}
