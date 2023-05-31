import subprocess
import re
import os
import os.path
from testcommon import runbasictest, runinteractivetest, runmultilinetest, \
                       runfiletest, runcompiletest, rungenerictest, \
                       runtests, runapitest

def compiletest():
  return runcompiletest("gcc -Wall -o myls ../../basicls.c")

def _testhelper(flags, expected):
  command = "./myls " + flags
  return runmultilinetest(command, [], expected)

def badargtest1():
  return _testhelper("", "Usage: ./myls [-l] path")

def badargtest2():
  return _testhelper("nosuchfile", "Unable to stat path: No such file or directory")

def singlefiletest1():
  return _testhelper("myls", "myls")

def singlefiletest2():
  return _testhelper("../../mydir/file2", "../../mydir/file2")

def dirtest1():
  return _testhelper("../../mydir", ".\n..\nfile1\nfile2\nnextdir\nanotherdir")

def dirtest2():
  return _testhelper("../../mydir/anotherdir", ".\n..")

def dirtest3():
  return _testhelper("../../mydir/nextdir", ".\n..\nemptyfile\nsmallfile\ninnerdir")

def flagtest1():
  path_todo = "../../mydir/file2"
  data = os.stat(path_todo)
  expected = " ".join([str(x) for x in [path_todo, data.st_uid, \
                                        data.st_gid, data.st_size]])
  return _testhelper("-l " + path_todo, expected)

def flagtest2():
  path_todo = "../../mydir"
  expected = ""
  for f in ['.', '..'] + os.listdir(path_todo):
    this_file_path = os.path.join(path_todo, f)
    data = os.stat(this_file_path)
    expected += " ".join([str(x) for x in [f, data.st_uid, \
                                           data.st_gid, data.st_size]])
    expected += "\n"
  return _testhelper("-l " + path_todo, expected)

def apitest():
  return runapitest("../../basicls.c",
                    {"stat[ ]*\(" : (2, 1000, "Missing 'stat' system call to get file data"),
                     "S_ISDIR[ ]*\(" : (1, 1000, "Missing check if file is a directory via 'S_ISDIR' library function"),
                     "opendir[ ]*\(" : (1, 1000, "Missing 'opendir' system call for handling directories"),
                     "readdir[ ]*\(" : (1, 1000, "Missing 'readdir' system call for handling directories"),
                     "strcat[ ]*\(" : (1, 1000, "Missing 'strcat' library call to handle full file paths"),
                     "malloc[ ]*\(" : (1, 1000, "Missing 'malloc' library call to handle full file paths"),
                     "free[ ]*\(" : (1, 1000, "Missing 'free' system call to free allocated memory"),
                     "perror[ ]*\(" : (3, 1000, "Insufficient use of 'perror' library call; should be used to test system calls: 'opendir', 'stat' (for original file and files in directory)"),
                    })

def ectest1():
  return _testhelper("-r ../../mydir", \
                     "\n".join([".", "..", "file1", "file2", \
                                "nextdir", "anotherdir", \
                                "", "../../mydir/nextdir:", \
                                "emptyfile", "smallfile", "innerdir", \
                                "", "../../mydir/nextdir/innerdir:", \
                                "bestfile", "", \
                                "../../mydir/anotherdir:"]))

tests = (badargtest1,badargtest2,singlefiletest1,singlefiletest2,\
         dirtest1,dirtest2,dirtest3,flagtest1,flagtest2, apitest)
ectests = [ectest1,]

runtests(compiletest, tests, ectests)
