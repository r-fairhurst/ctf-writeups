# In Plain Sight
> Barely hidden tbh..

## Solution
we are given a file called meow.jpg

using srch_strings to see if there is anything hidden in the file we see a flag.pngUT text in the file.

```shell
flag.pngUT
```

this might be a file hidden in the file, attempint to unzip it, tells us this:

```shell
$ unzip meow.jpg
Archive:  meow.jpg
warning [meow.jpg]:  2144878 extra bytes at beginning or within zipfile
```

so we know ther is a file hidden in the file, but we need to find a way to extract it.

using dd to extract the file from the jpg, we can see the flag.png file.

```shell
$ dd if=meow.jpg of=flag.png skip=2144878 bs=1
```

this splits the file into a new file called flag.png

unzip this file we get a picture we can view.

In that picture is the flag, it is extremely hard to see, but it is there.

## Flag:
```shell
NTIGRITI{w4rmup_f|46z}
```
