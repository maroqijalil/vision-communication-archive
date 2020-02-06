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
  io.println(io.Yellow("usage:"), "kuro pull [-h]")
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

  io.printAsync(io.Empty(), "fetching", package, "\b..", io.Progress())
  for fetch_info in repo.remotes['origin'].fetch():
    if fetch_info.flags & git.FetchInfo.NEW_HEAD != 0:
      if package_clear:
        io.println(io.Warn(), package, "pull from origin remote:")
        package_clear = False
      io.println(io.SuccessList(), "update", fetch_info.ref, "to '{}'".format(fetch_info.commit.summary))

  io.printAsync(io.Empty(), "checking diffs..", io.Progress())
  try:
    commits_behind = pkg.getCommitsBehind(repo)
    commits_ahead = pkg.getCommitsAhead(repo)
  except git.GitCommandError:
    if package_clear:
      io.println(io.Warn(), package, "pull from origin remote:")
      package_clear = False
    io.println(io.WarnList(), "remote does not have", repo.active_branch, "branch")
    all_clear = False
    continue

  if len(commits_behind) == 0:
    if len(commits_ahead) > 0:
      if package_clear:
        io.println(io.Warn(), package, "pull from origin remote:")
        package_clear = False
      io.println(io.WarnList(), package, "is ahead of master by", io.PluralCount('commit', len(commits_ahead)))
      all_clear = False
      continue
    else:
      io.println(io.Success(), package, "is up to date with origin")
      continue

  io.printAsync(io.Empty(), "checking dirty state..", io.Progress())
  if pkg.repoIsNotClean(repo):
    inp = io.scanln(io.WarnList(), "found dirty state, are you sure you want to reset it? (yes/no):")
    io.newLine()
    if inp == 'yes':
      io.printAsync(io.EmptyList(), "resetting package..", io.Progress())
      pkg.resetRepo(repo)
      io.println(io.SuccessList(), "done reset package")
      io.newLine()
    elif inp != 'no':
      io.println(io.FailList(), "invalid input, skipped")
      continue

  try:
    io.printAsync(io.Empty(), "merging branches..")
    pkg.mergeRepo(repo, "origin/{}".format(repo.active_branch))

  except git.CommandError:
    if package_clear:
      io.println(io.Warn(), package, "pull from origin remote:")
      package_clear = False
    io.println(io.WarnList(), "done, found merge conflict")
    continue

  if package_clear:
    io.println(io.Warn(), package, "pull from origin remote:")
    package_clear = False
  io.println(io.SuccessList(), "done, merged", io.PluralCount('commit', len(commits_behind)))
  continue

if not all_clear:
  io.println(io.Warn(), "done, not all clear")
  io.exitSuccess()

io.println(io.Success(), "done, all clear")
io.exitSuccess()