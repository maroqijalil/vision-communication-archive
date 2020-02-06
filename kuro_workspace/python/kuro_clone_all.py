import ioutils as io
import pkgutils as pkg

import argparse
import git

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument('package', nargs='?', default='all')
parser.add_argument('residuals', nargs='*', default=[ ])
parser.add_argument('command')
parser.add_argument('-h', '--help', action='store_true')
args = parser.parse_args()

if args.package != 'all' or len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), "kuro clone [-h]")
  io.exitFail()

all_clear = True

for package in pkg.packages:

  avaliable_pkg = False

  io.printAsync(io.Empty(), "checking package..", io.Progress())
  if not pkg.packageIsDir(package):
    for package_check in pkg.packages:
      if package == package_check:
        avaliable_pkg = True
        break
    
  if not avaliable_pkg:
    io.println(io.Fail(), package, "does not exist")
    continue
  
  if pkg.packageIsGitRepository(package):
    io.println(io.Success(), package, "already cloned")
    continue

  if avaliable_pkg:
    io.printAsync(io.Empty(), "cloning package..", io.Progress())
    try:
      pkg.cloneNewRepoFile(args.package, 'master')
    except Exception as e:
      io.println(io.Fail(), "could not clone the", args.package)
      io.println(e)
      continue

  if not avaliable_pkg:
    io.printAsync(io.Empty(), "cloning package..", io.Progress())
    try:
      repo = pkg.masterCloneRepo(package)
    except Exception as e:
      io.println(io.Fail(), "cold not clone the", args.package)
      io.println(e)
      continue

  if pkg.packageIsGitRepository(package):
    io.println(io.Success(), "done", package, "cloned")
    continue

if not all_clear:
  io.println(io.Warn(), "done, not all clear")
  io.exitSuccess()

io.println(io.Success(), "done, all clear")
io.exitSuccess()

