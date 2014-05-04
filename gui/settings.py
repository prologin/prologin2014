
import optparse

import utils

parser = optparse.OptionParser()
parser.add_option(
    '-w', '--width', dest='width', type='int', default=800,
    help='Set the width of the GUI'
)
parser.add_option(
    '-H', '--height', dest='height', type='int', default=600,
    help='Set the height of the GUI'
)

def load(argv):
    global options, args
    options, args = parser.parse_args(argv)

    options.width = utils.set_between(options.width, lower=500)
    options.height = utils.set_between(options.height, lower=400)
