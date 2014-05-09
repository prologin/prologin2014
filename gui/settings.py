import argparse

import utils

parser = argparse.ArgumentParser()
parser.add_argument(
    '-w', '--width', dest='width', type=int, default=800,
    help='Set the width of the GUI'
)
parser.add_argument(
    '-H', '--height', dest='height', type=int, default=600,
    help='Set the height of the GUI'
)

def load(argv):
    global options
    options = parser.parse_args(argv)

    options.width = utils.set_between(options.width, lower=500)
    options.height = utils.set_between(options.height, lower=400)
