import os.path
import re

import pygame

import api
import utils


TILE_WIDTH = 25
TILE_HEIGHT = 25
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
    global tiles, gui_icons, icon_shadow, wizards
    gui_icons = get_images(
        'score magic simple_big tower_icon'
        ' fontain_big artefact_big base_big'
        ' phase-construction'
        ' phase-move'
        ' phase-shoot'
        ' phase-siege'
        ' wizard'
        ''.split())
    tiles = get_images('simple_big tower fontain_big artefact_big base_big wizard'.split())
    wizards = get_colored_images(tiles['wizard'])
    icon_shadow = utils.make_shadow(ICON_WIDTH, ICON_HEIGHT, alpha=128)

def get_font_path(name):
    return os.path.join(images_dir, name)


PLAYER_COLORS = [
    (64, 128, 192),
    (192, 64, 64),
    (48, 192, 64),
    (234, 192, 64),
]

def get_player_image(images, game_state, player_id):
    rank = game_state.player_id_to_rank[player_id]
    return images[rank]

def get_player_color(game_state, player_id):
    rank = game_state.player_id_to_rank[player_id]
    return PLAYER_COLORS[rank]

def get_colored_images(pattern):
    return [
        utils.set_color(pattern, color)
        for color in PLAYER_COLORS
    ]
