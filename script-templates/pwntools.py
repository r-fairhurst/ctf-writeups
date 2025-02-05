#!/usr/bin/env python3
from pwn import *

target_os = 'linux'
target_arch = 'amd64'
host = 'ip'
port = 1337
executable = './vulnerable_binary'

# Set up pwntools context
context(os=target_os, arch=target_arch)
# Define variables for the target binary and remote server

def exploit():
    io = remote(host, port)  # Connect to the remote server
    # io = process(executable)  # Run the binary locally

    io.recvuntil('')

    payload = b'A' * 64  # Example payload
    payload += p64(0xaaaaaaaa)  # Example payload

    # Send the payload
    io.sendline(payload)

    # Interact with the process
    io.interactive()

if __name__ == '__main__':
    exploit()