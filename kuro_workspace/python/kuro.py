import ioutils as io

import argparse
import sys

try:

  command_names = [
    'status     Show the working tree status', 
    'pull       Fetch from and integrate with another repository or a local branch', 
    'push       Update remote refs along with associated objects', 
    'reset      Reset current HEAD to the specified state',
    'build      Build all or a specific package', 
    'clean      Remove the generated directory by the colcon build', 
    'commit     Record changes to the repository', 
    'log        Show commit logs (default 5)', 
    'run        Run ros2 a package specific executable', 
    'view       View a package specific executable', 
    'kill       Kill a package specific executable',
    'clone      Clone a repository via ssh'
  ]

  parser = argparse.ArgumentParser(add_help=False)
  parser.add_argument('package', nargs='?', default='all')
  parser.add_argument('command', nargs='?', default='help')
  parser.add_argument('executable', nargs='*', default='none')
  parser.add_argument('residuals', nargs='*', default=[ ])
  parser.add_argument('-h', '--help', action='store_true')
  args = parser.parse_args()

  def printCommand():
    io.println("command: ")
    for cmd in command_names:
      io.println("  ", cmd, " ")
      if cmd ==command_names[5] and args.package == 'all': 
        break

  for pkg in command_names:
    if args.package == pkg:
      args.command = pkg
      args.package = 'all'
  
  if args.package == 'all':
    
    if args.command == 'status':
      import kuro_status_all

    elif args.command == 'pull':
      import kuro_pull_all

    elif args.command == 'push':
      import kuro_push_all

    elif args.command == 'reset':
      import kuro_reset_all

    elif args.command == 'build':
      import kuro_build
    
    elif args.command == 'clean':
      import kuro_clean

    elif args.command == 'view':
      import kuro_view_all  

    elif args.command == 'kill':
      import kuro_kill_all

    elif args.command == 'clone':
      import kuro_clone_all

    else:
      if args.command == 'run' or args.command == 'log' or args.command == 'commit':
        io.println("for", io.Yellow(args.command) , "command: ")
        io.println(io.Yellow("usage:"), "<package>", args.command, "[-h]")
      else: 
        io.println(io.Yellow("usage:"), "kuro [command] [-h]\n")
        printCommand()

  else:
 
    if args.command == 'status':
      import kuro_status

    elif args.command == 'pull':
      import kuro_pull

    elif args.command == 'push':
      import kuro_push

    elif args.command == 'commit':
      import kuro_commit

    elif args.command == 'log':
      import kuro_log

    elif args.command == 'reset':
      import kuro_reset

    elif args.command == 'build':
      import kuro_build

    elif args.command == 'clean':
      import kuro_clean

    elif args.command == 'run':
      import kuro_run

    elif args.command == 'view':
      import kuro_view

    elif args.command == 'kill':
      import kuro_kill
    
    elif args.command == 'clone':
      import kuro_clone

    else:
      io.println(io.Yellow("usage:"), args.package + " [command] [-h]")
      printCommand()

except KeyboardInterrupt:
  io.println(io.Fail(), "keyboard interrupt, aborted")
  sys.exit(1)
