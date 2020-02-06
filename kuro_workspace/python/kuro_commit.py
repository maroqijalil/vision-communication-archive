import ioutils as io
import pkgutils as pkg

import argparse
import datetime
import os

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument('package', nargs='?', default="")
parser.add_argument('message', nargs='?', default="")
parser.add_argument('residuals', nargs='*', default=[ ])
parser.add_argument('command')
parser.add_argument('-h', '--help', action='store_true')
args = parser.parse_args()

if args.package == 'all' or len(args.residuals) > 0 or args.help:
  io.println(io.Yellow("usage:"), args.package, "commit [message] [-h]")
  io.exitFail()

io.printAsync(io.Empty(), "checking package..", io.Progress())
if not pkg.packageIsDir(args.package):
  io.println(io.Fail(), args.package, "does not exist")
  io.exitFail()

if not pkg.packageIsGitRepository(args.package):
  io.println(io.Fail(), args.package, "is not a git repository")
  io.exitFail()

repo = pkg.getPackageRepo(args.package)

io.printAsync(io.Empty(), "checking last commit date..", io.Progress())

try:
  committed_time = datetime.datetime.fromtimestamp(repo.head.commit.committed_date)
except ValueError:
  committed_time = datetime.datetime.now()

current_time = datetime.datetime.now()
if current_time < committed_time:
  io.println(io.Fail(), "current date and time are behind last committed date and time")
  io.exitFail()

io.printAsync(io.Empty(), "staging files..", io.Progress())
repo.git.add('--all')

uncommitted_changes = pkg.getUncommittedChanges(repo)
if len(uncommitted_changes) < 1:
  io.println(io.Success(), "nothing to be committed")
  io.exitSuccess()

io.println(io.Warn(), "changes to be committed:")
for uncommitted_change in uncommitted_changes:
  io.println(io.WarnList(), uncommitted_change)
io.newLine()

io.printAsync(io.Empty(), "checking author info..", io.Progress())

author_name = ""
author_email = ""

try:
  author_name = os.environ['KURO_AUTHOR_NAME']
  author_email = os.environ['KURO_AUTHOR_EMAIL']

  if len(author_name) < 1 or len(author_email) < 1:
    raise KeyError

except KeyError:
  io.println(io.Warn(), "could not load author info")

  while len(author_name) < 1:
    author_name = io.scanln(io.WarnList(), "input author name:")
    io.newLine()

    if len(author_name) < 1:
      io.println(io.FailList(), "author name cannot be empty")

  while len(author_email) < 1:
    author_email = io.scanln(io.WarnList(), "input author email:")
    io.newLine()

    if len(author_email) < 1:
      io.println(io.FailList(), "author email cannot be empty")

  io.printShell("export KURO_AUTHOR_NAME='{}'".format(author_name))
  io.printShell("export KURO_AUTHOR_EMAIL='{}'".format(author_email))

author = "{} <{}>".format(author_name, author_email)
io.println(io.Success(), "author:", author)

io.printAsync(io.Empty(), "input message..", io.Progress())

commit_message = args.message
if len(commit_message) < 1:
  io.println(io.Warn(), "no commit message provided")
  while len(commit_message) < 1:
    commit_message = io.scanln(io.WarnList(), "input commit message:")
    io.newLine()

    if len(commit_message) < 1:
      io.println(io.FailList(), "commit message cannot be empty")

io.println(io.Success(), "commit message:", commit_message)

io.printAsync(io.Empty(), "commiting changes..", io.Progress())
repo.git.commit('-am', commit_message, author=author)

io.println(io.Success(), "done, changes committed")
io.exitSuccess()