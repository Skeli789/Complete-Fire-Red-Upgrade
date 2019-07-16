#!/usr/bin/env python3

from datetime import datetime
from glob import glob
import hashlib
import itertools
import os
from pathlib import Path
import subprocess
import sys
from string import StringFileConverter

PathVar = os.environ.get('Path')
Paths = PathVar.split(';')
PATH = ''
for candidatePath in Paths:
    if 'devkitARM' in candidatePath:
        PATH = candidatePath
        break
if PATH == '':
    print('DevKit does not exist in your Path variable.\nChecking default location.')
    PATH = 'C://devkitPro//devkitARM//bin'
    if os.path.isdir(PATH) is False:
        print('...\nDevkit not found.')
        sys.exit(1)
    else:
        print('Devkit found.')

PREFIX = '/arm-none-eabi-'
AS = (PATH + PREFIX + 'as')
CC = (PATH + PREFIX + 'gcc')
LD = (PATH + PREFIX + 'ld')
GR = 'deps/grit.exe'
ARP = 'armips'
OBJCOPY = (PATH + PREFIX + 'objcopy')
SRC = './src'
GRAPHICS = './graphics'
ASSEMBLY = './assembly'
STRINGS = './strings'
BUILD = './build'
IMAGES = './Images'
ASFLAGS = ['-mthumb', '-I', ASSEMBLY]
LDFLAGS = ['BPRE.ld', '-T', 'linker.ld']
CFLAGS = ['-mthumb', '-mno-thumb-interwork', '-mcpu=arm7tdmi', '-mtune=arm7tdmi',
          '-mno-long-calls', '-march=armv4t', '-Wall', '-Wextra', '-Os', '-fira-loop-pressure', '-fipa-pta']


class Master:
    @staticmethod
    def init():
        Master.printedCompilingImages = False

    @staticmethod
    def printCompilingImages():
        if not Master.printedCompilingImages:
            # Used to tell the script whether or not the string 'Compiling Images' has been printed
            Master.printedCompilingImages = True
            print('Compiling Images')


def RunCommand(cmd: [str]):
    """Runs the command line command."""
    try:
        subprocess.check_output(cmd)
    except subprocess.CalledProcessError as e:
        print(e.output.decode(), file=sys.stderr)
        sys.exit(1)


def CreateOutputFile(fileName: str, newFileName: str) -> [str, bool]:
    """Helper function to produce object file output."""
    if not os.path.isfile(fileName):
        return [newFileName, False]

    fileExists = os.path.isfile(newFileName)

    # If the object file was created after the file was last modified
    if fileExists and os.path.getmtime(newFileName) > os.path.getmtime(fileName):
        return [newFileName, False]

    return [newFileName, True]


def MakeGeneralOutputFile(fileName: str) -> [str, bool]:
    """Return hash of filename to use as object filename."""
    m = hashlib.md5()
    m.update(fileName.encode())
    newFileName = os.path.join(BUILD, m.hexdigest() + '.o')

    return CreateOutputFile(fileName, newFileName)


def MakeOutputImageFile(fileName: str) -> [str, bool]:
    """Return 'IMG_' + hash of filename to use as object filename."""
    m = hashlib.md5()
    m.update(fileName.encode())
    newFileName = os.path.join(BUILD, 'IMG_' + m.hexdigest() + '.o')

    return CreateOutputFile(fileName, newFileName)


def ProcessAssembly(assemblyFile: str) -> str:
    """Assemble."""
    objectFile, regenerateObjectFile = MakeGeneralOutputFile(assemblyFile)
    if regenerateObjectFile is False:
        return objectFile  # No point in recompiling file

    try:
        print('Assembling %s' % assemblyFile)
        cmd = [AS] + ASFLAGS + ['-c', assemblyFile, '-o', objectFile]
        RunCommand(cmd)

    except FileNotFoundError:
        print('Error! The assembler could not be located.\n'
              + 'Are you sure you set up your path to devkitPro/devkitARM/bin correctly?')
        sys.exit(1)

    return objectFile


def ProcessC(cFile: str) -> str:
    """Compile C."""
    objectFile, regenerateObjectFile = MakeGeneralOutputFile(cFile)
    if regenerateObjectFile is False:
        return objectFile  # No point in recompiling file

    try:
        print('Compiling %s' % cFile)
        cmd = [CC] + CFLAGS + ['-c', cFile, '-o', objectFile]
        RunCommand(cmd)

    except FileNotFoundError:
        print('Error! The C compiler could not be located.\n'
              + 'Are you sure you set up your path to devkitPro/devkitARM/bin correctly?')
        sys.exit(1)

    return objectFile


