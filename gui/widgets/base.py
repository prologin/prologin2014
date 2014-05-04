import pygame

import utils

# Every widget will inherit from this one
class BaseWidget(object):

    def __init__(self, x, y, width, height):
        self.x, self.y = x, y
        self.width, self.height = width, height
        self.surface = utils.make_surface(width, height)

    def plug(self, widgets):
        '''
        Subclasses should override this method to keep link to other widgets.
        '''
        pass

    def display(self, surface):
        surface.blit(self.surface, (self.x, self.y))

    def rebase_coordinates(self, x, y):
        '''
        Rebase given coordinates to be relative to the widget's position.  Be
        careful, this computation is valid even if the coordinates are outside
        the widget (result can be negative, etc.).
        '''
        return (x - self.x, y - self.y)

    def is_click_inside(self, x, y):
        '''
        If the given coordinates matches the widget's bounding box, return the
        relative coordinates. Return None otherwise. Subclasses should use this
        method to react to the mouse.
        '''
        if (
            self.x <= x and x < self.x + self.width and
            self.y <= y and y < self.y + self.height
        ):
            return self.rebase_coordinates(x, y)
        else:
            return None

    def handle_click(self, x, y, but1, but2, but3):
        '''
        Return if the click has been handled by this widget. Subclasses should
        override this method to do some action.
        '''
        return bool(self.is_click_inside(x, y))

    def handle_move(self, x, y, but1, but2, but3):
        '''
        Called when the widget is selected and the cursor has moved. Subclasses
        should override this method to perform some action.
        '''
        pass
