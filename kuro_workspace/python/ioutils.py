import os
import sys
import threading
import termios
import time
import tty

__temp_file = open('/tmp/kuro.sh', 'w')

def printShell(command):
  global __temp_file
  __temp_file.write(command + '\n')

def runCommand(command):
  if os.system(command) != 0:
    return False

  return True

def runCommandMuted(command):
  return runCommand("{} > /dev/null 2>&1".format(command))

def exitSuccess():
  sys.exit(0)

def exitFail():
  sys.exit(1)

def newLine():
  sys.stdout.write('\n')
  sys.stdout.flush()

def backspace():
  sys.stdout.write('\b')
  sys.stdout.flush()

def resetLine():
  sys.stdout.write('\r')
  sys.stdout.write('\033[J')

def saveCursor():
  sys.stdout.write('\033[s')

def loadCursor():
  sys.stdout.write('\033[u')

def printStream(*args):
  first = True
  for arg in args:
    if not first:
      sys.stdout.write(" ")
    else:
      first = False
    sys.stdout.write(str(arg))
  sys.stdout.flush()

__print_async_stop = True
__threads = []

def printAsyncDone():
  global __print_async_stop
  global __threads

  while len(__threads) > 0:
    __print_async_stop = True
    if not __threads[0].is_alive():
      del __threads[0]

def __printAsyncCallback(interval, *args):
  global __print_async_stop
  global __threads

  while not __print_async_stop:
    loadCursor()
    resetLine()
    printStream(*args)
    time.sleep(interval)

def printAsyncInterval(interval, *args):
  global __print_async_stop

  printAsyncDone()
  saveCursor()

  __print_async_stop = False

  thread = threading.Thread(target=__printAsyncCallback, args=(interval, *args))
  thread.start()

  __threads.append(thread)

def printAsync(*args):
  printAsyncInterval(0.10, *args)

def println(*args):
  printAsyncDone()

  resetLine()
  printStream(*args)
  newLine()

def scanln(*args):
  printAsyncDone()

  string = ""
  ch = ' '

  fd = sys.stdin.fileno()
  old_settings = termios.tcgetattr(fd)
  while ord(ch) != 13:

    resetLine()
    printStream(*args, string)

    try:
      tty.setraw(sys.stdin.fileno())
      ch = sys.stdin.read(1)
    finally:
      termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)

    if ord(ch) == 127:
      if len(string) > 0:
        string = string[:-1]
    elif ord(ch) == 3:
      raise KeyboardInterrupt
    elif ord(ch) >= 32:
      string += ch

  termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)

  return string

def scanAnyKey(*args):
  printAsyncDone()

  resetLine()
  printStream(*args)

  fd = sys.stdin.fileno()
  old_settings = termios.tcgetattr(fd)

  ch = '\n'
  try:
    tty.setraw(sys.stdin.fileno())
    ch = sys.stdin.read(1)
  finally:
    termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)

  if ord(ch) == 3:
    raise KeyboardInterrupt

class Progress:

  chars = ['\u28bf', '\u28fb', '\u28fd', '\u28fe', '\u28f7', '\u28ef', '\u28df', '\u287f']
  pos = 0

  def __str__(self):
    character = Progress.chars[Progress.pos]
    Progress.pos  = (Progress.pos + 1) % len(Progress.chars)
    return character

  def __len__(self):
    return len(Progress.chars[Progress.pos])

class Ref:

  def __init__(self, *refs):
    self.refs = refs

  def __str__(self):
    string = ""
    first = True

    for ref in self.refs:
      if not first:
        string += " "
      else:
        first = False
      string += str(ref)

    return string

  def __len__(self):
    length = 0
    for ref in self.refs:
      length += len(ref)
    return length

class Color(Ref):

  def __init__(self, color, *refs):
    Ref.__init__(self, *refs)
    self.start = color
    self.end = '\033[39m'
    self.__checkRef(self.refs)

  def __str__(self):
    return self.start + Ref.__str__(self) + self.end

  def len(self):
    return Ref.__len__(self)

  def __checkRef(self, refs):
    if issubclass(type(refs), Ref):
      for ref in refs:
        self.__checkRef(ref)

    if issubclass(type(refs), Color):
      refs.end = self.start

class Black(Color):

  def __init__(self, *refs):
    Color.__init__(self, '\033[30m', *refs)

class Red(Color):

  def __init__(self, *refs):
    Color.__init__(self, '\033[31m', *refs)

class Green(Color):

  def __init__(self, *refs):
    Color.__init__(self, '\033[32m', *refs)

class Yellow(Color):

  def __init__(self, *refs):
    Color.__init__(self, '\033[33m', *refs)

class Blue(Color):

  def __init__(self, *refs):
    Color.__init__(self, '\033[34m', *refs)

class Purple(Color):

  def __init__(self, *refs):
    Color.__init__(self, '\033[35m', *refs)

class Cyan(Color):

  def __init__(self, *refs):
    Color.__init__(self, '\033[36m', *refs)

class White(Color):

  def __init__(self, *refs):
    Color.__init__(self, '\033[37m', *refs)

class Style(Ref):

  def __init__(self, style, *refs):
    Ref.__init__(self, *refs)
    self.start = style
    self.end = '\033[0m'
    self.__checkRef(self.refs)

  def __str__(self):
    return self.start + Ref.__str__(self) + self.end

  def len(self):
    return Ref.__len__(self)

  def __checkRef(self, refs):
    if issubclass(type(refs), Ref):
      for ref in refs:
        self.__checkRef(ref)

    if issubclass(type(refs), Style):
      refs.end += self.start

class Normal(Style):

  def __init__(self, *refs):
    Style.__init__(self, '\033[0m', *refs)

class Bold(Style):

  def __init__(self, *refs):
    Style.__init__(self, '\033[1m', *refs)

class Dim(Style):

  def __init__(self, *refs):
    Style.__init__(self, '\033[2m', *refs)

class Italic(Style):

  def __init__(self, *refs):
    Style.__init__(self, '\033[3m', *refs)

class Underline(Style):

  def __init__(self, *refs):
    Style.__init__(self, '\033[4m', *refs)

class Blink(Style):

  def __init__(self, *refs):
    Style.__init__(self, '\033[5m', *refs)

class Inverted(Style):

  def __init__(self, *refs):
    Style.__init__(self, '\033[7m', *refs)

class Hidden(Style):

  def __init__(self, *refs):
    Style.__init__(self, '\033[8m', *refs)

class Empty(Ref):

  def __init__(self):
    Ref.__init__(self, White("[ ]"))

class Success(Ref):

  def __init__(self):
    Ref.__init__(self, Green("[\u2713]"))

class Fail(Ref):

  def __init__(self):
    Ref.__init__(self, Red("[\u2717]"))

class Warn(Ref):

  def __init__(self):
    Ref.__init__(self, Yellow("[\u0021]"))

class EmptyList(Ref):

  def __init__(self):
    Ref.__init__(self, White("    -"))

class SuccessList(Ref):

  def __init__(self):
    Ref.__init__(self, Green("    \u2713"))

class FailList(Ref):

  def __init__(self):
    Ref.__init__(self, Red("    \u2717"))


class WarnList(Ref):

  def __init__(self):
    Ref.__init__(self, Yellow("    \u0021"))

class Plural(Ref):

  def __init__(self, ref, count):
    if count > 1:
      Ref.__init__(self, ref, "\bs")
    else:
      Ref.__init__(self, ref)

class PluralCount(Plural):
  def __init__(self, ref, count):
    Plural.__init__(self, Ref(count, ref), count)