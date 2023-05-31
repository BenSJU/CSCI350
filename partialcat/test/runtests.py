import subprocess
import re
from testcommon import runbasictest, runinteractivetest, runmultilinetest, \
                       runcompiletest, rungenerictest, runapitest, runtests

def compiletest():
  return runcompiletest("gcc -Wall -o partcat ../../pcat.c")

def _testhelper(flags, expected):
  command = "./partcat " + flags
  return runbasictest(command, expected)

def badargtest1():
  return _testhelper("", "Usage: ./partcat file startbyte numbytes")

def badargtest2():
  return _testhelper("../../thisfile.txt 10", "Usage: ./partcat file startbyte numbytes")

def nobytestest():
  return _testhelper("../../thisfile.txt 40 0", "")

def smalltest1():
  return _testhelper("../../thisfile.txt 0 4", "ABCD")

def smalltest2():
  return _testhelper("../../thisfile.txt 10 4", "KLMN")

def pastendtest():
  return _testhelper("../../thisfile.txt 200 20", "stuvwxyz\r\n\r\n9")

def bigtest():
  return _testhelper("../../thisfile.txt 11 130", \
"""LMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVW\r
XYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghi\r
jk""")

def apitest():
  return runapitest("../../pcat.c",
                    {"fopen[ ]*\(" : (0, 0, "Use of 'fopen' library function is not allowed; use the 'open' system call"),
                     "fread[ ]*\(" : (0, 0, "Use of 'fread' library function is not allowed; use the 'read' system call"),
                     "fwrite[ ]*\(" : (0, 0, "Use of 'fwrite' library function is not allowed; use the 'write' system call"),
                     "close[ ]*\(" : (1, 1000, "Missing file close using 'close' system call"),
                     "read[ ]*\(" : (1, 1000, "Improper or missing use of 'read' system call"),
                     "write[ ]*\(" : (1, 1000, "Improper or missing use of 'write' system call"),
                     "lseek[ ]*\(" : (1, 1000, "Improper or missing use of 'lseek' system call"),
                     "perror[ ]*\(" : (5, 1000, "Insufficient use of 'perror' system call; should be used to test system calls: 'open', 'lseek', 'read', 'write', 'close'"),
                     "EINTR" : (4, 1000, "Insufficient checking for EINTR errno; should be tested for system calls: 'open', 'read', 'write', 'close'"),
                    })

tests = (badargtest1, badargtest2, nobytestest, smalltest1, smalltest2, \
         pastendtest, bigtest, apitest)
ectests = []

runtests(compiletest, tests, ectests)
