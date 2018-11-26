#include <stdio.h>
#include "stack.h"

int main()
{
  int i, num = 10;
  printf("Generating list with %d ints\n", num);
  Stack *stack = stackAlloc(sizeof(int)); 
  for(i = 1; i <= num; i++) {
    push(stack,&i);
  }
  for(i = 1; i <= num-2; i++) {
    int headData;
    pop(stack,&headData);
    printf("top value is: %d\n",headData);
  }
  freeStack(&stack);
  return 0;
}
