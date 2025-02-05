#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

// SRAND Seed is printed in the outptut
void generate_otp(char *otp, size_t length) {
    srand(1738779153);  //Replace with your time(null) timestamp
    for (int i = 0; i < length; i ++) {
        *(otp+i) = (char) (rand() % 26) + 'a';
    }
}

void decrypt_bond(char *otp, char *bond, size_t length) {
    for (int i = 0; i < length; i++) {
        int decrypted_char = (bond[i] - 'a') - (otp[i] - 'a');
        if (decrypted_char < 0) {
            decrypted_char += 26; 
        }
        printf("%c", (decrypted_char % 26) + 'a');
    }
}


int main(void){
    char* ciphertext = "smpqsrvwvblulhfd"; //Replace with your encrypted flag
    char otp[17] = {0};

    // Modeled off of code flow from main function 
    int len = 16;
    generate_otp(otp, len);
    printf("osu{");
    decrypt_bond(otp, ciphertext, len);
    printf("}\n");

}