
import pygame

# TODO: change the colors
BLACK = (0, 0, 0)
DARK_GREY = (64, 64, 64)
GREY = (128, 128, 128)
LIGHT_GREY = (192, 192, 192)
WHITE = (255, 255, 255)

RED = (255, 0, 0)
GREEN = (0, 255, 0)

def make_surface(width, height):
    result = pygame.Surface((width, height))
    result = result.convert_alpha()
    result.fill((0, 0, 0, 0))
    return result

def make_button(caption, font):
    text = font.render(caption, True, WHITE)
    text_w, text_h = text.get_size()
    but_w, but_h = text_w + 16, text_h + 8
    button = make_surface(but_w, but_h)
    button.fill(LIGHT_GREY)
    button.fill(DARK_GREY, (2, 2, but_w - 2, but_h - 2))
    button.fill(GREY, (2, 2, but_w - 4, but_h - 4))
    button.blit(text, ((but_w - text_w) / 2, (but_h - text_h) / 2))
    return button

def make_bordered_text(caption, font, fgcolor=WHITE, bgcolor=BLACK):
    text = font.render(caption, True, fgcolor)
    border = font.render(caption, True, bgcolor)
    result = make_surface(*(unit + 2 for unit in text.get_size()))
    for dx in (0, 2):
        for dy in (0, 2):
            result.blit(border, (dx, dy))
    result.blit(text, (1, 1))
    return result

def make_shadow(width, height, alpha=192):
    result = make_surface(width, height)
    result.fill((0, 0, 0, alpha))
    return result


def set_between(value, lower=None, upper=None):
    result = value
    if lower is not None and result < lower:
        result = lower
    if upper is not None and result > upper:
        result = upper
    return result


def is_in_rect(x, y, w, h):
    return (
        0 <= x and x < w and
        0 <= y and y < h
    )

def is_near(x1, y1, x2, y2):
    return abs(x1 - x2) + abs(y1 - y2) <= 1


def iter_dict(dict, keys):
    for key in keys:
        yield key, dict[key]

