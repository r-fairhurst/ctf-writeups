# BabyFlow
> Does this login application even work?!
> nc babyflow.ctf.intigriti.io 1331 

## Solution

we are given the "source code" binary for this challenge.

connecting to the server we are greeted with a login prompt.

checking the binary in ghidra we can see that the binary is a simple login application.

```c
int iVar1;
  char local_38 [44];
  int local_c;
  
  local_c = 0;
  printf("Enter password: ");
  fgets(local_38,0x32,stdin);
  iVar1 = strncmp(local_38,"SuPeRsEcUrEPaSsWoRd123",0x16);
  if (iVar1 == 0) {
    puts("Correct Password!");
    if (local_c == 0) {
      puts("Are you sure you are admin? o.O");
    }
    else {
      puts("INTIGRITI{the_flag_is_different_on_remote}");
    }
  }
  else {
    puts("Incorrect Password!");
  }
  return 0;
```

here we can see that we need 'local_c' t obe anything other than 0 to get the flag.

there is not input sanitization so we can just input anything and get the flag.

the order of variable declaration goes from local_38 to local_c so we can just input a bunch of characters and then a number to get the flag.

> the minimum number of chars to enter is 44,

entering really anything but 0's will get you the flag

```shell
INTIGRITI{b4bypwn_9cdfb439c7876e703e307864c9167a15}
```
