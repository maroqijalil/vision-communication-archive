import ioutils as io
import pkgutils as pkg

import argparse

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument('package', nargs='?', default='all')
parser.add_argument('count', nargs='?', default='5')
parser.add_argument('residuals', nargs='*', default=[ ])
parser.add_argument('command')
parser.add_argument('-h', '--help', action='store_true')
args = parser.parse_args()

if args.package == 'all' or len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), args.package, "log [-h]")
  io.exitFail()

io.printAsync(io.Empty(), "checking package..", io.Progress())
if not pkg.packageIsDir(args.package):
  io.println(io.Fail(), args.package, "does not exist")
  io.exitFail()

if not pkg.packageIsGitRepository(args.package):
  io.println(io.Fail(), args.package, "is not a git repository")
  io.exitFail()

repo = pkg.getPackageRepo(args.package)

io.printAsync(io.Empty(), "getting commits..", io.Progress())
commits = list(repo.iter_commits(repo.active_branch, max_count=args.count))

if len(commits) < 1:
  io.println(io.Warn(), "no commit found in", args.package)
  io.exitSuccess()

io.println(io.Success(), "found", len(commits), io.Plural("commit", len(commits)), "in", args.package, ":")
for commit in commits:
  io.println(io.EmptyList(), str(commit.name_rev)[:20], "\b,", commit.summary)

io.exitSuccess()