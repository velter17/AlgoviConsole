#!/usr/bin/python

import subprocess
import sys
import argparse
import shutil
import os


srcPath = os.path.dirname(os.path.realpath(__file__))
buildPath = srcPath + '/build'

targetOptions = [
    "linux",
    "win"
]

parser = argparse.ArgumentParser(description='Bla-bla build script')
parser.add_argument('-c', '--clean_build', action='store_const', const=True, default=False, help='Make clean build')
parser.add_argument('-d', '--debug', action='store_const', const=True, default=False, help='Debug mode build')
parser.add_argument('-t', '--target', default="linux", choices=targetOptions, help="operation system target = { linux, win }")
parser.add_argument('--with-tests', action='store_const', const=True, default=False, help='Build tests')
parser.add_argument('-j', '--jobs', default="3", help="amount of jobs")
parser.add_argument('-i', '--install', default=False, action='store_const', const=True, help='install')

args = parser.parse_args()

buildType = 'Debug' if args.debug else 'Release'
buildPath = buildPath + '/' + args.target + '-' + buildType
#installPath = srcPath + '/app/' + args.target + '-' + buildType + '/'
installPath = "/opt/algovi/"

if args.clean_build and os.path.exists(buildPath):
    shutil.rmtree(buildPath, True)

cmakeArgs = [
    'cmake',
    srcPath,
    '-DCMAKE_BUILD_TYPE=' + buildType,
    '-DCMAKE_INSTALL_PREFIX=' + installPath,
    '-DBIN_DIR=' + installPath,
    #'-DBLA_TARGET_SYSTEM=' + args.target,
]

#if args.with-tests:
#    cmakeArgs += '-BUILD_TESTS'

if args.target == "win":
    cmakeArgs.append('-G')
    cmakeArgs.append('MinGW Makefiles')

makeArgs = []
if args.target == 'linux':
    makeArgs.append('make')
else:
    makeArgs.append('mingw32-make.exe')
makeArgs.append('-j' + args.jobs)
if args.install:
    makeArgs.append('install')


try:
    os.makedirs(buildPath)
    os.makedirs(installPath)
except:
    pass

print "execute cmake: " + str(cmakeArgs)
if subprocess.call(cmakeArgs, cwd=buildPath) != 0:
    print("Failed to run cmake: " + repr(cmakeArgs))
    sys.exit(1)

if subprocess.call(makeArgs, cwd=buildPath) != 0:
    print("Failed to run make: " + repr(makeArgs))
    sys.exit(1)
else:
    if args.install:
        if not os.path.exists("/usr/local/bin/algovi"):
            os.symlink("/opt/algovi/bin/algovi_start_script.sh", "/usr/local/bin/algovi")
        shutil.copyfile("/opt/algovi/bin/algovi_complation_script.sh", "/etc/bash_completion.d/algovi_complation.sh")
