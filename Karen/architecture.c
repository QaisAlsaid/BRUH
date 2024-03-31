#ifdef __ARM_ARCH
#define arc 1
#elif __x86_64__
#define arc 2
#else 
#define arc 0
#endif
#include <stdio.h>
#include <stdlib.h>
int main()
{
  char* arc_str = (char*) alloca(sizeof(char) * 4);
  switch(arc)
  {
    case 1:
      arc_str = "arm\0";
      break;
    case 2:
      arc_str = "x86\0";
      break;
    case 0:
      arc_str = "idk\0";
      break;
  }
  printf(arc_str);
}
