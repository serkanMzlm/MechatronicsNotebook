#!/usr/bin/python3

import sys
import argparse

metin = "Sets the debug level"

parser = argparse.ArgumentParser()
parser.add_argument('-d', '--debug', dest='level', 
                        type=int, default=0, help=metin)

args = parser.parse_args()
flag = args.level

def debug(id, text):
    if id == flag:
        print(text)
        print("---"*20)

# argparse kullanıldığı için gereksiz
# def get_argv():
#     if len(sys.argv) > 1:
#         _debug = int(sys.argv[1])
#     else:
#         _debug = 0
#     return _debug