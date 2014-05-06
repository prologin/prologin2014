
import pygame

# TODO: change the colors
BLACK = (0, 0, 0)
DARK_GREY = (64, 64, 64)
GREY = (128, 128, 128)
LIGHT_GREY = (192, 192, 192)
WHITE = (255, 255, 255)

RED = (255, 0, 0)
GREEN = (0, 255, 0)
YELLOW = (253, 179, 0)
BLUE = (0, 137, 253)

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
    text_w, text_h = text.get_size()
    result = make_surface(text_w + 2, text_h + 2)
    for dx in range(3):
        for dy in range(3):
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

def set_color(surface, color):
    '''
    Return a copy on the given surface where fuchsia pixels are replaced using
    the given color. Lighting and saturation are handled, but not blending with
    another color.
    '''
    result = surface.copy()
    width, height = result.get_size()
    # TODO: handle saturation
    for y in range(height):
        for x in range(width):
            (r, g, b, a) = result.get_at((x, y))
            if abs(r - b) < 5 and g < r:
                light = r
                saturation = 255 # TODO
                r = color[0] * light // 255
                g = color[1] * light // 255
                b = color[2] * light // 255
                result.set_at((x, y), (r, g, b, a))
    return result
