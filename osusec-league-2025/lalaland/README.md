# lalaland
- **Category:** crypto
- **Points:** 110 (2 flags)
- **Description:** oh no no no, you’re not decrypting that flag, absolutely not, but oh? what’s this? my Extremely Convenient Breaker, just sitting here, existing, being, perhaps, useful? hm. anyway, rsa, ugh, such a drag, right? but imagine, just imagine, a world where you didn’t have to. wild concept. maybe even... a service. a new one. mine, perhaps.

- Flag 1: 50 points
    - Access: nc chal.ctf-league.osusec.org 1315
    - Download: http://chal.ctf-league.osusec.org/crypto/lalaland/convenient.py
- Flag 2: 60 points
    - Access: nc chal.ctf-league.osusec.org 1316
    - Download: http://chal.ctf-league.osusec.org/crypto/lalaland/rsa.py

## Solution for flag1
### first look
accessing the server we get the following:
```shell
Here's the encrypted flag in hex: 
42217643bb0dc5d0593dbca44b1e6dd81ea10202f9e9aae9d5cf175cbc3f378cf6be9a16c042efcccdacd43a7f01596c98b99e39b73973cc317924007acac7d1
Alright, lemme spin up my Extremely Convenient Breaker (trademark copyright all rights reserved). 
What ciphertext do you want me to break in an extremely convenient manner? Enter as hex: 
```

it gives us the encrypted flag right off the bat, and then asks what ciphertext we want to decrypt, trying to copy and past the encrypted flag into the input doesn't work
```shell
What ciphertext do you want me to break in an extremely convenient manner? Enter as hex: 33c6fb4a654c4eb3d32db0b42cc1e06a2c7ad72c30ec402d1e1037c5eb0d2034b205cb41ace5023fef3a1a37613ff0b48e622ed2b31e9ffcc797516c7e8487ff
No, I'm not decrypting the flag. 
```

I have also noticed that each time we access the challenge, it gives us a different cipher text, so I don't think we can copy and paste old cipher texts to decrpyt them.

now we will take a peak at the convenient.py file (which is the source code)

### looking at the convenient.py file
```python
key = os.urandom(16)
with open("flag.txt", "r") as f:
    flag = f.readline().strip()
cipher = AES.new(key, AES.MODE_ECB)

flag_enc = cipher.encrypt(flag.encode())
print("Here's the encrypted flag in hex: ")
print(flag_enc.hex())
```

this encryption generates a random key of 16 bytes, reads the flag from a file, and the encrypts the flag with aes in ecb mode.

```python
while True:
    ecb = input("What ciphertext do you want me to break in an extremely convenient manner? Enter as hex: ")
    try:
        ecb = bytes.fromhex(ecb)
        if not len(ecb) == 64:
            print("Sorry, it's not *that* convenient. Make your ciphertext 64 bytes please. ")
        elif ecb == flag_enc:
            print("No, I'm not decrypting the flag. ")
        else:
            print(cipher.decrypt(ecb))
    except Exception:
        print("Uh something went wrong, please try again. ")
```

this is the loop that we interact with while connected to the server, which asks for a cipher text, checks if it is 64 bytes long, and then looks if our input is exactly the same as the encrypted flag, if it is, it won't do anything

but if it isn't, it will try and decrypt the input with the key that was used to encrypt the flag

so I connected to the server, copy and pasted the flag, and changed the last byte of the flag to somes thing else and we get part of a flag decrypted
```shell
b'osu{this_chal_brought_to_you_by_lactf_evryone_saqq\r\xe5Nw\xe9\x8c\x908\xa6\x9f)\xc3\x8f\xf7'
```

so we have part of the flag:

#### first part of flag 1
osu{this_chal_brought_to_you_by_lactf_evryone_

now we have to get a different chunk of it, so I changed the first byed of the flag to something else to see if we can get a different chunk of the flag

and we get a different chunk of the flag

```shell
b'\x9c\xa8\x01r\xca}\xf29\x1fw\xbf\x83\xe4\xdb\x13\xbaought_to_you_by_lactf_evryone_say_thank_u_lactf}'
```

#### Second part of flag 1
```shell
say_thank_u_lactf}
```

### flag 1 
osu{this_chal_brought_to_you_by_lactf_evryone_say_thank_u_lactf}

## Solution for flag2
### first look
connecting to the server we get the following:
```shell
Welcome to my RSA as a Service! 
Pass me two primes and I'll do the rest for you. 
Let's keep the primes at a 64 bit size, please. 
Input p: 
```

trying to break the program with a large number/a string doesn't lead to anything

### looking at the rsa.py file
```python
while True:
    p = input("Input p: ")
    q = input("Input q: ")
    try:
        p = int(p)
        q = int(q)
        assert isPrime(p)
        assert isPrime(q)
```

the source code asks for two primes and ensures both are prime numbers

if we look further after the primes

```python
except Exception:
    print("Oh no! My service! Please don't give us a bad review! ")
    print("Here, have a complementary flag for your troubles. ")
    with open("flag.txt", "r") as f:
        print(f.read())
```

within the try block if we somehow break the program and get past all the checks, it will print the flag

### breaking the program
so far we know that at a minimum we need two primes

the next rules are the following:
```python
try:
    assert p != q
except:
    print("You should probably make your primes different. ")
    continue

try:
    assert (p > 2**63) and (p < 2**64)
    assert (q > 2**63) and (q < 2**64)
    break
```

the primes have to be differnt and between 2^63 and 2^64

once we have correct primes it will encrypt the flag

```python
print("1. Encrypt 2. Decrypt 3. Exit ")
choice = input("Pick an option: ")

if choice == "1":
    msg = input("Input a message (as an int): ")
    try:
        msg = int(msg)
    except:
        print("Hm, looks like something's wrong with your message. ")
        continue
    encrypted = pow(msg, e, n)
    print("Here's your ciphertext! ")
    print(encrypted)

elif choice == "2":
    ct = input("Input a ciphertext (as an int): ")
    try:
        ct = int(ct)
    except:
        print("Hm, looks like something's wrong with your message. ")
        continue
    decrypted = pow(ct, d, n)
    print("Here's your plaintext! ")
    print(decrypted)

else:
    print("Thanks for using my service! ")
    print("Buh bye! ")
    break
```

here it gives us the option to encrypt or decrypt a message, we want to attempt to break out of this while loop in some fashion.

another location I noticed that might just cause the program to break is the following:
```python
d = pow(e, -1, phi)
```

Trying to search up how to break this I found that if choose two primes p,qp,q such that (p−1)×(q−1)(p−1)×(q−1) is a multiple of 65537.
This ensures gcd⁡(e,ϕ)≠1gcd(e,ϕ)=1, making pow(e, -1, phi) fail and might trigger the exception block to get the flag

### breaking the program
I crafted a script that will generate two primes, between 2^63 and 2^64, and then check if (p-1)*(q-1) is a multiple of 65537, if it is, it will connect to the server and send the primes

this script if the sol.py script

running it gets us:

```shell
╰─ python3 sol.py                                                                                                                              ─╯
9223372036854775837 9223372036859527241
[+] Opening connection to chal.ctf-league.osusec.org on port 1316: Done
[*] Switching to interactive mode
Oh no! My service! Please don't give us a bad review! 
Here, have a complementary flag for your troubles. 
osu{definitely_did_not_steal_this_challenge_from_lactf_like_subscr}
```

and there is the flag

#### flag 2
osu{definitely_did_not_steal_this_challenge_from_lactf_like_subscr}
