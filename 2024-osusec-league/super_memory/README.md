# super_memory
> Description: Welcome to super memory world! each level has two flags, one for normies and one the leet... can you beat them all?

## Solution

### Flag 1:

Running the program gives us a small platform terminal game and after exploring to the end we find two flags, one is accessible but is not the flag we want.
The other is inaccessible but has the flag we want.

After looking at the source code for the program, we can see there is in the function "handleMovement" there are two things it checks for using our current position.
The first is for the letter 'f' where if our position matches the same position as this letter it will print out a statement to us.
The second is for the number '1' where in this case if our player goes over it calls a different function "read_flag" this is the one we want to get and is the flag we cannot access.

```c
char *handleMovement(...)
   ...
       witch (level[*ypos][*xpos]) {
       case ' ':
           break;
       case 'f':
           return "Does this feel like the end?\n\Are you proud of yourself?\n\
                   Question: Should Magellan have been proud of himself had he turned around after leaving the harbor?\n\
                   Answer: No.";
           break;
       case '1':
           read_flag();
           break:
   ...

```

Since we cannot access the flag '1' by normal means we have to find an exploit in this program.

I first looked closer at the "handleMovement" function and noticed that when moving left or right the program never checks to see if the player will still move inbounds of the level width.
It only subtracts from and adds to the x position

```c
char *handleMovement(...)
   ...
   if (input == 'd')
       (*xpos) ++;
   if (input == 'a')
       (*xpos) --;
   ...
```

Then knowing that we can look at the function "renderLevel"
Here I noticed that the variables dealing with rendering of the width of the level is unsigned, so it always be positive.
That meant that if one were to try to go to the left far enough to try and make the xposition negative it might loop around and put the player on the opposite side.

```c
void renderLevel(... unsigned char xpos, unsigned char ypos...) {
   unsigned char xRange[2] = {xpos - viewW/2, xpos + viewW/2};
   unsigned char yRange[2] = {ypos - viewH/2, ypos + viewH/2};

   if (viewW / 2 > xpos) {
       xRange[0] = 0;
       xRange[1] = viewW;
   }
   if (255 - viewW / 2 < xpos) {
       xRange[0] = 255-viewW;
       xRange[1] = (char)255;
   }
   ...
```

Attempting this strategy you need to go to the right in the level for a bit and climb up the tower to the second highest point, jump as high as you can, then go the left until you land on the "clouds"
Then going on the clouds to the left you eventually are able to reach the "boundary" of the level and trying to go left into it...teleports you to the far right side of the map, letting you access the flag '1'

And there is our flag!

```shell
osu{4r0und_th3_w0r1d}
```

### Flag 2

> this is similar to flag one where you play a platform terminal game trying to gain access to the flag.


In this level, level2, there are doorways that are locked unless we pick up the corresponding keys. There are a total of 4 doors to be unlocked, A, B, C, D with the corresponding keys a, b, c, d.
gathering the keys and going through the doors lets you gain access to the fake flag 'f' relatively quickly, but similar to the first the real flag we want is '1' which this time is blocked by a door '[' that tells us we need $1650549605 dollars to pass through it.

Also in this level is a few dollars, these are scattered throughout the level and only has a total of 4 dollars to collect.

So how do we get the flag?

There is no way to loop around the level or access the flag through the previous glitch, but what is different is how the program stores information.

All data related to the player is stored in a struct "PlayerInfo"

```c
struct PlayerInfo {
   int jumpCount;
   int collected_items;
   unsigned char xpos;
   unsigned char ypos;
   char inventory[4];
   uint dollars;
};
```

I then was stuck until I looked at the code for how it handles picking up items, and how if you spam picking up the keys more than the 4 slots available, your money will go up.

For example, trying to pick up the key 'a' at the start fills up all your key slots, and once filled, after two more presses your money starts to increase on the third.
Spamming this with 'a' got me to 1633771873 dollars, which is not enough to pass through, and so i tried again with 'b' and it got me 1650549605 dollars, which should me more than enough for the entry cost of 1650549605.


Now with all the money in the world attempting to pass the door '[' to get to the flag it tells you, your excess wealth is disgusting.


This puzzled me, since I had more than enough money to pass. But I checked the source code and saw this:

```c
char* paid_unlock(char key, struct PlayerInfo *player_info) {
   if (blocks[key].solid == 1) {
       if (player_info->dollars == 1650549605) {
           player_info->dollars = 0;
           blocks[key].solid = 0;
           return "Unlocked!";
       }
       else if (player_info->dollars > 1650549605) {
           return "Your excessive wealth disgusts me.";
       }
       else {
           return "Sorry! You need $1650549605 to unlock me!";
       }
   }
   return "";
};
```

The program requires EXACTLY 1650549605 dollars to unlock it and move past it.


But how do we get that exact amount? Brute force it and try multiple combinations of keys to maybe find the correct one?


No! So I experimented a bit more, and noticed that picking up just one extra 'a' key got me exactly 97 dollars. Curious...97 is the decimal value of the letter 'a'...
Picking up another 'a' gives us 24929 dollars which is 0x6161 in hex, and 61 in hex is 'a' so it corresponds to "aa"

Trying to convert the exact number of 1650549605 to hex gets us 0x62616365 which breaking them up into two bytes each gives us b a c e as the ascii values.
But there is not one 'e' on the level to pick up or interact with. we could get 0x62616364 with b a c d but not 65, So I wondered if all we needed to do was pick up a dollar after that to get it to 65.


Attempting it by first picking up all the keys normally but skipping a dollar by the key 'b' the picking up the keys in the order 'b a c d' then the dollar we skipped by b
netted us with 1684234595 dollars, not 1650549605 like we expected. But 168423495 did correspond to 0x64636163, which is d c a c in ascii. Almost the reverse order of what I attempted...

After being puzzled for a bit we discovered that this was most likely due to the data being in little endian, meaning it is the reverse of what we expected for the order.

Trying again, but this time with the order of 'd c a b' gets you 1659549604 and the extra dollar you leave behind at b gets you to exactly 1650549605 dollars, the exact amount needed to go through the door and get the final flag.

```shell
osu{g1v3_m3_4_d0114r}
```

The reason this exploit works is due to c's continuous memory, and in the struct mentioned above, the order of the memory would go from inventory of keys -> dollars, so if there was a bug in the code that let you add data onto the array past the 4 slots, it would then try and add it to the unsigned int bytes instead.

Which after looking at the code for handling the pickup of items, there is no checks for if the collected items, which is the int used to keep track of the amount of items collected, goes greater than 3 which would be the max size of the array.

```c
if (input == 's' && current_block_properties.mustInteract) {
       if (current_block_properties.specialProperty) {
           message = (*current_block_properties.specialProperty)(current_level_location, player_info);
       }
       if (current_block_properties.collectable) {
           player_info->inventory[player_info->collected_items] = current_level_location;
           player_info->collected_items ++;
       }
       if (current_block_properties.disappears) {
           current_level_location = ' ';
       }
   }
```

This let us keep trying to add the item to our inventory and caused memory overflow into the dollars variable in the structure.

Very cool ctf 