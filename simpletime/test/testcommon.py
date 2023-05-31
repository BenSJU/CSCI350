import pexpect
import os
import re
import tempfile
import shutil
import signal
import time
from contextlib import contextmanager

totalTests = 0
passedTests = 0

totalEC = 0
passedEC = 0

@contextmanager
def _testdir():
  # significant code and motivation:
  # https://stackoverflow.com/questions/21257782/how-to-remove-file-when-program-exits
  path = tempfile.mkdtemp(suffix=".testdir", dir=os.getcwd())
  try:
    yield path
  finally:
    pass
    try:
      shutil.rmtree(path)
    except IOError:
      # do nothing...user has to delete it manually!
      pass

def _testerror(message):
  print("ERROR: test is written incorrectly: " + message + ".")
  print("REPORT THIS TO YOUR INSTRUCTOR!")

def _testfailprint(expected, got):
  try:
    print("Test Failed!")
    print("Expected: " + expected)
    print("Got: " + got)
  except:
    _testerror("Error while printing error message")

def _testgenericfail(msg):
  try:
    print("Test Failed!")
    print(msg)
  except:
    _testerror("Error while printing generic error message")

# return: (success, output)
# success is True or False (if we gathered real output)
# output only makes sense if success is True (but should still be printed!)
# Note: prompts can also include special commands.  So far supported:
#   _signal:SIGUSR1
#   _sleep:N  (N seconds)
def _runandcaptureoutput(command, prompts=[], timeout=5,
                         replacefrom=[], replaceto=[]):
  try:
    child = pexpect.spawn(command, timeout=timeout)
    for i in range(len(prompts)):
      if prompts[i].strip() == "_signal:SIGUSR1":
        child.kill(signal.SIGUSR1)
      elif prompts[i].strip()[:7] == "_sleep:":
        try:
          time.sleep(float(prompts[i].strip()[7:]))
        except:
          _testerror("Improperly implemented sleep")
      else:
        child.sendline(prompts[i])
    child.sendeof()
    try:
      output = child.read().decode("utf-8")
    except:
      print("Failed to decode output (contains unprintable characters)!")
      return (False, "???")
    output = output.strip()
    for i in range(min(len(replacefrom), len(replaceto))):
      output = output.replace(replacefrom[i], replaceto[i])
    return (True, output)
  except pexpect.EOF:
    try:
      return (False, child.before.decode("utf-8"))
    except:
      print("Failed to decode output (contains unprintable characters)!")
      return (False, "???")

def runnotice(command):
  print("--- Running: '" + command + "'")

def runbasictest(command, expectedoutput):
  runnotice(command)
  try:
    child = pexpect.spawn(command)
    child.expect(pexpect.EOF)
    if child.before.decode("utf-8").strip() != expectedoutput.strip():
      raise
    return True
  except:
    try:
      _testfailprint(expectedoutput, child.before.decode("utf-8"))
    except:
      print("Failed to decode output (contains unprintable characters)!")
    return False

def runinteractivetest(command, prompts, expectedoutputs):
  runnotice(command)

  if len(prompts) != len(expectedoutputs):
    _testerror("mismatched number of prompts and outputs")
    return False

  try:
    child = pexpect.spawn(command)
    for i in range(len(prompts)):
      this_prompt = prompts[i]
      this_expected = expectedoutputs[i]
      if this_prompt:
        child.sendline(this_prompt)
      child.expect_exact(this_expected, timeout=5)
      if child.before and \
         child.before.decode("utf-8").strip() != this_prompt.strip():
        raise
    return True
  except:
    try:
      _testfailprint(this_expected, child.before.decode("utf-8"))
    except:
      print("Failed to decode output (contains unprintable characters)!")
    return False

def runmultilinetest(command, prompts, expected, \
                     replacefrom=['\r'], replaceto=['']):
  runnotice(command)
  (success, output) = _runandcaptureoutput(command, prompts)
  if not success:
    _testfailprint(expected, output)
    return False
  
  for i in range(min(len(replacefrom), len(replaceto))):
    output = output.replace(replacefrom[i], replaceto[i])
  if output == expected.strip():
    return True
  else:
    _testfailprint("\n" + expected, "\n" + output)
    return False

