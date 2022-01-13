#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#ifdef USE_MYMATH
  #include "math/myMath.h"
#else
  #include <math.h>
#endif
int sub(int a, int b) {
    return (a - b);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s argv[1] argv[2] \n", argv[0]);
        return 1;
    }
    int a = atof(argv[1]);
    int b = atoi(argv[2]);

#ifdef USE_MYMATH
    printf("Now we use our own Math library. \n");
    int result = add(a, b);
    printf("%d + %d = %d\n", a, b, result);
#else
    printf("Now we use the main.cpp sub function. \n");
    int result = sub(a, b);
    printf("%d - %d = %d\n", a, b, result);
#endif
    return 0;
}
