import ioutils as io
import pkgutils as pkg

import argparse
import git

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument('package', nargs='?', default='all')
parser.add_argument('command')
parser.add_argument('residuals', nargs='*', default=[ ])
parser.add_argument('-h', '--help', action='store_true')
args = parser.parse_args()

if args.package != 'all' or len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), "kuro push [-h]")
  io.exitFail()

all_clear = True

for package in pkg.packages:

  package_clear = True

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

  io.printAsync(io.Empty(), "checking remotes..", io.Progress())
  if 'origin' not in repo.remotes:
    io.println(io.Fail(), package, "does not have origin remote")
    all_clear = False
    continue

  is_up_to_date = True

  io.printAsync(io.Empty(), "fetching", package, "\b..", io.Progress())
  for fetch_info in repo.remotes['origin'].fetch():
    if fetch_info.flags & git.FetchInfo.NEW_HEAD != 0:
      if package_clear:
        io.println(io.Warn(), package, "push to origin remote:")
        package_clear = False
      io.println(io.SuccessList(), "update", fetch_info.ref, "to '{}'".format(fetch_info.commit.summary))

  io.printAsync(io.Empty(), "checking diffs..", io.Progress())
  try:
    commits_behind = pkg.getCommitsBehind(repo)
    commits_ahead = pkg.getCommitsAhead(repo)
  except git.GitCommandError:
    if package_clear:
      io.println(io.Warn(), package, "push to origin remote:")
      package_clear = False
    io.println(io.WarnList(), "remote does not have", repo.active_branch, "branch")
    all_clear = False
    continue

  if len(commits_ahead) == 0:
    if len(commits_behind) > 0:
      if package_clear:
        io.println(io.Warn(), package, "push to origin remote:")
        package_clear = False
      io.println(io.WarnList(), package, "is behind of master by", io.PluralCount('commit', len(commits_behind)))
      all_clear = False
      continue
    else:
      io.println(io.Success(), package, "is up to date with origin")
      continue

  io.printAsync(io.Empty(), "pushing", package, "\b..", io.Progress())
  for push_info in repo.remotes['origin'].push():
    if push_info.flags & git.PushInfo.NEW_HEAD != 0:
      if package_clear:
        io.println(io.Warn(), package, "push to origin remote:")
        package_clear = False
      io.println(io.SuccessList(), "update", push_info.ref, "to '{}'".format(push_info.commit.summary))

  if package_clear:
    io.println(io.Warn(), package, "push to origin remote:")
    package_clear = False
  io.println(io.SuccessList(), "done, pushing", io.PluralCount('commit', len(commits_ahead)))
  continue

if not all_clear:
  io.println(io.Warn(), "done, not all clear")
  io.exitSuccess()

io.println(io.Success(), "done, all clear")
io.exitSuccess()