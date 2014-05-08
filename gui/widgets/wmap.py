from collections import defaultdict

import pygame

from api import *
import data
import game
import utils
from widgets.base import BaseWidget

class MapWidget(BaseWidget):

    WIDTH  = TAILLE_TERRAIN * data.TILE_WIDTH
    HEIGHT = TAILLE_TERRAIN * data.TILE_HEIGHT

    DYNAMIC_TILES = {
        case_info.CASE_TOURELLE: 'tower',
        case_info.CASE_BASE:     'base',
        case_info.CASE_FONTAINE: 'fontain',
        case_info.CASE_ARTEFACT: 'artefact',
    }

    # Size of the text used to represent the number of wizards on a cell.
    FONT_SIZE = 12

    def __init__(self, *args):
        super(MapWidget, self).__init__(*args)

        # initializations
        self.game_state = None

        # Position of the "subjective view". This is the coordinate of the
        # tower currently inspected.  Shadow is then displayed on top of out of
        # range cells.
        self.position = None

        self.static_map_surface = None
        self.subjective_surface = None
        self.map_surface = None

        self.display_grid = False
        self.font = pygame.font.Font(
            data.get_font_path('font.ttf'),
            self.FONT_SIZE
        )

        self.cells_width = self.width / data.TILE_WIDTH
        self.cells_height = (
            (self.height - data.TILE_OVERLAY)
            // (data.TILE_HEIGHT - data.TILE_OVERLAY)
        )
        self.center = (self.cells_width / 2, self.cells_height / 2)


    def plug(self, widgets):
        self.details_widget = widgets['details']

    def handle_view_click(self, x, y, but1, but2, but3, absolute=False):
        x = x // data.TILE_WIDTH
        y = (y - data.TILE_OVERLAY) // (data.TILE_HEIGHT - data.TILE_OVERLAY)

        if but1:
            self.details_widget.update_position(x, y)
            self.update_subjective((x, y))
        elif but3:
            self.update_subjective(None)

    def handle_click(self, x, y, but1, but2, but3):
        coords = self.is_click_inside(x, y)
        if not coords:
            return False
        if self.game_state is None:
            return True
        self.handle_view_click(coords[0], coords[1], but1, but2, but3)
        return True

    def switch_grid(self):
        self.display_grid = not self.display_grid
        self.update_static_map()
        self.update_game()

    def update_subjective(self, position):
        # The subjective view is meaningful only for towers. Cancel the
        # subjective view for all other cells.
        cell = (
            self.game_state.cell(*position)
            if position else
            None
        )
        if not cell or cell.type != case_info.CASE_TOURELLE:
            position = None
        else:
            tower = cell.towers[0]

        self.position = position
        self.subjective_surface = self.make_map_surface(self.game_state)
        if self.position:
            shadow_color = utils.BLACK + (data.TILE_SHADOW_ALPHA, )
            for y in range(TAILLE_TERRAIN):
                for x in range(TAILLE_TERRAIN):
                    dx = abs(position[0] - x)
                    dy = abs(position[1] - y)
                    if dx + dy > tower.scope:
                        self.subjective_surface.fill(
                            shadow_color,
                            (
                                x * data.TILE_WIDTH,
                                y * data.TILE_HEIGHT,
                                data.TILE_WIDTH,
                                data.TILE_HEIGHT
                            )
                        )

        self.redraw_all()

    def make_map_surface(self, game_state):
        surf_size = (
            game_state.map_width * data.TILE_WIDTH,
            game_state.map_height * (data.TILE_HEIGHT - data.TILE_OVERLAY) + data.TILE_OVERLAY
        )
        return utils.make_surface(*surf_size)

    def update_static_map(self, game_state=None):
        game_state = self.set_or_get_game_state(game_state)

        # If we have to display a cell grid, prepare a "grid tile", that will
        # be pasted on top of the ground tiles.
        if self.display_grid:
            grid_tile = utils.make_surface(data.TILE_WIDTH, data.TILE_HEIGHT)
            last_x = data.TILE_WIDTH - 1
            last_y = data.TILE_HEIGHT - 1
            # Draw a line at the bottom of the tile
            for x in range(0, data.TILE_WIDTH, 3):
                grid_tile.set_at((x + 0, last_y), utils.BLACK)
            for y in range(0, data.TILE_HEIGHT, 3):
                grid_tile.set_at((last_x, y + 0), utils.BLACK)

        # Create the surface and paste "ground" tiles on it.
        self.static_map_surface = self.make_map_surface(game_state)
        simple_pix = data.tiles['simple']
        coord_y = 0
        for y, row in enumerate(game_state.cells):
            coord_x = 0
            for x, cell in enumerate(row):
                coords = (coord_x, coord_y)
                self.static_map_surface.blit(simple_pix, coords)
                if self.display_grid:
                    self.static_map_surface.blit(grid_tile, coords)
                coord_x += data.TILE_WIDTH
            coord_y += data.TILE_HEIGHT - data.TILE_OVERLAY

    def update_game(self, game_state=None):
        game_state = self.set_or_get_game_state(game_state)
        self.game_state = game_state
        self.update_subjective(self.position)

        self.surface.fill(utils.BLACK)

        if self.game_state is None:
            return

        self.redraw_all()

    def redraw_all(self):
        if self.map_surface is None:
            self.map_surface = self.make_map_surface(self.game_state)
        else:
            self.map_surface.fill((0, 0, 0, 0))

        if self.static_map_surface is None:
            self.update_static_map(self.game_state)

        players = self.game_state.players

        coord_y = 0
        for y, row in enumerate(self.game_state.cells):
            coord_x = 0
            for x, cell in enumerate(row):
                coords = (coord_x, coord_y)

                # Add a tile on top of the static one for "dynamic" cells (such
                # as towers).
                if cell.type != case_info.CASE_SIMPLE:
                    tile = data.get_player_tile(
                        cell.type, self.game_state, cell.player
                    )
                    self.map_surface.blit(tile, coords)

                # Likewise for wizards.
                if cell.wizards > 0:
                    # The wizard tiles should be drawed only on simple cells.
                    # Other tiles (base, fountains, towers, artefacts) already
                    # contain the player color, so don't put too many tiles on
                    # the same cell: only put the wizard count in such cases.
                    if cell.type == case_info.CASE_SIMPLE:
                        wizard_tile = data.get_player_image(
                            data.wizards, self.game_state, cell.player
                        )
                        self.map_surface.blit(wizard_tile, coords)

                    count_text = utils.make_bordered_text(
                        str(cell.wizards), self.font,
                        fgcolor=data.get_player_color(
                            self.game_state, cell.player
                        )
                    )
                    count_width, count_height = count_text.get_size()
                    self.map_surface.blit(count_text, (
                        coord_x + data.TILE_WIDTH - count_width,
                        coord_y
                    ))

                coord_x += data.TILE_WIDTH
            coord_y += data.TILE_HEIGHT - data.TILE_OVERLAY

        # update display
        self.surface.fill(utils.BLACK)
        self.surface.blit(self.static_map_surface, (0, 0))
        self.surface.blit(self.map_surface, (0, 0))
        if self.position:
            self.surface.blit(self.subjective_surface, (0, 0))

    def set_or_get_game_state(self, game_state=None):
        if game_state:
            self.game_state = game_state
        return self.game_state
