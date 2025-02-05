# bearybonds
- **Category:** crypto
- **Points:** 140 (1 flag)

## Description:
Your name? Beary Bonds. Your favorite thing in the world? Bearer bonds. You've heard that Kanatomi Corp has billions and billions of that fine fine filigree paper, and you've just gotta get your hands all over 'em. One catch - they're behind industrial-grade encryption and wrapped in a mystery and also inside an enigma. Can you use some leaked source code to get inside the safe? Yipee ki-yay!

## Access Details:
- **Flag 1:** 140 points
- **Access:** nc chal.ctf-league.osusec.org 1314
- **Download:** https://chal.ctf-league.osusec.org/crypto/bearybonds/leak.c

# Solution
## First connection to server
First we downloaded teh file 'leak.c' and attempted to access the challenge server using the domain and port provided.

we were greated with a prompt to authorize our login

```shell
Kanatomi Corp. Industrial-Grade Safe
Please authorize your login
```

trying just 1234 as the input we got the following response

```shell
1234
1738778202:LOG: verifying authorization...
1738778203:LOG: 1/12 bolts opened...
1738778206:LOG: AUTHORIZATION DENIED (incorrect PIN)
```

So that means maybe 1 of the 4 digits is correct. and we need to get the other 12 digits correct.

we then tried to break the program by entering a long string of characters and this did not work


## Examining the code 'leak.c'
since we have access to a sanitized version of the source code we can see what is going on

```c
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
```

from the looks of things if we get the first pin and the 6th pin correct, it will tell us, but not the other digits in the pin

so from our previous input we know that the first digit is 1

### getting the pin
while we were trying different combinations of the pin we noticed that it would sometimes take longer or shorter time to verify the pin

Writing a small script to get how long it actually takes, we can see that if there is a correct pin it will take exactly 1 second to verify that pin, and if it was wrong, its pretty fast.

using this information I attempted to automate a script to semi-brute force the pin

but before I was able to finish it one of our other team members was able to get the pin just through plain guess and check

```shell
ping=123457801337
```

### getting the flag
now that we have the pin, the server will give us an encrypted flag that was encrypted with a one time pad

```shell
1738779153:LOG: authorization authorized...
1738779153:LOG: generating secure key
Encrypting bearer bonds with time-secure OTP dynamic caesar cipher:
osu{smpqsrvwvblulhfd}
Don't forget to lock the door on your way out
Have a nice day
1738779153:LOG: wished administrator a nice day :)
```

taking a peek back at the source code it uses a semi random seed that is time based to encrypt the flag

## Decrypting the flag
the source code appears to use a time based seed to encrypt the flag, so we can use the same seed to decrypt the flag, where it gets the current time when it calls the encrypt function

and conviently the server also gives us the time it used to encrypt the flag

```c
int main () {
    ...
    generate_otp(otp, FLAG_LENGTH);
    printf("Encrypting bearer bonds with time-secure OTP dynamic caesar cipher:\n");
    printf("osu{");
    encrypt_bond(otp, FLAG, FLAG_LENGTH);
    printf("}\n");
    ...
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
```

so after we give the server the pin, it will give us the time it used to encrypt the flag, and we can use that time to decrypt the flag

```shell
1738779153:LOG: generating secure key
Encrypting bearer bonds with time-secure OTP dynamic caesar cipher:
osu{smpqsrvwvblulhfd}
```

so we have the time: 1738779153

and our encrypted flag: osu{smpqsrvwvblulhfd}

just copy and pasting the encryption function into a seperate c file and hard coding the time to the time we got from the server, we can use this to get the same otp that was used to encrypt the flag

I also wrote a decrypt function to decrypt the flag automatically

this code can be found in 'flag_crafter.c'

and all you have to do to use it is change the hardcoded values, compile it, and run it

### Flag
```shell
osu{twentyonthebeach}
```

**I do not own nor did I create the content for this challenge. This is a writeup for the challenge that I solved**
