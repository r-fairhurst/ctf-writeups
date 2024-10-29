#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char userInput[100];
    int out[10] = {0};
    int delta = 0;

    printf("Enter a string at least 10 characters long: ");
    fgets(userInput, sizeof(userInput), stdin);

    // Remove newline character if present
    size_t len = strlen(userInput);
    if (userInput[len - 1] == '\n') {
        userInput[len - 1] = '\0';
        len--;
    }

    if (len < 10) {
        printf("String is too short\n");
        return 1;
    } else {
        printf("String is long enough\n");
    }

    printf("Calculating outputs for make_moves: \n");
    for (int i = 0; i < 10; i++) {
        userInput[i] = (char)(userInput[i] + delta);
        out[i] = userInput[i] % 4;
        delta = (userInput[i] + 7331) % 21;
        printf(" %d  ", out[i]);
    }

    printf("\nCalculating corresponding inputs\n");
    printf("enter in: \n");
    for (int i = 0; i < 10; i++) {
        int u;
        int choice = out[i];

        if (choice == 0) {
            u = 1; // User should choose 1 to win against 0
        } else if (choice == 1) {
            u = 2; // User should choose 2 to win against 1
        } else if (choice == 2) {
            u = 0; // User should choose 0 to win against 2
        } else {
            u = 0; // Default case, should not happen
        }

        printf(" %d  ", u);
    }
    return 0;
}




