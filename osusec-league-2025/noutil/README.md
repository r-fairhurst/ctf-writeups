# noutil
- **Category:** misc
- **Points:** 140 (1 flag)
- **Description:** What bash mysteries lie beyond the binary?
- **Flag 1:** 140 points
- **Access:** ssh noutil@chal.ctf-league.osusec.org -p 1322 (password ahtu8Paghee6hei6)

## First Look
Connecting to the server we get the following message:

```
GNU Coreutils are too bloated.
That's why minimalists use Busybox.
But the TRUE masters don't even need that.

flag.txt is around here somewhere ~_~
but I lost it in this cluttered filesystem :3
can you help me find it? UwU
```

so we know that the flag is in a file named "flag.txt" most likely and we need to find it.

## Solving
trying to use normal commands such as ls or grep or find tells us that they don't exist, and trying to see what commands are available with `help` shows that there are only really builtins avaiable and bash shell scripting commands.

so trying to find out how to list files in a dictory, without 'ls' leads us to echo

using 'echo *' as a command lets us see all the files in the current directory

but there is lots of files to go through and I do not want to do this manually, so after a few more minutes of figuring out how to use echo to list files in other directories, I came up with the following command to find the flag:

### script to find the flag
```bash
search_dir() { 
    for dir in "$1"/*; do 
        if [ -f "$dir/flag.txt" ]; then 
            echo "Found flag.txt at: $dir"
            return 0  # Stop the function and return success
        elif [ -d "$dir" ]; then 
            search_dir "$dir" && return 0  # Stop if found in a subdirectory
        fi
    done
    return 1  # If not found, return failure
}

search_dir "${1:-.}"
```

for copy and pasting aka one line
```bash
search_dir() { for dir in "$1"/*; do [ -f "$dir/flag.txt" ] && echo "Found flag.txt at: $dir" && return 0; [ -d "$dir" ] && search_dir "$dir" && return 0; done; return 1; }; search_dir "${1:-.}"
```

the above script works by using the /* character and adding this to the echo command to list all files in the current directory, then it checks if the file is a directory or a file, if it is a file it checks if the file is flag.txt, if it is it prints the directory and stops the function, if it is a directory it calls itself with the new directory and continues the search.

### getting the flag
so after running the scrip we can see that the flag is in the directory /lib/dpkg/util

trying to use cat shows us it is also not installed so we have to find some other way to read the file

consulting the internet we find that we can use the following command to read a file using echo again.

#### script to read file line by line
```bash
echo "$( < flag.txt )"
```

running this short script command we get the flag:

### flag
osu{!right_where_i_left_it!}

**I do not own nor did I create the content for this challenge. This is a writeup for the challenge that I solved**
