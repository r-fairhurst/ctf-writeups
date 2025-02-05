int authorize(char *pin) {
    printf("%ld:LOG: verifying authorization...\n", time(NULL));
    for (int i = 0; i < PIN_LENGTH; i++) {
        if (*(pin + i) != PIN[i]) {
            // request proprietary hydraulic security module to open bolt
            lock_all_bolts();
            fflush(stdout);
            return -1;
        }
        // request proprietary hydraulic security module to open bolt
        open_bolt(i);
        if (i == 0) {
            printf("%ld:LOG: 1/12 bolts opened...\n", time(NULL));
        }
        if (i == 5) {
            printf("%ld:LOG: 6/12 bolts opened. Safe is halfway unlocked.\n", time(NULL));
        }
        fflush(stdout);
    }
    return 0;
}

void encrypt_bond(char *otp, char *bond, size_t length) {
    for (int i = 0; i < length; i ++) {
        printf("%c", ((otp[i]-'a') + bond[i]-'a') % 26 + 'a');
    }
}

void generate_otp(char *otp, size_t length) {
    printf("%ld:LOG: generating secure key\n", time(NULL));
    srand(time(NULL));
    for (int i = 0; i < length; i ++) {
        *(otp+i) = (char) (rand() % 26) + 'a';
    }
}

int main() {
    char pin[PIN_LENGTH+1] = {0};
    printf("Kanatomi Corp. Industrial-Grade Safe\n");
    printf("Please authorize your login\n");
    fflush(stdout);
    fgets(pin, PIN_LENGTH+1, stdin);
    
    if (authorize(pin) == 0) {
        printf("%ld:LOG: authorization authorized...\n", time(NULL));
        char otp[17] = {0};
        generate_otp(otp, FLAG_LENGTH);
        printf("Encrypting bearer bonds with time-secure OTP dynamic caesar cipher:\n");
        printf("osu{");
        encrypt_bond(otp, FLAG, FLAG_LENGTH);
        printf("}\n");
        printf("Don't forget to lock the door on your way out\n");
        printf("Have a nice day\n");
        printf("%ld:LOG: wished administrator a nice day :)\n", time(NULL));
    } else {
        printf("%ld:LOG: AUTHORIZATION DENIED (incorrect PIN)\n", time(NULL));
    }
    fflush(stdout);
}
