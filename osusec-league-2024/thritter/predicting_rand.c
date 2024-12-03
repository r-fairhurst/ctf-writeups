#include <stdio.h>
#include <stdlib.h>

int main() {
    srand(0); // Hardcoded seed

    // Replicating the session ID generation
    printf(
			"%X%X%X%X\n",
            (unsigned int) rand (),
            (unsigned int) rand (),
            (unsigned int) rand (),
            (unsigned int) rand ());

    return 0;
}
