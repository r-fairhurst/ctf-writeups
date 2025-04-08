a=__import__
ab=bytes
ac=ab.fromhex
ad=getattr(a('ast'),'literal_eval')
ae=chr

ah={65: 78, 66: 79, 67: 80, 68: 81, 69: 82, 70: 83, 71: 84, 72: 85, 73: 86, 74: 87, 75: 88, 76: 89, 77: 90, 97: 110, 98: 111, 99: 112, 100: 113, 101: 114, 102: 115, 103: 116, 104: 117, 105: 118, 106: 119, 107: 120, 108: 121, 109: 122, 78: 65, 79: 66, 80: 67, 81: 68, 82: 69, 83: 70, 84: 71, 85: 72, 86: 73, 87: 74, 88: 75, 89: 76, 90: 77, 110: 97, 111: 98, 112: 99, 113: 100, 114: 101, 115: 102, 116: 103, 117: 104, 118: 105, 119: 106, 120: 107, 121: 108, 122: 109}

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

hex_string = '674141414141426e775044313253586f7577644d50384674735a65795472494d50516a727546694d666c72753675566a35355063762d346e347573625348436454436a5465436b636734306c55506f477238663135616d4149704b6537786d66334f636c6d6f32536e6362386f6b457444635449386c47572d706666393442647779334e4436634846584259'

ar = lambda secret: str(ak.unpack('>I', ap.new(al.b32decode(secret), ak.pack('>Q', int(1740697845.986939//30)), aj.sha1).digest()[-1 & 0x0F:][:4])[0] & 0x7FFFFFFF)[-6:].zfill(6)

at = lambda f: open(f,'rb').read()
au = lambda d: aq(av(ar(aw))).encrypt(d)
av = lambda t: (am.seed(t),bytes(al.urlsafe_b64encode(am.getrandbits(256).to_bytes(32,byteorder='big')).decode(),'utf-8'))[1]

aw='VEQKJGGCBECIT327XAFJT4GSBECITMYQBWFKV7V='

print(au(ac(hex_string).decode()))



aw='VEQKJGGCBECIT327XAFJT4GSBECITMYQBWFKV7V='