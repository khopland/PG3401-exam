#include <stdio.h>
int main(void) {
    int i;
    int j;
    // this is a for loop
    for (i = 0; i < 10; i++) {
        printf("Hello World");
    }
    // for loop wit \n between for and (
    for 
	
	(i = 0; i < 10; i++) {
        printf("Hello World");
    }

    // this is a for loop with no scoped body
    for (i = 0; i < 10; i++)
        printf("Hello World not body%d\n", i);

    // this is a for loop with no scoped body
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
            printf("Hello World loop no body %d\n", j + (10 * i));;

    // this is a for loop with no body
    for (i = 0; i < 10; i++)
        ;

    // for loop  in for loop
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            printf("Hello World %d\n", j + (10 * i));
        }
    }

    printf("Hello World");
    return 0;
};
