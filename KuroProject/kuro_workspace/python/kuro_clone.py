import ioutils as io
import pkgutils as pkg

import argparse
import git

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument('package', nargs='?', default='all')
parser.add_argument('branch', nargs='?', default='master')
parser.add_argument('residuals', nargs='*', default=[ ])
parser.add_argument('command')
parser.add_argument('-h', '--help', action='store_true')
args = parser.parse_args()

if len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), args.package, "clone [branch: default is master] [-h]")
  io.exitFail()

avaliable_pkg = False

io.printAsync(io.Empty(), "checking package..", io.Progress())
if not pkg.packageIsDir(args.package):
  for package in pkg.packages:
    if args.package == package:
      avaliable_pkg = True
      break
  
if not avaliable_pkg:
  io.println(io.Fail(), args.package, "does not exist")
  io.exitFail()

if pkg.packageIsGitRepository(args.package):
  io.println(io.Success(), args.package, "already cloned")
  io.exitFail()

if avaliable_pkg:
  io.printAsync(io.Empty(), "cloning package..", io.Progress())
  try:
    pkg.cloneNewRepoFile(args.package, 'master')
  except Exception as e:
    io.println(io.Fail(), "could not clone the", args.package)
    io.println(e)
    io.exitFail()

if args.branch == 'master' and not avaliable_pkg:
  io.printAsync(io.Empty(), "cloning package..", io.Progress())
  try:
    repo = pkg.masterCloneRepo(args.package)
  except Exception as e:
    io.println(io.Fail(), "could not clone the", args.package)
    io.println(e)
    io.exitFail()
elif not args.branch == 'master' and not avaliable_pkg:
  io.printAsync(io.Empty(), "cloning package..", io.Progress())
  try:
    repo = pkg.cloneRepo(args.package, args.branch)
  except Exception as e:
    io.println(io.Fail(), "could not clone the", args.package)
    io.println(e)
    io.exitFail()

io.println(io.Success(), "done,", args.package, "cloned")
io.exitSuccess()

