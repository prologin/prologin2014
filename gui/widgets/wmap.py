from collections import defaultdict

import pygame

from api import *
import data
import game
import utils
from widgets.base import BaseWidget

class MapWidget(BaseWidget):

    DYNAMIC_TILES = {
        case_info.CASE_TOURELLE: 'tower',
        case_info.CASE_BASE:     'base_big',
        case_info.CASE_FONTAINE: 'fontain_big',
        case_info.CASE_ARTEFACT: 'artefact_big',
    }

    # Size of the text used to represent the number of wizards on a cell.
    FONT_SIZE = 7

    def __init__(self, *args):
        super(MapWidget, self).__init__(*args)

        # initializations
        self.game_state = None
        self.position = None
        self.static_map_surface = None
        self.map_surface = None
        self.font = pygame.font.Font(
            data.get_font_path('font.ttf'),
            self.FONT_SIZE
        )

        self.cells_width = self.width / data.TILE_WIDTH
        self.cells_height = (self.height - data.TILE_OVERLAY) / (data.TILE_HEIGHT - data.TILE_OVERLAY)
        self.center = (self.cells_width / 2, self.cells_height / 2)


    def plug(self, widgets):
        self.details_widget = widgets['details']

    def handle_view_click(self, x, y, but1, but2, but3, absolute=False):
        x = x // data.TILE_WIDTH
        y = (y - data.TILE_OVERLAY) // (data.TILE_HEIGHT - data.TILE_OVERLAY)
        if not absolute:
            x += self.position[0]
            y += self.position[1]

        if but1:
            self.details_widget.update_position(x, y)
            #self.update_subjective()
        elif but3:
            self.update_display((x - self.center[0], y - self.center[1]))

    def handle_click(self, x, y, but1, but2, but3):
        coords = self.is_click_inside(x, y)
        if not coords:
            return False
        if self.game_state is None:
            return True
        self.handle_view_click(coords[0], coords[1], but1, but2, but3)
        return True

    def update_subjective(self):
        # TODO: determine if there is a subjective view in the first place. If
        # there is, implement it there!
        pass

    def make_map_surface(self, game_state):
        surf_size = (
            game_state.map_width * data.TILE_WIDTH,
            game_state.map_height * (data.TILE_HEIGHT - data.TILE_OVERLAY) + data.TILE_OVERLAY
        )
        return utils.make_surface(*surf_size)

    def update_static_map(self, game_state):
        self.static_map_surface = self.make_map_surface(game_state)
        simple_pix = data.tiles['simple_big']
        coord_y = 0
        for y, row in enumerate(game_state.cells):
            coord_x = 0
            for x, cell in enumerate(row):
                coords = (coord_x, coord_y)
                self.static_map_surface.blit(simple_pix, coords)
                coord_x += data.TILE_WIDTH
            coord_y += data.TILE_HEIGHT - data.TILE_OVERLAY

    def update_game(self, game_state):
        self.game_state = game_state

        self.surface.fill(utils.BLACK)

        if self.game_state is None:
            return

        if not self.position:
            self.map_surface = self.make_map_surface(game_state)
            self.position = (0, 0)
        else:
            self.map_surface.fill((0, 0, 0, 0))
            pass
        if self.static_map_surface is None:
            self.update_static_map(game_state)

        players = self.game_state.players

        coord_y = 0
        for y, row in enumerate(game_state.cells):
            coord_x = 0
            for x, cell in enumerate(row):
                coords = (coord_x, coord_y)

                # Add a tile on top of the static one for "dynamic" cells (such
                # as towers).
                try:
                    self.map_surface.blit(
                        data.tiles[self.DYNAMIC_TILES[cell.type]],
                        coords
                    )
                except KeyError:
                    pass

                # Likewise for wizards
                if cell.wizards > 0:
                    self.map_surface.blit(
                        data.wizards[cell.player],
                        coords
                    )
                    count_text = utils.make_bordered_text(
                        str(cell.wizards), self.font,
                        fgcolor=data.get_player_color(game_state, cell.player)
                    )
                    count_width, count_height = count_text.get_size()
                    self.map_surface.blit(count_text, (
                        coord_x + data.TILE_WIDTH - count_width,
                        coord_y
                    ))

                coord_x += data.TILE_WIDTH
            coord_y += data.TILE_HEIGHT - data.TILE_OVERLAY

        view_shift = (
                self.position[0] * data.TILE_WIDTH,
                self.position[1] * (data.TILE_HEIGHT - data.TILE_OVERLAY),
                self.width, self.height
                )

        # update display
        self.surface.fill(utils.BLACK)
        self.surface.blit(self.static_map_surface, (0, 0), view_shift)
        self.surface.blit(self.map_surface, (0, 0), view_shift)

