import os
import sys
import shutil
from typing import Callable, Match

FOREGROUND_BLACK = '\u001b[30m'
FOREGROUND_RED = '\u001b[31m'
FOREGROUND_GREEN = '\u001b[32m'
FOREGROUND_YELLOW = '\u001b[33m'
FOREGROUND_BLUE = '\u001b[34m'
FOREGROUND_MAGENTA = '\u001b[35m'
FOREGROUND_CYAN = '\u001b[36m'
FOREGROUND_WHITE = '\u001b[37m'

BACKGROUND_BLACK = '\u001b[40m'
BACKGROUND_RED = '\u001b[41m'
BACKGROUND_GREEN = '\u001b[42m'
BACKGROUND_YELLOW = '\u001b[43m'
BACKGROUND_BLUE = '\u001b[44m'
BACKGROUND_MAGENTA = '\u001b[45m'
BACKGROUND_CYAN = '\u001b[46m'
BACKGROUND_WHITE = '\u001b[47m'

COLOR_RESET = '\u001b[0m'


def trace(msg: str):
    print(FOREGROUND_GREEN)
    print(msg)
    print(FOREGROUND_WHITE)


def info(msg: str):
    print(FOREGROUND_CYAN)
    print(msg)
    print(FOREGROUND_WHITE)


def error(msg: str):
    print(FOREGROUND_RED)
    print(msg)
    print(FOREGROUND_WHITE)


def clean(root_dir: str, things_to_remove: list[str]):
    trace('cleaning...')
    os.chdir(root_dir)
    for i in things_to_remove:
        if os.path.exists(i):
            if os.path.isdir(i):
                shutil.rmtree(i)
            elif os.path.isfile(i):
                os.remove(i)


def find_files(dir: str, extension: str) -> list[str]:
    ret_files = []
    for subdir, dirs, files in os.walk(dir):
        for file in files:
            if file.endswith(extension):
                ret_files.append(os.path.join(subdir, file))
    return ret_files


def compile_files(build_dir: str, src_dir: str, flags: str, extension: str, func: Callable[[str, str, str], int]):
    trace(f'compiling {extension} files')
    files_to_compile = find_files(src_dir, extension)

    for file_to_compile in files_to_compile:
        os.chdir(build_dir)
        split = os.path.split(os.path.abspath(file_to_compile))
        dir = f'./{split[0].removeprefix(src_dir)}'
        filename = split[1]
        if not os.path.exists(dir):
            os.makedirs(dir)
        os.chdir(dir)
        func(file_to_compile, flags, filename)
    info(f'finished compiling {extension} files!')
