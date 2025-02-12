#!/usr/bin/env python3
from pwn import *
from sympy import nextprime

host = "chal.ctf-league.osusec.org"
port = 1316

def is_divisible(num, divisor):
  return num % divisor == 0


def gen_primes():
    start = 2**63
    end = 2**64

    p = nextprime(start-1)
    q = nextprime(p)

    while not is_divisible((p-1)*(q-1), 65537):
        q = nextprime(q)

    print(p, q)
    return p, q

def exploit():
    primes = []
    primes = gen_primes()
    p = remote(host, port)

    p.recvuntil("Input p: ")

    p.sendline(str(primes[0]))

    p.recvuntil("Input q: ")

    p.sendline(str(primes[1]))

    p.interactive()


if __name__ == "__main__":
    exploit()

