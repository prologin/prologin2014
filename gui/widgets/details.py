
from collections import namedtuple

import pygame

from api import *

import data
import game
import utils
from widgets.scrolled import ScrolledWidget
from widgets.base import BaseWidget

# display details on the map
class DetailsWidget(BaseWidget):
    PADDING = 8
    LINE_HEIGHT = 70

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

    def update_position(self, x, y, selection=None):
        if not self.game_state:
            return
        if (
            x < 0 or self.game_state.map_width <= x or
            y < 0 or self.game_state.map_height <= y
        ):
            self.surface = None
            #self.scroll(0)
            return

        self.position = (x, y)
        self.selection = selection
        cell = self.game_state.cells[y][x]
        #self.set_list_length(1 + len(self.game_state.players))
        self._display_cell(x, y, cell)

    # display details of a cell
    def _display_cell(self, x, y, cell):
        # clean
        self.surface.fill(utils.BLACK)
        # Left side:
        # Display an icon for the kind of cell, and its name on top of it.
        tile_y = 0 * (self.LINE_HEIGHT * 2 - data.TILE_HEIGHT) / 2
        # recuperate right image and 'blit' it
        self.surface.blit(
            data.tiles[
                {
                    case_info.CASE_SIMPLE:        'simple_big',
                    case_info.CASE_TOURELLE:      'tower',
                    case_info.CASE_BASE:          'base_big',
                    case_info.CASE_FONTAINE:      'fontain_big',
                    case_info.CASE_ARTEFACT:      'artefact_big',
                }[cell.type]
            ], (0, tile_y)
        )

        text = utils.make_bordered_text(
                game.CELL_TYPES[cell.type].capitalize(),
                self.font
        )

        text_w, text_h = text.get_size()

        # Blit text coresponding to the cell type
        self.surface.blit(
            text,
            (
                (data.TILE_WIDTH - text_w) / 2 + 20,
                tile_y + 2 * data.TILE_HEIGHT - text_h + 10
            )
        )

        # First line:
        # Display position
        text = self.font.render(
                u'Position : (%d, %d)' % (x, y),
                True, utils.WHITE
        )
        self.surface.blit(
            text,
            (data.TILE_WIDTH + 2 + self.PADDING * 6,
                0)
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
        self.surface.blit(
            text, (data.TILE_WIDTH + self.PADDING * 6, second_line_y)
        )

        # third line
        third_line_y = second_line_y + text.get_size()[1] + self.PADDING

        # towers
        if (cell.type == 1):
            text = self.font.render(
                    u'Scope: %d, Life: %d, Attack: %d' %
                    (cell.towers[0].scope,
                    cell.towers[0].life,
                    cell.towers[0].attack),
                    False,
                    utils.WHITE)
            text_w, text_h = text.get_size()

            self.surface.blit(
                text,
                (
                    data.TILE_WIDTH + self.PADDING * 6,
                    third_line_y
                )

            )



#    def _display_tower(self, tower, i):
#        top_shift = i * self.LINE_HEIGHT


    def handle_click(self, x, y, but1, but2, but3):
        result = super(DetailsWidget, self).handle_click(
            x, y, but1, but2, but3
        )
        if result:
            return True
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
