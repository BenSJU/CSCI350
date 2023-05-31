from testcommon import runbasictest, runcompiletest, runtests

def compiletest():
  return runcompiletest("gcc -Wall -o helloname ../../helloname.c")

def failtest1():
  command = "./helloname"
  expected = "Usage: ./helloname name"
  return runbasictest(command, expected)

def failtest2():
  command = "./helloname Peter Ohmann"
  expected = "Usage: ./helloname name"
  return runbasictest(command, expected)

def successtest():
  command = "./helloname Peter"
  expected = "Hello, Peter!"
  return runbasictest(command, expected)

tests = (failtest1, failtest2, successtest)
ectests = []

runtests(compiletest, tests, ectests)
