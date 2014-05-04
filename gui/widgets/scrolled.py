# -*- coding: utf-8 -*-

from widgets.base import BaseWidget
import utils

# Object scrollable
class ScrolledWidget(BaseWidget):

    # Subclasses must define the height of one line.
    LINE_HEIGHT = None
    PADDING = 0

    def __init__(self, *args):
        # Subclass must define 'self.font' to a valid pygame font.
        super(ScrolledWidget, self).__init__(*args)
        self.slots = (self.height - 2 * self.PADDING) / self.LINE_HEIGHT

        self.list_surface = None
        self.list_position = (self.PADDING, self.PADDING)
        self.list_size = (
            self.width - 2 * self.PADDING,
            self.LINE_HEIGHT * self.slots
        )

        self.cursor = 0
        self.cursor_max = 0

        # Arrows indicate that scrolling is possible.
        arrow_up = self.font.render(u'↑', True, utils.WHITE)
        arrow_down = self.font.render(u'↓', True, utils.WHITE)
        arrow_width, arrow_height = arrow_up.get_size()
        self.arrow_up = BaseWidget(
            self.width - arrow_width, 0,
            arrow_width, arrow_height
        )
        self.arrow_down = BaseWidget(
            self.width - arrow_width,
            self.height - arrow_height,
            arrow_width, arrow_height
        )
        self.arrow_up.surface.blit(arrow_up, (0, 0))
        self.arrow_down.surface.blit(arrow_down, (0, 0))

    def set_list_length(self, length):
        self.list_surface = utils.make_surface(
            self.list_size[0],
            self.LINE_HEIGHT * length
        )
        self.cursor_max = utils.set_between(length - self.slots, lower=0)
        self.cursor = utils.set_between(
            self.cursor, lower=0, upper=self.cursor_max
        )

    def clean_surface(self):
        self.surface.fill(utils.BLACK)

    def scroll(self, increment):
        self.clean_surface()
        if self.list_surface:
            self.cursor = utils.set_between(
                self.cursor + increment,
                lower=0, upper=self.cursor_max
            )
            self.surface.blit(
                self.list_surface,
                self.list_position,
                (0, self.cursor * self.LINE_HEIGHT) + self.list_size
            )

            if self.cursor > 0:
                # The displayed arrow to the top indicates that we can scroll
                # up.
                self.arrow_up.display(self.surface)
            if self.cursor < self.cursor_max:
                # The displayed arrow to the bottom indicates that we can
                # scroll down.
                self.arrow_down.display(self.surface)

    def is_click_inside(self, x, y):
        result = super(ScrolledWidget, self).is_click_inside(x, y)
        if result is None:
            return None
        x = result[0] - self.PADDING
        y = result[1] - self.PADDING
        if (
            result is None or
            x < 0 or self.width - 2 * self.PADDING <= x or
            y < 0 or self.height - 2 * self.PADDING <= y
        ):
            return None
        else:
            return y / self.LINE_HEIGHT + self.cursor

    def handle_click(self, x, y, but1, but2, but3):
        result = super(ScrolledWidget, self).is_click_inside(x, y)
        if not result:
            return False
        if but1 and self.arrow_up.is_click_inside(*result):
            self.scroll(-1)
            return True
        elif but1 and self.arrow_down.is_click_inside(*result):
            self.scroll(1)
            return True
        else:
            return False
