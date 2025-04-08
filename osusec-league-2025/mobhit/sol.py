a=__import__

import shutil as ai
import hashlib as aj
import struct as ak
import base64 as al
import random as am
import socket as an
import time as ao
import hmac as ap
from cryptography.fernet import Fernet as aq

ar = lambda secret: str(ak.unpack('>I', ap.new(al.b32decode(secret), ak.pack('>Q', int(ao.time()//30)), aj.sha1).digest()[-1 & 0x0F:][:4])[0] & 0x7FFFFFFF)[-6:].zfill(6)

at = lambda f: open(f,'rb').read()
au = lambda d: aq(av(ar(aw))).encrypt(d)
av = lambda t: (am.seed(t),bytes(al.urlsafe_b64encode(am.getrandbits(256).to_bytes(32,byteorder='big')).decode(),'utf-8'))[1]

aw='VEQKJGGCBECIT327XAFJT4GSBECITMYQBWFKV7V='

