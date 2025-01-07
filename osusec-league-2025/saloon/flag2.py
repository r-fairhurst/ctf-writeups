from pwn import *
p = remote("chal.ctf-league.osusec.org", 1308)

p.send(b"y\n")
p.recvuntil(b"math\n")

for i in range(100):
    line = p.recvline()
    lineSplit = line.split()
    number = int(lineSplit[0]) * int(lineSplit[2])
    print(number)
    p.sendline(str(number).encode("utf-8"))

#p.recvuntil(b"go!\n")

#line = p.recvall()

#binary_file = open("flag1", "wb")

#binary_file.write(line)
 
#binary_file.close()

p.recvuntil(b"1)")

p.sendline(b"osu{sh4rpsh00t3r}")

p.interactive()

p.recvuntil(b"bar\n")

for i in range(100):
    line = p.recvline()
    lineSplit = line.split()
    number = int(lineSplit[0], 16)
    print(number)
    p.sendline(str(number).encode("utf-8"))

p.interactive()