import ioutils as io
import pkgutils as pkg

import argparse

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument('package', nargs='?', default='all')
parser.add_argument('residuals', nargs='*', default=[ ])
parser.add_argument('command')
parser.add_argument('-h', '--help', action='store_true')
args = parser.parse_args()

if len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), args.package, "status [-h]")
  io.exitFail()

io.printAsync(io.Empty(), "checking package..", io.Progress())
if not pkg.packageIsDir(args.package):
  io.println(io.Fail(), args.package, "does not exist")
  io.exitFail()

if not pkg.packageIsGitRepository(args.package):
  io.println(io.Fail(), args.package, "is not a git repository")
  io.exitFail()

repo = pkg.getPackageRepo(args.package)

package_clear = True

io.printAsync(io.Empty(), "checking untracked files in", args.package, "\b..", io.Progress())
untracked_files = repo.untracked_files
if len(untracked_files) > 0:
  package_clear = False
  io.println(io.Warn(), "found", len(untracked_files), "untracked files:")
  for untracked_file in untracked_files:
    io.println(io.WarnList(), untracked_file)
  io.newLine()

io.printAsync(io.Empty(), "checking modified files in", args.package, "\b..", io.Progress())
modified_files = [ item.a_path for item in repo.index.diff(None) ]
if len(modified_files) > 0:
  package_clear = False
  io.println(io.Warn(), "found", len(modified_files), "modified files:")
  for modified_file in modified_files:
    io.println(io.WarnList(), modified_file)
  io.newLine()

io.printAsync(io.Empty(), "checking uncommitted changes in", args.package, "\b..", io.Progress())
uncommitted_changes = pkg.getUncommittedChanges(repo)
if len(uncommitted_changes) > 0:
  package_clear = False
  io.println(io.Warn(), "found", len(uncommitted_changes), "uncommitted changes:")
  for uncommitted_change in uncommitted_changes:
    io.println(io.WarnList(), uncommitted_change)
  io.newLine()

if not package_clear:
  io.println(io.Warn(), "done,", args.package, "is not clear")
  io.exitSuccess()

io.println(io.Success(), "done,", args.package, "is clear")
io.exitSuccess()