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

# if args.package == 'all':
#   import kuro_pull_all

if len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), args.package, "pull [-h]")
  io.exitFail()

io.printAsync(io.Empty(), "checking package..", io.Progress())
if not pkg.packageIsDir(args.package):
  io.println(io.Fail(), args.package, "does not exist")
  io.exitFail()

if not pkg.packageIsGitRepository(args.package):
  io.println(io.Fail(), args.package, "is not a git repository")
  io.exitFail()

repo = pkg.getPackageRepo(args.package)

io.printAsync(io.Empty(), "checking remotes..", io.Progress())
if 'origin' not in repo.remotes:
  io.println(io.Fail(), args.package, "does not have origin remote")
  io.exitFail()

is_up_to_date = True

io.printAsync(io.Empty(), "fetching", args.package, "\b..", io.Progress())
for fetch_info in repo.remotes['origin'].fetch():
  if fetch_info.flags & git.FetchInfo.NEW_HEAD != 0:
    if is_up_to_date:
      io.println(io.Success(), "updated branch:")
      is_up_to_date = False

    io.println(io.SuccessList(), "update", fetch_info.ref, "to '{}'".format(fetch_info.commit.summary))

if not is_up_to_date:
  io.newLine()

io.printAsync(io.Empty(), "checking diffs..", io.Progress())
try:
  commits_behind = pkg.getCommitsBehind(repo)
  commits_ahead = pkg.getCommitsAhead(repo)
except git.GitCommandError:
  io.println(io.Warn(), "remote does not have", repo.active_branch, "branch")
  io.println(io.Fail(), "aborted")
  io.exitFail()

if len(commits_behind) == 0:
  if len(commits_ahead) > 0:
    io.println(io.Warn(), args.package, "is ahead of master by", io.PluralCount('commit', len(commits_ahead)))
    io.exitSuccess()
  else:
    io.println(io.Success(), args.package, "is up to date with origin")
    io.exitSuccess()

io.printAsync(io.Empty(), "checking dirty state..", io.Progress())
if pkg.repoIsNotClean(repo):
  inp = io.scanln(io.Warn(), "found dirty state, are you sure you want to reset it? (yes/no):")
  io.newLine()
  if inp == 'yes':
    io.printAsync(io.EmptyList(), "resetting package..", io.Progress())
    pkg.resetRepo(repo)
    io.println(io.SuccessList(), "done reset package")
    io.newLine()
  elif inp != 'no':
    io.println(io.Fail(), "invalid input, cancelled")
    io.exitFail()

try:
  io.printAsync(io.Empty(), "merging branches..")
  pkg.mergeRepo(repo, "origin/{}".format(repo.active_branch))

except git.CommandError:
  io.println(io.Warn(), "done, found merge conflict")
  io.exitSuccess()

io.println(io.Success(), "done, merged", io.PluralCount('commit', len(commits_behind)))
io.exitSuccess()