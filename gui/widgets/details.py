
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
    PADDING = 10
    LINE_HEIGHT = 16

    ICON_MARGIN = 70

    def __init__(self, *args):
        self.font = pygame.font.Font(data.get_font_path('font.ttf'), 12)
        super(DetailsWidget, self).__init__(*args)
        self.line_shadow = utils.make_shadow(self.width, self.LINE_HEIGHT)
        self.game_state = None
        self.position = None

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

        # "Drawing cursor" position. Remove the need to use magic numbers for
        # positions in the code below.
        cur_y = 0

        rcolumn_x = self.ICON_MARGIN + self.PADDING

        # Left side:
        # Display an icon for the kind of cell, and its name as a legend on the
        # bottom side.
        tile_type_name = game.CELL_TYPES[cell.type]
        self.surface.blit(
            data.get_player_tile(cell.type, self.game_state, cell.player),
            (
                (self.ICON_MARGIN - data.TILE_WIDTH) // 2,
                (self.LINE_HEIGHT * 3 - data.TILE_HEIGHT) // 2
            )
        )

        # Right side:
        # Display the cell position on the first line, then its owner (if any)
        # on a second line.
        text = self.font.render(game.CELL_NAMES[cell.type], True, utils.WHITE)
        text_w, text_h = text.get_size()
        self.surface.blit(text, (rcolumn_x, cur_y))
        cur_y += text_h

        text = self.font.render(
            u'Position : (%d, %d)' % (x, y),
            True, utils.WHITE
        )
        self.surface.blit(text, (rcolumn_x, cur_y))
        cur_y += self.LINE_HEIGHT

        if cell.player != game.NO_PLAYER:
            owner = self.game_state.players[cell.player].name
            color = data.get_player_color(self.game_state, cell.player)
        else:
            owner = u'Personne'
            color = utils.DARK_GREY
        text = self.font.render(owner, True, color)
        self.surface.blit(text, (rcolumn_x, cur_y))
        cur_y += self.LINE_HEIGHT + self.PADDING

        # List of statistics to display.
        Row = namedtuple('Row', 'label icon number')
        rows = []

        if cell.type == case_info.CASE_TOURELLE:
            tower = cell.towers[0]
            rows.extend([
                Row(u'Vie',     'life',     tower.life),
                Row(u'Portée',  'range',    tower.scope),
                Row(u'Attaque', 'attack',   tower.attack),
            ])

        if cell.wizards > 0:
            rows.append(Row(u'Sorciers', 'wizard_gui', cell.wizards))

        for row in rows:
            # Display:
            #   [padding] [Name] [column separation] [icon] [number]
            label = self.font.render(row.label, True, utils.GREY)
            number = self.font.render(str(row.number), True, utils.WHITE)

            label_w, _  = label.get_size()
            label_x     = rcolumn_x - 2 * self.PADDING - label_w
            icon_x      = rcolumn_x
            number_x    = icon_x + data.GUI_ICON_WIDTH + self.PADDING

            self.surface.blit(label, (label_x, cur_y))
            if row.icon:
                icon = data.gui_icons[row.icon]
                icon_y = cur_y + (self.LINE_HEIGHT - data.GUI_ICON_HEIGHT) // 2
                self.surface.blit(icon, (icon_x, icon_y))
            self.surface.blit(number, (number_x, cur_y))

            cur_y += self.LINE_HEIGHT

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
