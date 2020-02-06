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

if args.package != 'all' or len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), "kuro status [-h]")
  io.exitFail()

all_clear = True

for package in pkg.packages:

  io.printAsync(io.Empty(), "checking package..", io.Progress())
  if not pkg.packageIsDir(package):
    io.println(io.Fail(), package, "does not exist")
    all_clear = False
    continue

  if not pkg.packageIsGitRepository(package):
    io.println(io.Fail(), package, "is not a git repository")
    all_clear = False
    continue

  repo = pkg.getPackageRepo(package)

  package_clear = True

  io.printAsync(io.Empty(), "checking untracked files in", package, "\b..", io.Progress())
  untracked_files = repo.untracked_files
  if len(untracked_files) > 0:
    all_clear = False
    if package_clear:
      io.println(io.Warn(), package, "({})".format(repo.active_branch), "has some problems:")
      package_clear = False
    io.println(io.WarnList(), "found", len(untracked_files), "untracked files")

  io.printAsync(io.Empty(), "checking modified files in", package, "\b..", io.Progress())
  modified_files = [ item.a_path for item in repo.index.diff(None) ]
  if len(modified_files) > 0:
    all_clear = False
    if package_clear:
      io.println(io.Warn(), package, "({})".format(repo.active_branch), "has some problems:")
      package_clear = False
    io.println(io.WarnList(), "found", len(modified_files), "modified files")

  io.printAsync(io.Empty(), "checking uncommitted changes in", package, "\b..", io.Progress())
  uncommitted_changes = pkg.getUncommittedChanges(repo)
  if len(uncommitted_changes) > 0:
    all_clear = False
    if package_clear:
      io.println(io.Warn(), package, "({})".format(repo.active_branch), "has some problems:")
      package_clear = False
    io.println(io.WarnList(), "found", len(uncommitted_changes), "uncommitted changes")

  if package_clear:
    io.println(io.Success(), package, "({})".format(repo.active_branch), "is clear")

io.printAsync(io.Empty(), "searching untracked packages..", io.Progress())
untracked_clear = True
for package in os.listdir(pkg.packages_path):
  if package in pkg.packages:
    continue

  if not pkg.packageIsDir(package):
    continue

  if not pkg.packageIsGitRepository(package):
    continue

  if untracked_clear:
    io.println()
    io.println(io.Warn(), 'found some untracked packages:')
    untracked_clear = False

  io.println(io.WarnList(), package)
  all_clear = False

if not untracked_clear:
  io.newLine()

if not all_clear:
  io.println(io.Warn(), "done, not all clear")
  io.exitSuccess()

io.println(io.Success(), "done, all clear")
io.exitSuccess()