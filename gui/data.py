import os.path
import re

import pygame

from api import *
import game
import utils


TILE_WIDTH = 25
TILE_HEIGHT = 25
TILE_OVERLAY = 0
GUI_ICON_WIDTH = 12
GUI_ICON_HEIGHT = 12
ICON_WIDTH = 32
ICON_HEIGHT = 32


TILE_SHADOW_ALPHA = 128


NO_PLAYER_COLOR = (128, 128, 128)
PLAYER_COLORS = [
    (64, 128, 192),
    (192, 64, 64),
    (48, 192, 64),
    (234, 192, 64),
]

ColoredImages = namedtuple('ColoredImages', 'no_player players')

images_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'data')

def get_image_path(name):
    return os.path.join(images_dir, name)

def get_images(names):
    result = {}
    for name in names:
        path = get_image_path('{}.png'.format(name))
        img = pygame.image.load(path).convert_alpha()
        result[name] = img
    return result

def load_images():
    global gui_icons, tiles
    global towers, bases, fontains, artefacts, wizards
    global icon_shadow

    gui_icons = get_images(
        'score magic simple tower_icon'
        ' phase-construction'
        ' phase-move'
        ' phase-shoot'
        ' phase-siege'
        ' wizard_gui'
        ' life range'
        ''.split())
    gui_icons['attack'] = gui_icons['phase-shoot']
    tiles = get_images('simple tower fontain artefact base wizard'.split())

    towers    = get_colored_images(tiles['tower'])
    bases     = get_colored_images(tiles['base'])
    fontains  = get_colored_images(tiles['fontain'])
    artefacts = get_colored_images(tiles['artefact'])
    wizards   = get_colored_images(tiles['wizard'])

    icon_shadow = utils.make_shadow(ICON_WIDTH, ICON_HEIGHT, alpha=128)

def get_font_path(name):
    return os.path.join(images_dir, name)

def get_player_image(images, game_state, player_id):
    if player_id == game.NO_PLAYER:
        return images.no_player
    else:
        rank = game_state.player_id_to_rank[player_id]
        return images.players[rank]

def get_player_tile(cell_type, game_state, player_id):
    if cell_type == case_info.CASE_SIMPLE:
        return tiles[game.CELL_TYPES[cell_type]]

    else:
        # If this cell *is* associated to some player, then the cell must be
        # one of these:
        return get_player_image(
            {
                case_info.CASE_TOURELLE: towers,
                case_info.CASE_BASE:     bases,
                case_info.CASE_FONTAINE: fontains,
                case_info.CASE_ARTEFACT: artefacts,
            }[cell_type],
            game_state,
            player_id
        )

def get_player_color(game_state, player_id):
    rank = game_state.player_id_to_rank[player_id]
    return (
        NO_PLAYER_COLOR
        if player_id == game.NO_PLAYER else
        PLAYER_COLORS[rank]
    )

def get_colored_images(pattern):
    return ColoredImages(
        utils.set_color(pattern, NO_PLAYER_COLOR),
        [
            utils.set_color(pattern, color)
            for color in PLAYER_COLORS
        ]
    )
