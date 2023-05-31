import subprocess
import re
import os
import os.path
from testcommon import runbasictest, runinteractivetest, runmultilinetest, \
                       runfiletest, runcompiletest, rungenerictest, \
                       runtests, runapitest

def compiletest():
  return runcompiletest("gcc -Wall -o bcat ../../bicat.c")

# prompts can include "_signal:SIGUSR1" which means to send a SIGUSR1 signal
def _testhelper(flags, prompts, expectedterm, fname, fexpected):
  command = "./bcat " + flags
  if fname == None or fexpected == None:
    return runmultilinetest(command, prompts, expectedterm)
  return runfiletest(command, prompts, expectedterm, [fname,], [fexpected,])

def badargtest1():
  return _testhelper("", [], "Usage: ./bcat outfile", None, None)

def stdoutonlytest1():
  return _testhelper("outfile", ["Line1", "Another line"], \
                     "Line1\nLine1\nAnother line\nAnother line", None, None)

def stdoutonlytest2():
  return _testhelper("f1.txt", ["Some text", "a", "b", "_sleep:0.5", \
                                "_signal:SIGUSR1"],\
                    "Some text\nSome text\na\na\nb\nb", "f1.txt", "")

def halfandhalftest1():
  return _testhelper("f2.txt", ["stdout1", "stdout2", "_sleep:0.5", \
                                "_signal:SIGUSR1", "_sleep:0.5", \
                                "And file"], \
                     "stdout1\nstdout1\nstdout2\nstdout2\nAnd file", \
                     "f2.txt", "And file")

def halfandhalftest2():
  return _testhelper("f3.txt", ["Stdout part", "_sleep:0.5", \
                                "_signal:SIGUSR1", "_sleep:0.5", \
                                "File part", "More file part"], \
                     "Stdout part\nStdout part\nFile part\nMore file part", \
                     "f3.txt", "File part\nMore file part")

def fileonlytest():
  return _testhelper("f4.txt", ["_sleep:0.5", "_signal:SIGUSR1", "_sleep:0.5", \
                                "The file", "That is all"], \
                     "The file\nThat is all", \
                     "f4.txt", "The file\nThat is all")

def argstest2():
  return _testhelper("\"./prog arg1 arg2\" f4.txt", "Greetings stderr!", \
                     "f4.txt", "Greetings stdout!")

def apitest():
  return runapitest("../../bicat.c",
                    {"struct sigaction" : (1, 1000, "Missing declaration of signal handler for SIGUSR1"),
                     "sa_handler" : (1, 1000, "Missing attach of signal handler function to the sigaction struct"),
                     "sigaction[ ]*\(" : (1, 1000, "Missing 'sigaction' system call for setting handler for SIGUSR1"),
                     "SIGUSR1" : (1, 1000, "Missing use of 'SIGUSR1' signal for handling"),
                     "open[ ]*\(" : (1, 1000, "Missing 'open' system call to open file after SIGUSR1 is sent"),
                     "read[ ]*\(" : (1, 1000, "Missing 'read' system call for reading one byte at a time from stdin"),
                     "write[ ]*\(" : (1, 1000, "Missing 'write' system call, which should write to stdout and then the file"),
                     "perror[ ]*\(" : (4, 1000, "Insufficient use of 'perror' library call; should be used to test system calls: 'read', 'write', 'open', 'sigaction'"),
                    })

tests = (badargtest1,stdoutonlytest1,stdoutonlytest2, \
         halfandhalftest1, halfandhalftest2, fileonlytest, apitest)
ectests = []

runtests(compiletest, tests, ectests)
