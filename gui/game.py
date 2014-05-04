from collections import namedtuple
import struct

from api import *

Cell = namedtuple('Cell', 'type player wizards towers')
Tower = namedtuple('Tower', 'player scope life attack')
Player = namedtuple('Player', 'name score magic')

CELL_TYPES = {
        case_info.CASE_SIMPLE:    u'base',
        case_info.CASE_TOURELLE:  u'tower',
        case_info.CASE_BASE:      u'base',
        case_info.CASE_FONTAINE:  u'fontain',
        case_info.CASE_ARTEFACT:   u'artefact'
}

# TODO: action ?

NO_PLAYER = -1
DEFAULT_CELL = Cell(case_info.CASE_SIMPLE, -1, 0, [])

def cell_from_json(cell):
    return Cell(cell['type'], cell['player'], cell['wizards'], [])

def tower_from_json(tower):
    return Tower(tower['player'], tower['scope'], tower['life'],
            tower['attack'])

class GameState:

    def __init__(self, json):
        # use the json file generated by the dumper
        self.turn = json['turn'][0]
        self.turn_max = json['turn'][1]
        self.phase = json['phase']

        game_map = json['map']

        self.map_width = self.map_height = TAILLE_TERRAIN

        # initialization of a map of simple cell
        self.cells = [
                [
                    DEFAULT_CELL._replace(towers=[])
                    for _ in range(self.map_width)
                ]
                for _ in range(self.map_height)
        ]

        for cell in game_map['cells']:
            x, y = cell['x'], cell['y']
            self.cells[y][x] = cell_from_json(cell)

        for tower in game_map['towers']:
            x, y = tower['x'], tower['y']
            self.cells[y][x].towers.append(tower_from_json(tower))

        self.players = {}
        self.player_id_to_order = {}
        ordered_players_ids = []
        # recuperates all the player
        for i, player in json['players'].items():
            i = int(i)
            self.players[i] = Player(
                    player['name'] or (u'Equipe %d' % i),
                    player['score'],
                    player['magic']
            )
            self.player_id_to_order[i] = len(ordered_players_ids)
            ordered_players_ids.append(i)
        # list ordered
        self.ordered_players_ids = sorted(ordered_players_ids)

