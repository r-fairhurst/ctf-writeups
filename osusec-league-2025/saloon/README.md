## Saloon 
**Category:** pwn **Points:** 100 (2 flags) 

**Description:** Can our indefatigable protagonist pass all the challenges required of the patrons of the robot saloon? Maybe!

- Flag 1: 50 points
    - Access: nc chal.ctf-league.osusec.org 1308
- Flag 2: 50 points
    - Access: nc chal.ctf-league.osusec.org 1308


## First access

First time accessing the server

prints out ascii art and asks if you want to continue or not, y/n for the options

n kicks you out
y shows you math and 1/100 

trying to input anything else will kick you out as well

the program kicks you out at some automated point.

with y as the input it shows you a math equation and 1/100, then asks for the answer

assuming we need to get all 100 answers correct to get the flag

trying to input anything other than an int or the wanted answer breaks it, so no input or buffer overflow we can take advantage of (that we know of)

## Writing script for all 100 math answers

The math seems to be all multiplication with some quick math before it kicks us out, so it saves us for figuring out what math to do

using pwntools makes this super easy

wrote a small script to get up until the point where it just shows us hte math equation.

then started on the process of just reading the line and converting it to an int and sending hte answer back

```python
from pwn import *

p = remote("saloon.ctf-league.osusec.org", 1308)

p.send(b"y\n")
p.recvuntil(b"math\n")

for i in range(100):
    line = p.recvline()
    lineSplit = line.split()
    number = int(lineSplit[0]) * int(lineSplit[2])
    print(number)
    p.sendline(str(number).encode("utf-8"))
```

> this can also be found in the script.py file, this is just now uncommented as I also used this to get the second flag

I set up a for loop and successfully got all 100 answers correct and the server then prints out a ton of binary data/hex/asci, its to much to be just a simple conversion and at the start of it, it has the hex artifact/magic bytes of a jpg file

So I am assuming it is a jpg file being written directly to the ternimal

## Getting the picture from the server

There is some ascii text the server prints out before the data, which we don't care for, so we recieve until we the asii text ends, and the data starts to show

looking up for a python library that will write binary data to a file, there is one called `binary_file` that will do exactly what we want

```shell
p.recvuntil(b"go!\n")

line = p.recvall()

binary_file = open("my_file.txt", "wb")

binary_file.write(line)
 
binary_file.close()

p.interactive() 
```

this successfully writes the data to a file, renaming it to my_file.jpg and opening it, shows us the flag as text on the picture

### Flag 1

```shell
osu{sh4rpsh00t3r}
```

## Finding the second flag

after the math questions are answered, and the pictures data is printed, the server asks for the flag next

for me specifically, entering in the flag was not working at first but if I hit enter it did

I realized i was not doing p.sendline() and was only doing p.send() thus it was waiting for input to go on

once that was fixed, the server then does something similar to the first flag where it shows some ascii text we can ignore, 

and then again asks for 100 questions to solve, but this time we convert a hex base 10 number to int and resumit it back to the server

```python
for i in range(100):
    line = p.recvline()
    lineSplit = line.split()
    number = int(lineSplit[0], 16)
    print(number)
    p.sendline(str(number).encode("utf-8"))

p.interactive()
```

the code above let us sumbit all 100 answers and see what was next to do

the server then tells us this:

```shell
Here's an OTP (one-time-pad) key:
v\x01\xfe\x88\xce̫x\x96\x8b\x0b=ϼ\xffMrPo\xaf
and here's a secret message!
\x19r\x8b\xf3\xbc\xa4\xd2\x15\xff\xe5lb\xa6Ϡ%\x13"\x0b\xd2
goodbye!
```

## Decrypting the message

the server gives us a one time pad messsage and the key to decrypt it

I have a script that can do this exact thing from a previous ctf, but I was to lazy to pull it out and use it

so I tried to convert it with chatGPT and it actually worked

### Flag 2

```shell
osu{rhyming_is_hard}
```

and that is our last flag

## Conclusion

pwntools is super useful and makes an easy and fast way to interact with a server, which I will definitely be using in the future

I cut out a lot of my learning of pwntools and the process for that, since that is what took the longest, the scripts is the result of my learning and some help from Club officers and teammates

** I do not own nor did I create the content for this challenge. This is a writeup for the challenge that I solved **
