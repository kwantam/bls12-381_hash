import hashlib
import struct
import textwrap

DEBUG = False

map2curve_alphas = [
    "",
    "\x00",
    "\xFF",
    "\xFF\x00"
    "\x11\x22\x33\x44\x11\x22\x33\x44\x11\x22\x33\x44\x11\x22\x33\x44\x55\x66\x77\x88\x55\x66\x77\x88\x55\x66\x77\x88\x55\x66\x77\x88",
]

# Convert an non-negative integer into n bytes
def i2osp(i, n):
    i = int(i)
    res = [0]*n
    for idx in range(n-1, -1, -1):
        res[idx] = i & 0xff
        i = i >> 8
    if i > 0:
        raise ValueError("Integer %s cannot fit into %s bytes" % (i, n))
    return struct.pack("<" + "B" * n, *res)

# Convert octal string into to integer
def os2ip(os):
    res = 0
    for (idx, b) in enumerate(struct.unpack("<" + "B" * len(os), os)):
        res = res << 8
        res += b
    return res

def Hex(n):
    return format(int(n),'x')

def pprint_hex(os):
    return "".join("{:02x}".format(ord(c)) for c in os)

def from_hex_string(str):
    os = "".join(chr(int(b, 16)) for b in str.split(" "))
    return os2ip(os)

def enable_debug():
    global DEBUG
    DEBUG = True

def tv_wrap(text):
    return textwrap.fill(text, 54).split("\n")

def tv_text(label, value):
    if DEBUG:
        chunks = tv_wrap(value)
        print("%7s = %s" % (label, chunks[0]))
        for i, v in enumerate(chunks[1:]):
            print((" " * 10) + v)

def tv(label, v, len):
    if DEBUG:
        chunks = tv_wrap(pprint_hex(i2osp(v, len)))
        print("%7s = %s" % (label, chunks[0]))
        for i, v in enumerate(chunks[1:]):
            print((" " * 10) + v)

class Ciphersuite:
    def __init__(self, label):
        (h2c_prefix, curve, hash_name, map_name, variant_name) = label.split("-")
        self.curve = Curve(curve)
        self.hash = Hash(hash_name)

class Hash:
    def __init__(self, label):
        if label == "SHA256":
            self.H = hashlib.sha256
        elif label == "SHA384":
            self.H = hashlib.sha384
        elif label == "SHA512":
            self.H = hashlib.sha512
        else:
            # RSW: fixed the below; it used to refer to the undefined variable "curve"
            raise ValueError("Hash %s is not recognized" % label)

    def hbits(self):
        return self.H().digest_size * 8

class Curve:
    def __init__(self, label):
        if label == "Curve25519":
            self.p = 2**255 - 19
        elif label == "P256":
            self.p = 2**256 - 2**224 + 2**192 + 2**96 - 1
        elif label == "P384":
            self.p = 2**384 - 2**128  - 2**96 + 2**32 - 1
        elif label == "P503":
            self.p = 2**250*3**159-1
        elif label == "BN256":
            mu = -(2**62 + 2**55 + 1)
            pp = lambda x: 36*x**4 + 36*x**3 + 24*x**2 + 6*x + 1
            self.p = pp(mu)
        # RSW: added the following line
        elif label == "BLS12_381_1" or label == "BLS12_381_2":
            self.p = 0x1a0111ea397fe69a4b1ba7b6434bacd764774b84f38512bf6730d2a0f6b0f6241eabfffeb153ffffb9feffffffffaaab
        else:
            # RSW: fixed the below; it used to refer to the undefined variable "curve"
            raise ValueError("Curve %s is not recognized" % label)
