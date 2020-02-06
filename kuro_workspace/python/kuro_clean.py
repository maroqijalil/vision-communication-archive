import ioutils as io
import pkgutils as pkg

import argparse
import os
import shutil

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument('package', nargs='?', default='none')
parser.add_argument('residuals', nargs='*', default=[ ])
parser.add_argument('command')
parser.add_argument('-h', '--help', action='store_true')
args = parser.parse_args()

if len(args.residuals) > 0 or args.package != 'all'or args.help:
  io.println(io.Yellow("usage:"), "kuro clean [-h]")
  io.exitFail()

directories = ['build', 'install', 'log']
directories_to_clean = []

io.printAsync(io.Empty(), "checking directories..", io.Progress())
for directory in directories:
  if os.path.exists(os.path.join(pkg.workspace_path, directory)):
    directories_to_clean.append(directory)

if len(directories_to_clean) < 1:
  io.println(io.Success(), "already cleaned")
  io.exitSuccess()

string = io.scanln(io.Warn(), "are you sure you want to clean the workspace? (yes/no)")
io.newLine()

if string == 'no':
  io.println(io.Fail(), "cancelled")
  io.exitFail()
elif string != 'yes':
  io.println(io.Fail(), "invalid input, cancelled")
  io.exitFail()

for directory in directories_to_clean:

  io.printAsync(io.Empty(), "checking", directory, "directory..", io.Progress())
  path = os.path.join(pkg.workspace_path, directory)

  if os.path.isdir(path):
    io.printAsync(io.Empty(), "removing", directory, "directory..", io.Progress())
    try:
      shutil.rmtree(path)
    except PermissionError:
      io.println(io.Fail(), "cannot remove", io.Underline(path), "(permission denied)")
      io.exitFail()

    io.println(io.Success(), directory, "directory removed")
    continue

  if os.path.isfile(path):
    io.println(io.Warn(), io.Underline(path), "is a file")

    io.printAsync(io.Empty(), "removing", directory, "file..", io.Progress())
    os.remove(path)

    io.println(io.Success(), directory, "file removed")

    continue

  continue

io.println(io.Success(), "cleaned")
io.exitSuccess()