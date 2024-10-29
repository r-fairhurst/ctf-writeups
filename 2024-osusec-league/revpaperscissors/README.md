# revpaperscissors
>rock paper scissors is a lot easier if ur opponent lets you read their mind before u start playing

## Solution
accessing the server lets us run either challenge 1 or 2, corresponding to each challenge

### Flag 1
runing challenge one lets us enter in a name, and then 3 options for rock, paper, and scissors, which is 0, 1, 2 respectively

Unsure of where to go next besides trying to brute force through the program, we open up ghidra to look at the decompiled code

Here we notice in the main program a set of 5 functions calls, each has a input of either rock paper or scissors and after those functions, it will give us the flag

```c
int main ()
...
    throw_hands("rock");
    throw_hands("scissors");
    throw_hands("paper");
    throw_hands("paper");
    throw_hands("scissors");
    printf("Wow, good job! Here\'s your flag: %s\n",flag);
...
```

looking at the function "throw_hands" we can see that it takes the string parameter and compares it to a choice we make, of 0, 1, or 2. 

So maybe if we just enter in the correpsonding numbers we see in the main function, it will give us the flag?

trying it out we enter our name, and then 0, 2, 1, 1, 2 in that order and it gives us the flag:

```shell
osu{n3x7_13v31}
```
And onto flag 2!

### Flag 2

unreadable:
```c
void make_moves(char *in,int *out)
{
  int *out_local;
  char *in_local;
  int delta;
  int i;
  
  delta = 0;
  for (i = 0; i < 10; i = i + 1) {
    in[i] = in[i] + (char)delta;
    out[i] = (int)(in[i] % '\x04');
    delta = (in[i] + 0x1ca3) % 0x15;
  }
  return;
}
```


readable:
```c
void make_moves(char *in,int *out)
{
  int *out_local;
  char *in_local;
  int delta;
  int i;
  
  delta = 0;
  for (i = 0; i < 10; i = i + 1) {
    in[i] = in[i] + (char)delta;
    out[i] = (int)(in[i] % 4);
    delta = (in[i] + 7331) % 21;
  }
  return;
}
```

"make_moves" only decides what the program uses as input for the "throw_hands" function, so we can just use the same inputs as before to keep things simple

```c
throw_hands(...)
...
  if (((((u != 0) || (choice != 1)) && ((u != 1 || (choice != 2)))) && ((u != 2 || (choice != 0))))
     && (choice != u)) {
    puts("You won this round!");
    if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
                    // WARNING: Subroutine does not return
      __stack_chk_fail();
    }
    return 1;
  }
  puts("Womp womp, I win!");
...
```

In this function the only part we care about is when it compares the user input, "u", to the choice from "makes_moves", the logic was a bit complex but I believe I got it right and figured out what input to use based on the choice. 

Since I did not want to do math and logic manually to figure out the corresponding choices based on the users name, and then go through the logic in "throw_hands" to get the actually correct inputs, I decided to write a short C program to do it for me

for example: 

output from make_moves func: 1 0 2 0 3 1 3 1 0 2
input needed for aaaaaaaaaa: 2 1 0 1 0 2 1 2 1 0


```shell
osu{74k3_m3_70_y0u2_134d3r}
```
