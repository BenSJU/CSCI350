from testcommon import runbasictest, runinteractivetest, runmultilinetest, \
                       runcompiletest, runtests

def compiletest():
  return runcompiletest("gcc -Wall -o guess ../../guessnumber.c")

def failtest1():
  command = "./guess"
  expected = "Usage: ./guess randomseed [maxnumber]"
  return runbasictest(command, expected)

def shorttest():
  command = "./guess 99 2"
  prompts = ("0")
  expected = \
"""Guess the number: 0
You guessed it!
"""
  return runmultilinetest(command, prompts, expected)

def longtest():
  command = "./guess 29"
  prompts = ("50", "90", "60", "82")
  expected = \
"""Guess the number: 50
Higher!
Guess the number: 90
Lower!
Guess the number: 60
Higher!
Guess the number: 82
You guessed it!
"""
  return runmultilinetest(command, prompts, expected)

def anothertest():
  command = "./guess 10 10"
  prompts = ("5", "3", "1", "0")
  expected = \
"""Guess the number: 5
Lower!
Guess the number: 3
Lower!
Guess the number: 1
Lower!
Guess the number: 0
You guessed it!
"""
  return runmultilinetest(command, prompts, expected)

tests = (failtest1, shorttest, longtest, anothertest)
ectests = []

runtests(compiletest, tests, ectests)
