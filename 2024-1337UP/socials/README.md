# Socials
> Have a look around our socials, maybe you'll find some flags! Don't forget to hit follow while you're there 🥺
> Flag format: INTIGRITI{Twitter+YouTube+Reddit}
> https://linktr.ee/hackwithintigriti 

## Solution

### Twitter
using the linktree and going to their twitter page, we can see a few recent tweets.
One of these tweets on the day the event launched, there is a reply with binary in it.

```shell
0110100000110000011100000011001101011111011110010011000001110101
```

this binary translates to: 

```shell
h0p3_y0u
```

which is the first part of the flag.

### YouTube
Next is youtube, following a similar format to twitter find a recent post, which was a live stream, look at the comments and bingo

```shell
5f336e6a30795f
```

this is in hex, and translating it to ascii is:

```shell
_3nj0y_
```

there is the second part of the flag.

### Reddit
Finally, we have reddit same deal, find a recent post and look at the comments, and bingo

```shell
ZDRfYzdm
```

this is in base64, and translating it to ascii is:

```shell
d4_c7f
```

there is the third part of the flag.

## Flag:
```INTIGRITI{h0p3_y0u_3nj0y_d4_c7f}```