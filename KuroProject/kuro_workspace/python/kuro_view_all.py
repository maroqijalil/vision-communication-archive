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

if args.package == 'all':
  executable_exist = False
  for package in pkg.packages:
    if io.runCommandMuted("screen -ls | grep {}".format(package)):
      io.printAsync(io.Empty(), "checking executables in ", package, "..", io.Progress())
      stdout = subprocess.check_output(["screen", "-ls | grep", package]).decode("utf-8") 
      executables = re.findall('__([a-z + _]+)', stdout)
      if len(executables) > 0:
        executable_exist = True
        for executable in executables:
          io.println(io.Success(), executable, "is running")
  
  if not executable_exist:
    io.println(io.Fail(), "no executable is running")
  
  io.exitSuccess()