import os
import subprocess
import ctypes 

support = ctypes.CDLL(os.path.abspath("support.so"))
support.ASCIIToDecimal.argtypes = [ctypes.c_char_p]
support.ASCIIToDecimal.restype = ctypes.c_char_p

x = input("Sender: ")

ret = support.ASCIIToDecimal(str.encode(x))
dec = ret.decode("utf-8", errors="ignore")
subprocess.check_call(["./client -D 127.0.0.1 -P 2222 -u '%s' " % dec],shell=True)


