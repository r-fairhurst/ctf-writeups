# Bug Squash 1
> Squash those bugs!

## Solution

First just started playing the game

You can control a cat on a 2d plane and “hit” bugs for points to increase your score

Opening up cheat engine to mess around with the score I find out what addresses are asociated with the score and changing their value, then squashing a bug updates the score to the value we want

Next I check for what functions or processes interacts with the score and find that the score is checked in the assembly code

Looking at these functions in the assembly code there is a cmp that checks the score value to a value of -1337

> maybe if we change the score to that it will give us the flag?

Attempting to set the values of the addresses of the score to -1338 so that when we squish a bug it will update to -1337 (since it adds one to the score)

After this is done, our score is -1337 and the game shows us the flag:

```shell
INTIGRITI{game_hacking_is_fun!}
```
