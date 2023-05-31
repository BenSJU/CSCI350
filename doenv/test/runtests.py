import subprocess
import re
import os
from testcommon import runbasictest, runinteractivetest, runmultilinetest, \
                       runcompiletest, rungenerictest, runtests, runapitest

def compiletest():
  return runcompiletest("gcc -Wall -o myenv ../../doenv.c")

def _skipline(line):
  return line.startswith("OLDPWD") or \
         line.startswith("PWD") or \
         line.startswith("SHLVL") or \
         line.startswith("_=")

def env_comparator(expected, actual):
  expected_lines = expected.strip().split('\n')
  actual_lines = actual.strip().split('\n')
  expected_lines.sort()
  actual_lines.sort()
  
  expected_lines = [line.strip() for line in expected_lines if not _skipline(line)]
  actual_lines = [line.strip() for line in actual_lines if not _skipline(line)]

  for i in range(min(len(expected_lines), len(actual_lines))):
    if expected_lines[i] != actual_lines[i]:
      print("first mismatch: ")
      print("expected: " + expected_lines[i])
      print("actual: " + actual_lines[i])
      print(".............................")
      return False
  return len(expected_lines) == len(actual_lines)

def _flagtest(flags):
  command = "./myenv " + flags
  expected = subprocess.check_output('env ' + flags, shell=True).decode("utf-8")
  return rungenerictest(command, [], expected, env_comparator)

def noparamtest():
  return _flagtest("")

def iflagtest1():
  return _flagtest(" -i BOB=uncle TOM=cat")

def iflagtest2():
  return _flagtest(" -i ")

def smallenvtest():
  eBackup = dict(os.environ)
  os.environ.clear()
  os.environ["FIRSTVAR"] = "someval"
  os.environ["SECONDVAR"] = "otherval"
  result = _flagtest(" -i A=a B=b C=c D=d")
  os.environ.clear()
  os.environ.update(eBackup)
  return result

def noitest():
  return _flagtest(" FREE=bird FAV_LANG=c A=bcd")

def apitest():
  return runapitest("../../doenv.c",
                    {"malloc[ ]*\(" : (1, 1000, "Missing allocation of new environment; use 'malloc'"),
                     "[ ]*=[ ]*NULL" : (1, 1000, "Missing NULL termination of new environment; assign 'newenviron[?] = NULL'"),
                    })

tests = (noparamtest,iflagtest1,iflagtest2,smallenvtest)
ectests = (noitest,apitest)

runtests(compiletest, tests, ectests)