# termexpected = expected output to the terminal
# [fname1, ...] = names of file to compare to...
# [fexpected1, ...] = expected contents of those files
def runfiletest(command, prompts, termexpected, fnames, fexpecteds, \
                replacefrom=['\r'], replaceto=['']):
  if len(fnames) != len(fexpecteds):
    _testerror("Expected file arguments are messed up")
    return False
  
  runnotice(command)
  (success, output) = _runandcaptureoutput(command, prompts, \
                                           replacefrom=replacefrom, \
                                           replaceto=replaceto)
  if not success:
    _testfailprint(termexpected, output)
    return False
  if output != termexpected.strip():
    print("Mismatched terminal (non-file) output!")
    _testfailprint(termexpected, output)
    return False
  
  for i in range(len(fnames)):
    output = "<could not read file>"
    try:
      with open(fnames[i], 'r') as f:
        output = f.read().strip()
        if output != fexpecteds[i].strip():
          print("Mismatch in file '" + fnames[i] + "'")
          _testfailprint(fexpecteds[i], output)
          return False
    except:
      print("Mismatch in (possibly non-existent?) file '" + fnames[i] + "'")
      _testfailprint(fexpecteds[i], output)
      return False
  return True

# currently, "API" tests are pretty basic and just use grep
# keyword_map: a funky dictionary mapping keywords to their required
#              number of uses
#              {'keyword_regex' : (min_uses, max_uses, error_message)}
#              (Note that min_uses and max_uses are inclusive, so if you
#              want exactly 0 uses, you should do 0,0.)
def runapitest(fname, keyword_map):
  runnotice("*API tests...*")

  try:
    with open(fname, 'r') as f:
      filecontents = f.read().strip()
  except:
    print("Problem reading (possibly non-existent?) file '" + fname + "'")
    return False

  try:
    for (kw_regex, (min_uses, max_uses, error_msg)) in keyword_map.items():
      use_count = len(re.findall(kw_regex, filecontents))
      if use_count < min_uses or use_count > max_uses:
        _testgenericfail("API Problem: " + error_msg)
        return False
  except:
    _testerror("malformed keyword map for API tests")

  return True

# comparefunc signature: f(expected:str, actual:str)
# should return True if they match
def rungenerictest(command, prompts, expected, comparefunc):
  runnotice(command)

  try:
    child = pexpect.spawn(command)
    for i in range(len(prompts)):
      child.sendline(prompts[i])
    try:
      output = child.read().decode("utf-8")
    except:
      print("Failed to decode output (contains unprintable characters)!")
      output = "???"
    if comparefunc(expected, output):
      return True
    else:
      _testfailprint("\n" + expected, "\n" + output)
      return False
  except pexpect.EOF:
    try:
      _testfailprint(expected, child.before.decode("utf-8"))
    except:
      print("Failed to decode output (contains unprintable characters)!")
    return False

def runcompiletest(compilecommand):
  runnotice(compilecommand)
  child = pexpect.spawn(compilecommand)
  output = child.read()
  if not output:
    return True
  else:
    print("Compilation error!")
    print(output.decode("utf-8"))
    return False

def _runonetest(toRun, isEC=False):
  global totalTests, passedTests, totalEC, passedEC

  if isEC:
    totalEC += 1
  else:
    totalTests += 1

  if toRun():
    if isEC:
      passedEC += 1
    else:
      passedTests += 1
    return True
  return False

def _printpassedratio():
  print("\n\nTests passed: %d / %d" % (passedTests, totalTests))
  if totalEC > 0:
    print("Extra Credit Tests passed: %d / %d" % (passedEC, totalEC))

def runtests(compiletest, othertests, ectests):
  with _testdir() as test_dir:
    os.chdir(test_dir)

    if not _runonetest(compiletest):
      _printpassedratio()
      exit(1)

    for test in othertests:
      _runonetest(test)
    for test in ectests:
      _runonetest(test, True)
    _printpassedratio()
