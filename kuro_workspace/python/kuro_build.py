import ioutils as io
import pkgutils as pkg

import argparse
import os

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument('package', nargs='?', default='all')
parser.add_argument('residuals', nargs='*', default=[ ])
parser.add_argument('command')
parser.add_argument('-h', '--help', action='store_true')
args = parser.parse_args()

if len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), "kuro build [-h]")
  io.exitFail()

command = "colcon build --symlink-install --merge-install"
if args.package != 'all':
  command = "{} --packages-up-to {}".format(command, args.package)

io.println(io.Warn(), "calling colcon build")

original_path = os.getcwd()
os.chdir(pkg.workspace_path)

io.newLine()
if os.system(command) == 0:
  io.newLine()
  io.println(io.Success(), "build succesfully")
else:
  io.newLine()
  io.println(io.Fail(), "build failed")

os.chdir(original_path)
io.exitSuccess()