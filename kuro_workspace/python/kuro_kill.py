import ioutils as io

import argparse

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument('package', nargs='?', default='none')
parser.add_argument('command')
parser.add_argument('executable', nargs='?', default='none')
parser.add_argument('residuals', nargs='*', default=[ ])
parser.add_argument('-h', '--help', action='store_true')
args = parser.parse_args()

if args.package == 'none' or args.executable == 'none' or len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), args.package, "kill <executable> [-h]")
  io.exitFail()

io.printAsync(io.Empty(), "checking executable..", io.Progress())
if not io.runCommandMuted("screen -ls | grep {}__{}".format(args.package, args.executable)):
  io.println(io.Fail(), args.executable, "in", args.package, "has not been run")
  io.exitFail()

io.printAsync(io.Empty(), "killing executable..", io.Progress())
if not io.runCommandMuted("screen -XS {}__{} quit".format(args.package, args.executable)):
  io.println(io.Fail(), "failed to kill", args.executable, "in", args.package)
  io.exitFail()

io.println(io.Success(), "done killing", args.executable, "in", args.package)
io.exitSuccess()