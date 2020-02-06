import ioutils as io
import pkgutils as pkg

import argparse
import os

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument('package', nargs='?', default='none')
parser.add_argument('command')
parser.add_argument('executable', nargs='?', default='none')
parser.add_argument('residuals', nargs='*', default=[ ])
parser.add_argument('-h', '--help', action='store_true')
args = parser.parse_args()

if args.package == 'none' or args.executable == 'none' or len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), args.package, "run <executable> [-h]")
  io.exitFail()

io.printAsync(io.Empty(), "checking executable..", io.Progress())
if io.runCommandMuted("screen -ls | grep {}__{}".format(args.package, args.executable)):

  io.println(io.Warn(), args.executable, "in", args.package, "has been run")
  string = io.scanln(io.Warn(), "do you want to kill it? (yes/no):")
  io.newLine()

  if string == 'no':
    io.println(io.Fail(), "cancelled")
    io.exitFail()
  elif string != 'yes':
    io.println(io.Fail(), "invalid input, cancelled")
    io.exitFail()

  io.printAsync(io.Empty(), "killing executable..", io.Progress())
  if not io.runCommandMuted("screen -XS {}__{} quit".format(args.package, args.executable)):
    io.println(io.Fail(), "failed to kill", args.executable, "in", args.package)
    io.exitFail()

  io.println(io.Success(), "killed", args.executable, "in", args.package)

io.printAsync(io.Empty(), "checking log file..", io.Progress())
log_file = os.path.join(pkg.workspace_path, 'log','{}__{}.log'.format(args.package, args.executable))
if os.path.exists(log_file):
  io.println(io.Success(), "removed previous log file")
  os.remove(log_file)

io.println(io.Success(), "running", args.executable, "in", args.package)
io.println(io.Warn(), "use ctrl+a,d to detach and ctrl+c to exit")
io.scanAnyKey(io.Warn(), "press any key to continue")

run_script = os.path.join(pkg.workspace_path, 'run.sh')
command = "bash {} {} {} {}".format(run_script, args.package, args.executable, log_file)
if not io.runCommandMuted("screen -S {}__{} {}".format(args.package, args.executable, command)):
  io.println(io.Fail(), "failed to run", args.executable, "in", args.package)
  io.exitFail()

io.println(io.Success(), "done running", args.executable, "in", args.package)
io.exitSuccess()