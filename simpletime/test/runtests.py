import subprocess
import re
import os
from testcommon import runbasictest, runinteractivetest, runmultilinetest, \
                       runcompiletest, rungenerictest, runapitest, runtests

def compiletest1():
  return runcompiletest("gcc -Wall -o stime ../../simpletime.c")

def compiletest2():
  return runcompiletest("gcc -Wall -o runner ../runner.c")

def _testhelper(flags, expected):
  command = "./stime " + flags
  return runmultilinetest(command, [], expected)

def badargtest():
  return _testhelper("", "Usage: ./stime prog [args...]")

def sleeptest1():
  return _testhelper("sleep 2", "Time in seconds: 2")

def sleeptest2():
  return _testhelper("sleep 1", "Time in seconds: 1")

def badprogtest1():
  return _testhelper("./nosuchprog", "Could not exec child: No such file or directory")

def badprogtest2():
  return _testhelper("doesntexist arg1 arg2", "Could not exec child: No such file or directory")

def badprogtest3():
  return _testhelper("/root/passwd", "Could not exec child: Permission denied")

def cattest():
  expected = subprocess.check_output("cat ../../stuff/samplefile", shell=True).decode("utf-8")
  return _testhelper("cat ../../stuff/samplefile", expected + "Time in seconds: 0")

def runnertest1():
  os.environ["TIMETORUN"] = "1"
  print("(Setting TIMETORUN=1)")
  return _testhelper("./runner", "All done!\nTime in seconds: 1")

def runnertest2():
  os.environ["TIMETORUN"] = "4"
  print("(Setting TIMETORUN=4)")
  return _testhelper("./runner", "All done!\nTime in seconds: 4")

def apitest():
  return runapitest("../../simpletime.c",
                    {"clock[ ]*\(" : (0, 0, "Do not use 'clock' library function here; it measures CPU rather than wall-clock time"),
                     "time[ ]*\(" : (2, 1000, "Use 'time' library call to get both the start and end time"),
                     "execvp[ ]*\(" : (1, 1000, "Missing use of 'execvp' system call wrapper; should be used to launch child process"),
                     "WIFEXITED[ ]*\(" : (1, 1000, "Use WIFEXITED macro to check for child exit"),
                     "WEXITSTATUS[ ]*\(" : (1, 1000, "Use WEXITSTATUS macro to check child exit status"),
                     "perror[ ]*\(" : (3, 1000, "Insufficient use of 'perror' library call; should be used to test system calls: 'fork', 'wait', 'exec*'"),
                    })


tests = (compiletest2,badargtest,sleeptest1,sleeptest2, \
         badprogtest1, badprogtest2, badprogtest3, cattest, \
         runnertest1, runnertest2, apitest)
ectests = []

runtests(compiletest1, tests, ectests)
