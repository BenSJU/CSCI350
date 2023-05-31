import subprocess
import re
import os
from testcommon import runbasictest, runinteractivetest, runmultilinetest, \
                       runfiletest, runcompiletest, rungenerictest, \
                       runtests, runapitest

def compiletest1():
  return runcompiletest("gcc -Wall -o ocap ../../outputcapture.c")

def compiletest2():
  return runcompiletest("gcc -Wall -o prog ../prog.c")

def _testhelper(flags, expectedterm, fname, fexpected):
  command = "./ocap " + flags
  if fname == None or fexpected == None:
    return runmultilinetest(command, [], expectedterm)
  return runfiletest(command, [], expectedterm, [fname,], [fexpected,])

def badargtest():
  return _testhelper("", "Usage: ./ocap cmd outfile", None, None)

def lstest():
  return _testhelper("ls f1.txt", "", "f1.txt", "f1.txt\nocap\nprog")

def badcmdtest():
  return _testhelper("notreal out.txt", "", "out.txt", "")

def progtest():
  return _testhelper("./prog f2.txt", "Greetings stderr!", \
                     "f2.txt", "Greetings stdout!")

def argstest1():
  return _testhelper("\"echo hello\" f3.txt", "", "f3.txt", "hello")

def argstest2():
  return _testhelper("\"./prog arg1 arg2\" f4.txt", "Greetings stderr!", \
                     "f4.txt", "Greetings stdout!")

def apitest():
  return runapitest("../../outputcapture.c",
                    {"fopen[ ]*\(" : (0, 0, "Use of 'fopen' library function is not allowed; use the 'open' system call"),
                     "read[ ]*\(" : (0, 0, "Use of 'read' system call should not be necessary here; use 'dup2' instead"),
                     "write[ ]*\(" : (0, 0, "Use of 'write' system call should not be necessary here; use 'dup2' instead"),
                     "fork[ ]*\(" : (1, 1000, "Missing child process creation via the 'fork' system call"),
                     "wait[ ]*\(" : (1, 1000, "Missing 'wait' system call for parent process"),
                     "open[ ]*\(" : (1, 1000, "Missing file open using 'open' system call"),
                     "O_WRONLY" : (1, 1000, "File should be opened in write-only mode (O_WRONLY)"),
                     "O_CREAT" : (1, 1000, "File should be created if not existing on open (O_CREAT)"),
                     "O_TRUNC" : (1, 1000, "File should be truncated when opened (O_TRUNC)"),
                     "close[ ]*\(" : (1, 1000, "Missing file close using 'close' system call"),
                     "dup2[ ]*\(" : (1, 1000, "Improper or missing use of 'dup2' system call"),
                     "execvp[ ]*\(" : (1, 1000, "Improper or missing use of 'execvp' system call wrapper"),
                     "perror[ ]*\(" : (5, 1000, "Insufficient use of 'perror' library call; should be used to test system calls: 'fork', 'open', 'dup2', 'close', 'exec'"),
                    })

tests = (compiletest2, badargtest, lstest, badcmdtest, \
         progtest, argstest1, argstest2, apitest)
ectests = []

runtests(compiletest1, tests, ectests)
