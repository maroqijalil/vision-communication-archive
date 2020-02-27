import git
import os

workspace_path = os.environ['KURO_PATH']
packages_path = os.path.join(workspace_path, 'src')

ssh_path = "git@gitlab.com:ICHIRO-ITS"

packages = [
  'kuro_cmake',
  'kuro_dynamixel',
  'kuro_head',
  'kuro_interface',
  'kuro_motion',
  'kuro_robot',
  'kuro_simulation',
  'kuro_utility',
  'kuro_vision',
  'kuro_walk'
]

def packageIsDir(package):
  return os.path.isdir(os.path.join(packages_path, package))

def packageIsGitRepository(package):
  return os.path.isdir(os.path.join(packages_path, package, '.git'))

def getPackageRepo(package):
  return git.Repo(os.path.join(packages_path, package))

def repoIsNotClean(repo):
  return repo.is_dirty() or len(repo.untracked_files) > 0

def resetRepo(repo):
  repo.git.reset('--hard')
  repo.git.clean('-xdf')
  return True

def mergeRepo(repo, target):
  repo.git.merge(target, repo.active_branch)
  return True

def getCommitsBehind(repo):
  branch = repo.active_branch
  return [item for item in repo.iter_commits("{}..origin/{}".format(branch, branch))]

def getCommitsAhead(repo):
  branch = repo.active_branch
  return [item for item in repo.iter_commits("origin/{}..{}".format(branch, branch))]

def getUncommittedChanges(repo):
  try:
    uncommitted_changes = [ item.a_path for item in repo.index.diff('HEAD') ]
  except git.BadName:
    uncommitted_changes = [ item[0] for item in repo.index.entries ]
  return uncommitted_changes

def masterCloneRepo(package):
  return cloneRepo(package, 'master')

def cloneRepo(package, on_branch):
  package_link = package + '.git'
  return git.Repo.clone_from(os.path.join(ssh_path, package_link), os.path.join(packages_path, package), branch=on_branch)

def cloneNewRepoFile(package, on_branch):
  package_link = package + '.git'
  git.Git(os.path.join(packages_path)).clone(os.path.join(ssh_path, package_link), branch=on_branch)
