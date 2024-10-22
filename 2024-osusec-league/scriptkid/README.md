# script kid
> I was trying to download more RAM for my computer when I came across this file called not_malware.exe
> Real talk, I kinda believe it cause like... who would lie about that?
> My mom usually tells me if its safe to install new computer programs but she is busy, can you check the file for me?
> If it is malware you should totally hack the hackers back and show them who's boss!
## Solution

### Flag 1:
hidden in a base64 string, and converting it to a string gives you the password to his server, which is the flag. 
```shell
private static string stealerConfig =
"ewogICJjMl9zZXJ2ZXIi0iAic2Z0cDovL3NjcmlwdGtpZC5jdGYtbGVhZ2Uub3N1c2VjLm9yZyIsCiAgInBvcnQiOiAxMzA0LAogICJ1c2VybmFtZSI6ICJyZWFsaXR5c3VyZiIsCiAgInBhc3N3b3JkIjogIm9zdXtkMG50X3N0MHIzX3kwdXJfczNjcjN0c19sMWszX3RoMXN9Igp9ICA=";
```

Converting this base64 string to a string gives us the following:
```shell
{
    "c2_server": "sftp://scriptkid.ctf-league.osusec.org",
    "port": 1304,
    "username": "realitsurf",
    "password": "osu{d0nt_st0r3_y0ur_s3cr3ts_l1k3_th1s}"
}
```
We use this sftp server for the second flag.

As for the first flag, it is used as the password to the sftp server

### Flag 2:
>connecting to SFTP server: "sftp -P 1304 realitysurf@scriptkid.ctf-league.osusec.org" and using the password from the previous flag, we can access the server.

In the SFTP server, there is an "ftp" directory; within it, there is the flag.txt, which you don't have permission to read.

Trying to use cat flag.txt will give you a error

Using help shows what commands we do have access to. One of which is the "get" command

Using...
```shell
get flag.txt
```

...will download the flag.txt file to your local machine in your CWD. Exit the server to invoke cat, or just opening on the file to read the flag:

```shell
osu{I_h4ck3d_th3m_b4ck!}
```

### Flag3: 
From here on out we are trying to save timmy from his greed.

>"This is Timmy's mom. Thank you for saving his computer and PWNing those pesky hackers!
>Since little Timmy can't download RAM he will have to play older games like Roblox.
>I found this "free Robux generator" on his hard drive and I'm almost certain its malware but I can't tell.
>It appears to be obfuscated. Whether thats to hide malware, or to hide a top secret robux generation method, I'm not sure!

>Do me a favor and find out if this is malware. If it is, please delete little Timmy's info from their server!>
>It might be harder than last time since these malware developers seem smarter, but all it takes is for them to make one mistake.

taking a look at his "free Robux generator" we can see that it is a malware program, and it is most likely sending data to a server.

There are a few points in the code that we might be able to gain more information about the malware and the server it is sending data to.

Using the dnspy tool, we can put a breakpoint in the malware to stop running the program at a specific point.

Using this feature we can look at the data being sent to the server, and the server it is being sent to.

Normally this data is encrypted but if we put a breakpoint at where the information is being decrypted before its used to send information to the server, we can see the unencrypted data.

Putting the breakpoint there it will let us see the data, and we can see give the flag being sent to a server at the address: http://scriptkidrevctf-league.osusec.org/credentials

>**The malware creators server... dun dun dun**

<TODO: insert code of where the breakpoint is instered, must do on windows, I am on mac atm>

```shell
osu{wa1t_1m_g0at3d?}
```

### Flag4: 
>Following Timmys Mom's request to delete Timmy's information from the malware creators server...

Going directly to the HTTP address on a browswer lets us see what data the malware creators have gathered.
Here we can search for Timmy's data, and see that he is the very first entry, along with his data releted to his totally real roblox account.

So we need to remove it.

Using the previous HTTP address, we can use the commands:

```shell
curl -X DELETE "http://scriptkidrev.ctf-league.osusec.org/credentials/1"
```

**OR**

```shell
Invoke-WebRequest -Uri "http://scriptkidrev.ctf-league.osusec.org/credentials/1" -Method DELETE
```
> The invoke web request worked for me on windows after curl did not. Not sure why

Using one of these commands to remove timmies data, we fulfill Timmy's Mom's request to delete his information from the "malware" creators servers, giving us the final flag. 

```shell
osu{"status":"osu{I_s4v3d_t1mmy_l0l}"}
```

That is the final flag for this challenge. Using dynspy to find Timmy's server by converting a base64 string, and hacking the hacker, to saving Timmy from his greed making further use of dynspy and its features to discover more information on the malware creators and wher they store the data to do as Timmy's mom requested and remove Timmy's information from their servers.


>** I do not own nor did I create the content for this challenge. This is a writeup for the challenge that I solved **
