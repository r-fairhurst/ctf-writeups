# Name: michaelsoft_binbows
- Category: rev
- Points: 110 (2 flags)
## Description:
u got da $200 key right
- Flag 1: 50 points
- Flag 2: 60 points
- Access: nc chal.ctf-league.osusec.org 1312
- Download: http://chal.ctf-league.osusec.org/rev/binbows/binbows

## First look at the server
Connecting to the server we get the following output:
```shell
Binbows (TM) Operating System
Copyright (C) Michaelsoft Corp. All your rights are belong to us.

Your copy of Michaelsoft Binbows needs activation!
Please enter your product key (a-z lowercase, 32 characters):
```

inputting anything over 32 bits, breaks the server saying:

```shell
Critical system error! Binbows has detected tampering. Please do not leave the room; your device will explode shortly :)
```

anything under 32 chars or exactly 32 chars, the server does not validate the key and closes

```shell
Validating product key with Michaelsoft servers...
While you wait, here's some fun Michaelsoft facts!
With new Copilot for Business, Michael himself will respond to your questions! Plans start at $2,000,000/month!
Did you know that in Michaelsoft Binbows, pressing ctrl+windows+alt+shift+l opens LinkedIn in your default browser? This way, you can stay up to date with all of the exciting updates to Michaelsoft 365!
Error: Key must be 32 characters long.
Error: Invalid product key >:(. Remember kids, you wouldn't download a car. So why download an operating system?
Please purchase a valid Binbows license to continue.
```

## Looking at the binary

We have a flag free version of the binary, so using a mix of ghidra and binary ninja we can see the assembly language, and the decompilied C code

In this C code there is two points of interest we found that look like they will give us the flags

### POI 1



### POI 2


## Cracking POI 1 - Flag 1

The first POI is a simple function that normally never gets called, but we can call it by overwriting the return address of the main function to the address of the read_flag function

however we need to make sure that the 'canary' is not overwritten.

This value is set in the code and is 0xdeadbeef in hex.

After some experementation the following payload sent with pwntools works and does not force us out of the server, but does not get us the flag

```py
payload = b'a' * offset
payload += p32(0x8049855)
payload += p32(0xDEADBEEF)
payload += p32(0x8049855)
```
