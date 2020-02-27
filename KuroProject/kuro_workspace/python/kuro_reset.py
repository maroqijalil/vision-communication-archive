import ioutils as io
import pkgutils as pkg

import argparse

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument('package', nargs='?', default='all')
parser.add_argument('residuals', nargs='*', default=[ ])
parser.add_argument('command')
parser.add_argument('-h', '--help', action='store_true')
args = parser.parse_args()

# if args.package == 'all':
#   import kuro_reset_all

if len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), args.package, "reset [-h]")
  io.exitFail()

io.printAsync(io.Empty(), "checking package..", io.Progress())
if not pkg.packageIsDir(args.package):
  io.println(io.Fail(), args.package, "does not exist")
  io.exitFail()

if not pkg.packageIsGitRepository(args.package):
  io.println(io.Fail(), args.package, "is not a git repository")
  io.exitFail()

repo = pkg.getPackageRepo(args.package)

if not pkg.repoIsNotClean(repo):
  io.println(io.Success(), "done, nothing to be reset in", args.package)
  io.exitSuccess()

string = io.scanln(io.Warn(), "are you sure you want to reset", args.package, "\b? (yes/no):")
io.newLine()

if string == 'no':
  io.println(io.Fail(), "cancelled")
  io.exitFail()
elif string != 'yes':
  io.println(io.Fail(), "invalid input, cancelled")
  io.exitFail()

io.printAsync(io.Empty(), "resetting", args.package, "\b..", io.Progress())
repo.git.reset('--hard')

io.printAsync(io.Empty(), "cleaning", args.package, "\b..", io.Progress())
repo.git.clean('-xdf')

io.println(io.Success(), "done,", args.package, "has been reset")
io.exitSuccess()