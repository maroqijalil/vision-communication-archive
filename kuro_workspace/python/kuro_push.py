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
#   import kuro_push_all

if args.package == 'all' or len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), args.package, "push [-h]")
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

if len(commits_ahead) == 0:
  if len(commits_behind) > 0:
    io.println(io.Warn(), args.package, "is behind of master by", io.PluralCount('commit', len(commits_behind)))
    io.exitSuccess()
  else:
    io.println(io.Success(), args.package, "is up to date with origin")
    io.exitSuccess()

is_up_to_date = True

io.printAsync(io.Empty(), "pushing", args.package, "\b..", io.Progress())
for push_info in repo.remotes['origin'].push():
  if push_info.flags & git.PushInfo.NEW_HEAD != 0:
    if is_up_to_date:
      io.println(io.Success(), "updated branch:")
      is_up_to_date = False

    io.println(io.SuccessList(), "update", push_info.ref, "to '{}'".format(push_info.commit.summary))

if not is_up_to_date:
  io.newLine()

io.println(io.Success(), "done, pushing", io.PluralCount('commit', len(commits_ahead)))
io.exitSuccess()