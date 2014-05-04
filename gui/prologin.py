import threading

import pygame

# initialization
pygame.init()

import state_reader
import window
import settings

game_state_reader = state_reader.StechecReader()
window = window.Window(game_state_reader)
gfx_thread = threading.Thread(target=window.run)

settings.load(['gui.so'])

def partie_debut():
    gfx_thread.start()

def phase_construction():
    game_state_reader.put_state()

def phase_deplacement():
    game_state_reader.put_state()

def phase_tirs():
    game_state_reader.put_state()

def phase_siege():
    game_state_reader.put_state()

#def jouer_tour():
#    pass
    #state_reader.put_state()

def partie_fin():
    pass
    game_state_reader.do_end()
