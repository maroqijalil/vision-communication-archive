import ioutils as io
import pkgutils as pkg

import argparse
import os
import subprocess
import re

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument('package', nargs='?', default='none')
parser.add_argument('command')
parser.add_argument('executable', nargs='?', default='none')
parser.add_argument('residuals', nargs='*', default=[ ])
parser.add_argument('-h', '--help', action='store_true')
args = parser.parse_args()

if len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), args.package, "view <executable> [-h]")
  io.exitSuccess()

if args.executable == 'none':
  if io.runCommandMuted("screen -ls | grep {}".format(args.package)):
    io.printAsync(io.Empty(), "checking executables in ", args.package, "..", io.Progress())
    stdout = subprocess.check_output(["screen", "-ls | grep", args.package]).decode("utf-8") 
    executables = re.findall('__([a-z + _]+)', stdout)
    if len(executables) > 0:
      for executable in executables:
        io.println(io.Success(), executable, "is running")

  else:
    io.println(io.Fail(), "no executable is running")

  io.exitSuccess()

io.printAsync(io.Empty(), "checking executable..", io.Progress())
if io.runCommandMuted("screen -ls | grep {}__{}".format(args.package, args.executable)):
  io.println(io.Success(), "resuming", args.executable, "in", args.package)
  io.println(io.Warn(), "use ctrl+a,d to detach and ctrl+c to exit")
  io.scanAnyKey(io.Warn(), "press any key to continue")
  if not io.runCommandMuted("screen -r {}__{}".format(args.package, args.executable)):
    io.println(io.Fail(), "failed to view", args.executable, "in", args.package)
    io.exitFail()

else:
  io.println(io.Warn(), args.executable, "in", args.package, "has not been run")

  io.printAsync(io.Empty(), "checking log files..", io.Progress())
  log_file = os.path.join(pkg.workspace_path, 'log','{}__{}.log'.format(args.package, args.executable))
  if not os.path.exists(log_file):
    io.println(io.Warn(), "no log file of", args.executable, "in", args.package, "found")
    io.exitFail()

  io.println(io.Success(), "viewing log file of", args.executable, "in", args.package)
  io.println(io.Warn(), "use ctrl+a,esc to scroll and ctrl+c to exit")
  io.scanAnyKey(io.Warn(), "press any key to continue")

  view_script = os.path.join(pkg.workspace_path, 'view.sh')
  command = "bash {} {}".format(view_script, log_file)
  if not io.runCommandMuted("screen -S {}__{} {}".format(args.package, args.executable, command)):
    io.println(io.Fail(), "failed to view", args.executable, "in", args.package)
    io.exitFail()

io.println(io.Success(), "done viewing", args.executable, "in", args.package)
io.exitSuccess()