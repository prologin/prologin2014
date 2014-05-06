from collections import namedtuple
import struct

from api import *

Cell = namedtuple('Cell', 'type player wizards towers')
Tower = namedtuple('Tower', 'player scope life attack')
Player = namedtuple('Player', 'name score magic towers')

CELL_TYPES = {
    case_info.CASE_SIMPLE:    'simple',
    case_info.CASE_TOURELLE:  'tower',
    case_info.CASE_BASE:      'base',
    case_info.CASE_FONTAINE:  'fontain',
    case_info.CASE_ARTEFACT:  'artefact',
}

CELL_NAMES = {
    case_info.CASE_SIMPLE:    u'Case simple',
    case_info.CASE_TOURELLE:  u'Tourelle',
    case_info.CASE_BASE:      u'Base',
    case_info.CASE_FONTAINE:  u'Fontaine',
    case_info.CASE_ARTEFACT:  u'Artefact',
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

        self.players = {}
        self.player_id_to_rank = {}

        # Get all the players
        for i, player in json['players'].items():
            # The "key" of each player is the JSON dump is supposed to be an
            # integer. It enables us to order players and thus to
            # deterministically assign them colors.
            i = int(i)

            self.players[i] = Player(
                    player['name'] or (u'Équipe %d' % i),
                    player['score'],
                    player['magic'],
                    []
            )

        for tower in game_map['towers']:
            x, y = tower['x'], tower['y']
            self.cells[y][x].towers.append(tower_from_json(tower))
            self.players[int(tower['player'])].towers.append(tower_from_json(tower))


        for i, player_id in enumerate(sorted(self.players.keys())):
            self.player_id_to_rank[player_id] = i
