import os.path
import re

import pygame

import api
import utils


TILE_WIDTH = 10
TILE_HEIGHT = 10
TILE_OVERLAY = 0
GUI_ICON_WIDTH = 12
GUI_ICON_HEIGHT = 12
ICON_WIDTH = 32
ICON_HEIGHT = 32

tiles = None

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
    global tiles, gui_icons, icon_shadow#, icons, settlement, icon_shadow
    gui_icons = get_images(
        'score magic'
        ' phase-construction'
        ' phase-move'
        ' phase-shoot'
        ' phase-siege'
        ''.split())
    tiles = get_images('simple tower fontain artefact base'.split())
    # TODO: Icons ?
    #icons = get_images('caravelle galion gold'.split())
    #settlement = get_colored_images(tiles['settlement'])

    icon_shadow = utils.make_shadow(ICON_WIDTH, ICON_HEIGHT, alpha=128)

def get_font_path(name):
    return os.path.join(images_dir, name)


PLAYER_COLORS = [
    (0, 128, 192),
    (192, 0, 0),
    (0, 192, 0),
    (234, 192, 0),
]

def get_player_color(game_state, player_id):
    player_order = game_state.player_id_to_order[player_id]
    return PLAYER_COLORS[player_order]

def get_colored_images(pattern):
    return [
        utils.set_color(pattern, color)
        for color in PLAYER_COLORS
    ]
