import ioutils as io
import pkgutils as pkg

import argparse

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument('package', nargs='?', default='all')
parser.add_argument('residuals', nargs='*', default=[ ])
parser.add_argument('command')
parser.add_argument('-h', '--help', action='store_true')
args = parser.parse_args()

if args.package != 'all' or len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), "kuro reset [-h]")
  io.exitFail()

io.printAsync(io.Empty(), "checking packages..", io.Progress())
dirty_packages = []
for package in pkg.packages:
  if not pkg.packageIsGitRepository(package):
    continue

  repo = pkg.getPackageRepo(package)
  if pkg.repoIsNotClean(repo):
    dirty_packages.append(package)

if len(dirty_packages) < 1:
  io.println(io.Success(), "done, nothing to be reset")
  io.exitSuccess()

io.println(io.Warn(), "packages to be reset:")
for package in dirty_packages:
  io.println(io.WarnList(), package)
io.newLine()

string = io.scanln(io.Warn(), "are you sure you want to reset those packages? (yes/no):")
io.newLine()

if string == 'no':
  io.println(io.Fail(), "cancelled")
  io.exitFail()
elif string != 'yes':
  io.println(io.Fail(), "invalid input, cancelled")
  io.exitFail()

for package in dirty_packages:
  repo = pkg.getPackageRepo(package)

  io.printAsync(io.Empty(), "resetting", package, "\b..", io.Progress())
  repo.git.reset('--hard')

  io.printAsync(io.Empty(), "cleaning", package, "\b..", io.Progress())
  repo.git.clean('-xdf')

  io.println(io.Success(), package, "has been reset")

io.exitSuccess()