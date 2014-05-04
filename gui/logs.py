# -*- coding: utf-8 -*-

def write(text):
    lines = text.split('\n') or ['']
    print ('\x1b[1m[\x1b[31mGUI\x1b[0;1m]\x1b[0m %s' % lines[0])
    for line in lines[1:]:
        print (' ' * 6 + line)
