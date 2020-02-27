import ioutils as io
import pkgutils as pkg

import argparse
import os
import subprocess
import re

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument('package', nargs='?', default='all')
parser.add_argument('residuals', nargs='*', default=[ ])
parser.add_argument('command')
parser.add_argument('-h', '--help', action='store_true')
args = parser.parse_args()

if len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), args.package, "view <executable> [-h]")
  io.exitSuccess()

target_packages = list()
target_executables = list()

if args.package == 'all':

  package_clear = True

  for package in pkg.packages:
    if io.runCommandMuted("screen -ls | grep {}".format(package)):
      io.printAsync(io.Empty(), "checking executables in ", package, "..", io.Progress())
      stdout = subprocess.check_output(["screen", "-ls | grep", package]).decode("utf-8")
      executables = re.findall('__([a-z + _]+)', stdout)
      if len(executables)>0:
        for executable in executables:
          if package_clear:
            io.println(io.Warn(), "some packages are running:")
            package_clear = False

          target_packages.append(package)
          target_executables.append(executable)
          io.println(io.WarnList(), executable, "in", package)

  if len(target_executables) == 0:
    string = io.println(io.Fail(), "no executable is running")
    io.exitSuccess()
  elif len(target_executables) == 1:
    string = io.scanln(io.Warn(), "do you want to kill it? (yes/no):")
  else:
    string = io.scanln(io.Warn(), "do you want to kill them? (yes/no):")
  io.newLine()

  if string == 'no':
    io.println(io.Fail(), "cancelled")
    io.exitFail()
  elif string != 'yes':
    io.println(io.Fail(), "invalid input, cancelled")
    io.exitFail()

  i = 0
  while i < len(target_executables):
    if not io.runCommandMuted("screen -XS {}__{} quit".format(target_packages[i], target_executables[i])):
      io.println(io.Fail(), "failed to kill", target_executables[i], "in", target_packages[i])

    io.println(io.Success(), "killed", target_executables[i], "in", target_packages[i])
    i = i + 1

  io.exitSuccess()