def ProcessString(stringFile: str) -> str:
    """Build and assemble strings."""
    assemblyFile = stringFile.split('.string')[0] + '.s'
    objectFile = MakeGeneralOutputFile(assemblyFile)[0]
    fileExists = os.path.isfile(objectFile)

    if fileExists and os.path.getmtime(objectFile) > os.path.getmtime(stringFile):
        # If the .o file was created after the string file was last modified
        return objectFile

    print('Building Strings %s' % stringFile)
    StringFileConverter(stringFile)

    cmd = [AS] + ASFLAGS + ['-c', assemblyFile, '-o', objectFile]
    RunCommand(cmd)
    os.remove(assemblyFile)
    return objectFile


def ProcessImage(imageFile: str) -> str:
    """Compile image."""
    if '.bmp' in imageFile:
        assemblyFile = imageFile.split('.bmp')[0] + '.s'
    else:
        assemblyFile = imageFile.split('.png')[0] + '.s'

    namelist = imageFile.split('\\')  # Get path of grit flags
    namelist.pop(len(namelist) - 1)
    flags = ''.join(str(i) + '\\' for i in namelist)
    flags += 'gritflags.txt'

    try:
        with open(flags, 'r') as file:
            line = file.readline()  # Only needs the first line
            cmd = [GR, imageFile] + line.split() + ['-o', assemblyFile]

    except FileNotFoundError:
        print('Error: No gritflags.txt found in directory with ' + imageFile + '.')
        sys.exit(1)

    objectFile = MakeOutputImageFile(assemblyFile)[0]
    fileExists = os.path.isfile(objectFile)

    if fileExists and os.path.getmtime(objectFile) > os.path.getmtime(imageFile):
        # If the .o file was created after the image was last modified
        return objectFile
    else:
        Master.printCompilingImages()
        RunCommand(cmd)

    regenerateObjectFile = MakeOutputImageFile(assemblyFile)[1]
    if regenerateObjectFile is False:
        os.remove(assemblyFile)
        return objectFile  # No point in recompiling file

    cmd = [AS] + ASFLAGS + ['-c', assemblyFile, '-o', objectFile]
    RunCommand(cmd)
    os.remove(assemblyFile)
    return objectFile


def LinkObjects(objects: itertools.chain) -> str:
    """Link objects into one binary."""
    linked = 'build/linked.o'
    cmd = [LD] + LDFLAGS + ['-o', linked] + list(objects)
    RunCommand(cmd)
    return linked


def Objcopy(binary: str):
    """Run the objcopy."""
    cmd = [OBJCOPY, '-O', 'binary', binary, 'build/output.bin']
    RunCommand(cmd)


def RunGlob(globString: str, fn) -> map:
    """Glob recursively and run the processor function on each file in result."""
    if globString == '**/*.png' or globString == '**/*.bmp':  # Search the GRAPHICS location
        directory = GRAPHICS
    elif globString == '**/*.s':
        directory = ASSEMBLY
    elif globString == '**/*.string':
        directory = STRINGS
    else:
        directory = SRC

    if sys.version_info > (3, 4):
        try:
            files = glob(os.path.join(directory, globString), recursive=True)
            return map(fn, files)

        except TypeError:
            print('Error compiling. Please make sure Python has been updated to the latest version.')
            sys.exit(1)
    else:
        files = Path(directory).glob(globString)
        return map(fn, map(str, files))


def main():
    Master.init()
    startTime = datetime.now()
    globs = {
            '**/*.s': ProcessAssembly,
            '**/*.c': ProcessC,
            '**/*.string': ProcessString,
            '**/*.png': ProcessImage,
            '**/*.bmp': ProcessImage,
    }

    # Create output directory
    try:
        os.makedirs(BUILD)
    except FileExistsError:
        pass

    # Gather source files and process them
    objects = itertools.starmap(RunGlob, globs.items())

    # Link and extract raw binary
    linked = LinkObjects(itertools.chain.from_iterable(objects))
    Objcopy(linked)

    # Build special_inserts.asm
    if not os.path.isfile('build/special_inserts.bin') \
            or os.path.getmtime('build/special_inserts.bin') < os.path.getmtime('special_inserts.asm'):
        print('Assembling special_inserts.asm')
        cmd = [AS] + ASFLAGS + ['-c', 'special_inserts.asm', '-o', 'build/special_inserts.o']
        RunCommand(cmd)

        cmd = [OBJCOPY, '-O', 'binary', 'build/special_inserts.o', 'build/special_inserts.bin']
        RunCommand(cmd)

    print('Built in ' + str(datetime.now() - startTime) + '.')


if __name__ == '__main__':
    main()
