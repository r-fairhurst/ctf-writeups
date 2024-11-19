# Secure Bank
> Can you crack the bank?
> nc securebank.ctf.intigriti.io 1335 

## Solution

Accessing the server first, it asks us for a “superadmin pin“

Not knowing what it is I decompile the executable and see that it is hard coded into the file

It is the hex value of 0x539 or 1337 in decimal

Using the pin to enter into the server, it asks for a 2fa code, which appears to also be hardcoded, but it is obfuscated to make it hard to read

The obfuscation happens in the code below and in obsure_key which again just uses the admin pin to obfuscate the code

```c
uint generate_2fa_code(int param_1) {
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
 
  local_10 = param_1 * 0xbeef;
  local_c = local_10;
  for (local_14 = 0; local_14 < 10; local_14 = local_14 + 1) {
	local_c = obscure_key(local_c);
	local_10 = ((local_10 ^ local_c) << 5 | (local_10 ^ local_c) >> 0x1b) +
           	(local_c << ((char)local_14 + (char)(local_14 / 7) * -7 & 0x1fU) ^
           	local_c >> ((char)local_14 + (char)(local_14 / 5) * -5 & 0x1fU));
  }
  return local_10 & 0xffffff;
}
```

Essentially what it does is not important, I took the code, put it in my own C file (which can be seen in 2fa-grabber.c), and printed the result in decimal, giving me: 

```shell
5670688
```

Then I connected to the server again, inputted the admin pin, and then the 2fa code and bang, flag.
    
```shell
INTIGRITI{pfff7_wh47_2f4?!}
```
