#! /usr/bin/env python3
# -*- coding: utf-8 -*-

import gzip
import sys

import pygame
pygame.init()

import logs
import settings
from state_reader import DumpReader
from window import Window

settings.load(sys.argv[1:])
try:
    filename = settings.args[0]
except IndexError:
    logs.write('Error: a file is needed.')

if filename.endswith('.gz'):
    dump_file = gzip.open(filename, 'r')
else:
    dump_file = open(filename, 'r')
state_reader = DumpReader(dump_file)
window = Window(state_reader)
window.run()
