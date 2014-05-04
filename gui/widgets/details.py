
from collections import namedtuple

import pygame

from api import *

import data
import game
import utils
from widgets.scrolled import ScrolledWidget
from widgets.scrolled import BaseWidget

# TODO: Stats Aggregate
# display details on the map
class DetailsWidget(ScrolledWidget):
    PADDING = 8
    LINE_HEIGHT = 55

    def __init__(self, *args):
        self.font = pygame.font.Font(data.get_font_path('font.ttf'), 12)
        super(DetailsWidget, self).__init__(*args)
        self.line_shadow = utils.make_shadow(self.width, self.LINE_HEIGHT)
        self.game_state = None
        self.position = None

    def plug(self, widgets):
        self.map_widget = widgets['map']

    def update_game(self, game):
        self.game_state = game
        if self.position:
            self.update_position(*self.position)

    # display details of a cell
    def _display_cell(self, x, y, cell):
        # Left side:
        # Display an icon for the kind of cell, and its name on top of it.
        tile_y = 0 * (self.LINE_HEIGHT * 2 - data.TILE_HEIGHT) / 2
        # recuperate right image and 'blit' it
        self.list_surface.blit(
            data.tiles[
                {
                    case_info.CASE_SIMPLE:        'simple',
                    case_info.CASE_TOURELLE:      'tower',
                    case_info.CASE_BASE:          'base',
                    case_info.CASE_FONTAINE:      'fontain',
                    case_info.CASE_ARTEFACT:      'artefact',
                }[cell.type]
            ], (0, tile_y)
        )

        text = utils.make_bordered_text(
                game.CELL_TYPES[cell.type].capitalize(),
                self.font
        )

        text_w, text_h = text.get_size()

        # Blit text coresponding to the cell type
        self.list_surface.blit(
            text,
            (
                (data.TILE_WIDTH - text_w) / 2,
                tile_y + data.TILE_HEIGHT - text_h
            )
        )

        # First line:
        # Display position
        text = self.font.render(
                u'Position : (%d, %d)' % (x, y),
                True, utils.WHITE
        )
        self.list_surface.blit(
            text,
            (data.TILE_WIDTH + self.PADDING, 0)
        )

        # Second line:
        # Owner of the cell
        second_line_y = text.get_size()[1] + self.PADDING

        if cell.player != game.NO_PLAYER:
            owner = self.game_state.players[cell.player].name
            color = data.get_player_color(self.game_state, cell.player)
        else:
            owner = 'Personne'
            color = utils.DARK_GREY

        text = self.font.render(owner, True, color)
        self.list_surface.blit(
            text, (data.TILE_WIDTH + self.PADDING, second_line_y)
        )

        # Third line:
        # Number of wizards (of the owner)
        third_line_y = second_line_y + text.get_size()[1] + self.PADDING

        nb_wizards = cell.wizards

        text = self.font.render(nb_wizards, True, utils.WHITE)
        self.list_surface.blit(
            text, (data.TILE_WIDTH + self.PADDING, third_line_y)
        )

        # TODO: tower ?

#    def _display_player_stats(self, id, stats, entry_y):
#        player_label = self.font.render(
#            self.game_state.players[id].name,
#            True, data.get_player_color(self.game_state, id)
#        )
#        if not any(stat for _, stat in stats):
#            player_label.blit(self.line_shadow, (0, 0))
#
#        self.list_surface.blit(
#            player_label,
#            (self.PADDING, entry_y)
#        )


#    def _display_tower(self, tower, i):
#        top_shift = i * self.LINE_HEIGHT


    def handle_click(self, x, y, but1, but2, but3):
        result = super(DetailsWidget, self).handle_click(
            x, y, but1, but2, but3
        )
        if result:
            return True
        # TODO: selecting a boat -> draw its way
        return False
        if not self.position:
            return False

        item = self.is_click_inside(x, y)
        if not item:
            return False
        if not but1:
            return True
        item -= 1
        units = self.game_state.map_units[self.position[1]][self.position[0]]
        if item < len(units):
            self.update_position(self.position[0], self.position[1], item)
        else:
            return False
