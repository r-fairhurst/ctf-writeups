# revpaperscissors
>rock paper scissors is a lot easier if ur opponent lets you read their mind before u start playing

## Solution
accessing the server lets us run either challenge 1 or 2, corresponding to each challenge

### Flag 1
runing challenge one lets us enter in a name, and then 3 options for rock, paper, and scissors, which is 0, 1, 2 respectively

Unsure of where to go next besides trying to brute force through the program, we open up ghidra to look at the decompiled code

Here we noticed in the main program a set of 5 functions calls, each has a input of either rock paper or scissors and after those functions, it will give us the flag

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

Similar to challenge 1 we have to beat the computer in rock paper scissors, but this time we have to beat it 10 times in a row

Looking at the source code there is no hard coded flag, so we have to figure out how to beat the computer 10 times in a row

In the code it seems to user a function called "make_moves" to decide what the computer will throw, and then compares it to the user input

>here is the decompiled code for "make_moves"

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
In this state with the hex values it is hard to read, So lets make it more readable

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

There! Much better, now we can see the math a bit easier

It seeems to take a string, and then for each position in the string (assumming it is at least 10 characters long) will do math and store the resulting int into the out array

"make_moves" only decides what the program uses as input for the "throw_hands" function, so we can just use the same inputs as before to keep things simple

Then after the make_moves function is called, it will call the "throw_hands" function to compare the user input to the choice from "make_moves"

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

This function is also a bit hard to read due to the logic, and the fact that the make_moves function can have 4 different values in the array, instead of 3

In this function the only part we really care about is when it compares the user input, "u", to the choice from "makes_moves", the logic was a bit complex but I believe I got it right and figured out what input to use based on the choice. 

Since I did not want to do math and logic manually to figure out the corresponding choices based on the users name, and then go through the logic in "throw_hands" to get the actually correct inputs, I decided to write a short C program to do it for me

for example: inputting bbbbbbbb into my c progame gives you this as a result: 

```shell
Enter a string at least 10 characters long: bbbbbbbbbbbbbbbbb
Calculating outputs for make_moves: 
 2   2   1   0   3   3   2   1   0   0  
Calculating corresponding inputs... enter in: 
 0   0   2   1   0   0   0   2   1   1 
```

Testing this on the program by inputting the corresponding input from my program to the server, it gives me the flag:

```shell
osu{74k3_m3_70_y0u2_134d3r}
```

>** I do not own nor did I create the content for this challenge. This is a writeup for the challenge that I solved **
