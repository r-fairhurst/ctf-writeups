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

p.recvuntil(b"go!\n")

line = p.recvall()

binary_file = open("flag1", "wb")

binary_file.write(line)
 
binary_file.close()

p.interactive() 
