# script kid
> I was trying to download more RAM for my computer when I came across this file called not_malware.exe
> Real talk, I kinda believe it cause like... who would lie about that?
> My mom usually tells me if its safe to install new computer programs but she is busy, can you check the file for me?
> If it is malware you should totally hack the hackers back and show them who's boss!
#Solution

## Flag 1:
hidden in a base64 string, and converting it to a string gives you the password to his server, which is the flag. 
## Flag 2:
in the SFTP server, there is an FTP file; within it, there is the flag.txt, which you don't have permission to read.
Using help shows we have gotten working, so doing get flag.txt downloads the flag onto our system to read it.

## Flag3: 
Using dnspy and putting a breakpoint in the malware to stop running the program, letting us access the encrypted data, Unencrypted, will give us a flag and URL the data is being sent to: http://scriptkidrev.ctf-league.osusec.org/credentials

## Flag4: using the previous HTTP address, we can use the commands:

```shell
curl -X DELETE "http://scriptkidrev.ctf-league.osusec.org/credentials/1"
```
OR

```shell
Invoke-WebRequest -Uri "http://scriptkidrev.ctf-league.osusec.org/credentials/1" -Method DELETE
```
> The invoke web request worked for me. Not sure why

Using these commands, we fulfill Timmy's Mom's request to delete his information from the "malware" creators servers, giving us the final flag. 
