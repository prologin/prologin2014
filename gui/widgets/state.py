import pygame

import data
import data
import utils
from widgets.base import BaseWidget

# widget describing the state of the player
# FIXME: magic ?
class StateWidget(BaseWidget):
    ICONS_PADDING = 3
    LEFT_MARGIN = data.GUI_ICON_WIDTH + ICONS_PADDING

    TEXT_SIZE = 12
    TEXT_HEIGHT = 15
    LINES = 11

    PHASE_TEXT = {
        'construction': u'construction',
        'move':         u'déplacement',
        'shoot':        u'tir',
        'siege':        u'siège',
    }
    PHASES = 'construction move shoot siege'.split()

    HEIGHT = TEXT_HEIGHT * LINES

    def __init__(self, x, y, width):
        super(StateWidget, self).__init__(x, y, width, self.HEIGHT)
        self.font = pygame.font.Font(data.get_font_path('font.ttf'), self.TEXT_SIZE)
        self.game_state = None
        self.turn = 0
        self.last_turn = None

    def _display_text(self, text, height, color, column_info=None):
        '''
        Display a centered text at a given height, taking into account columns
        if any.
        '''
        if column_info:
            available_width = self.width - self.LEFT_MARGIN
            col_no, cols = column_info
            col_width = int(available_width / cols)
            col_start = self.LEFT_MARGIN + col_width * col_no
        else:
            col_start = 0
            col_width = self.width
        surf = self.font.render(text, True, color)
        surf_w, surf_h = surf.get_size()
        self.surface.blit(surf, (col_start + (col_width - surf_w) / 2, height))

    def _display_scores(self): # (And magic)
        if self.game_state is None:
            return
        players = self.game_state.players
        cols = len(players)
        ids = sorted(players)
        for i, (id, player) in enumerate(utils.iter_dict(players, ids)):
            self._display_text(
                player.name,
                0 * self.TEXT_HEIGHT,
                data.get_player_color(self.game_state, id),
                (i, cols)
            )
            self._display_text(
                str(player.score),
                1 * self.TEXT_HEIGHT,
                utils.WHITE,
                (i, cols)
            )
            self._display_text(
                str(player.magic),
                2 * self.TEXT_HEIGHT,
                utils.WHITE,
                (i, cols)
            )

        # Put icons in a left margin to explain the meaning of these
        # statistics.
        self.surface.blit(
            data.gui_icons['score'],
            (0, 1 * self.TEXT_HEIGHT)
        )
        self.surface.blit(
            data.gui_icons['magic'],
            (0, 2 * self.TEXT_HEIGHT)
        )

    def _display_turn(self):
        self._display_text(
            (
                u'Tour %d / %d' % (self.turn, self.last_turn)
                if self.last_turn else
                u'Tour %d' % self.turn
            ),
            4 * self.TEXT_HEIGHT, utils.WHITE
        )

        self._display_text(
            u'Phase de {}'.format(
                self.PHASE_TEXT.get(self.phase, self.phase)
            ),
            5 * self.TEXT_HEIGHT, utils.WHITE
        )

        # Display icons for the phase
        list_width = (
            (data.GUI_ICON_WIDTH + self.ICONS_PADDING) * len(self.PHASE_TEXT)
            - self.ICONS_PADDING
        )
        list_start = (self.width - list_width) // 2
        for i, phase in enumerate(self.PHASES):
            pos = (
                list_start + i * (data.GUI_ICON_WIDTH + self.ICONS_PADDING),
                (6 * self.TEXT_HEIGHT)
            )
            self.surface.blit(
                data.gui_icons['phase-{}'.format(phase)],
                pos
            )
            # Put a shadow over all icons but the one that represents the
            # current phase.
            if phase != self.phase:
                self.surface.blit(data.icon_shadow, pos)

    def _display_help(self):
        self._display_text(
            u'H: afficher/cacher l’aide', 8 * self.TEXT_HEIGHT, utils.WHITE
        )

    def _update(self):
        self.surface.fill(utils.BLACK)
        self._display_scores()
        self._display_turn()
        self._display_help()

    def update_turn(self, game_state):
        self.game_state = game_state
        self.turn = self.game_state.turn
        self.last_turn = self.game_state.turn_max
        self.phase = self.game_state.phase
        self._update()

    def update_wait(self):
        self._update()
        self._display_text(u'Attente du prochain', 9 * self.TEXT_HEIGHT, utils.GREY)
        self._display_text(u'champion…', 10 * self.TEXT_HEIGHT, utils.GREY)

    def update_end(self):
        self._update()
        self._display_text(u'Match terminé', 9 * self.TEXT_HEIGHT, utils.GREY)

    def update_wait_end(self, turn):
        self.turn = turn
        self._update()
        self._display_text(u'Attente de la fin', 9 * self.TEXT_HEIGHT, utils.RED)
        self._display_text(u'du match', 10 * self.TEXT_HEIGHT, utils.RED)
