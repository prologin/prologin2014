var MAP_CELLS = 31;
var CELL_SIZE = 25;
var MAP_SIZE = MAP_CELLS * CELL_SIZE;

var map;
var cells;
var players;
var timer;

var gameData = [];

var imgNames = ["wizard.png", "tower.png", "base.png", "fontain.png", "artefact.png"]
var images = []
for (var i = 0; i < imgNames.length; i++) {
    images[i] = new Image();
    images[i].src = "/static/replay/sprites/" + imgNames[i]
}

function hexToRgb(hex) {
    // Expand shorthand form (e.g. "03F") to full form (e.g. "0033FF")
    var shorthandRegex = /^#?([a-f\d])([a-f\d])([a-f\d])$/i;
    hex = hex.replace(shorthandRegex, function(m, r, g, b) {
        return r + r + g + g + b + b;
    });

    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
    } : null;
}

// Decompress an LZW-encoded string
function lzw_decode(s) {
    var dict = {};
    var data = (s + "").split("");
    var currChar = data[0];
    var oldPhrase = currChar;
    var out = [currChar];
    var code = 256;
    var phrase;
    for (var i=1; i<data.length; i++) {
        var currCode = data[i].charCodeAt(0);
        if (currCode < 256) {
            phrase = data[i];
        }
        else {
            phrase = dict[currCode] ? dict[currCode] : (oldPhrase + currChar);
        }
        out.push(phrase);
        currChar = phrase.charAt(0);
        dict[code] = oldPhrase + currChar;
        code++;
        oldPhrase = phrase;
    }
    return out.join("");
}

function replay_init() {
    document.getElementById('replay_turn').max = 403;
    document.getElementById('replay_canvas').width = 775;
    document.getElementById('replay_canvas').height = 775;

    $.getScript("/static/replay/gunzip.min.js", function(){
    $.get("dump/", function(data)
          {
              gameData = JSON.parse(data);
              replay_update();
          });
    });


}

function replay_play() {
    if (timer) return;
    timer = setInterval(function(){
        turn = document.getElementById('replay_turn');
        if (turn.value == 403) {
            pause();
        } else {
            turn.value++;
            replay_update();
        }
    }, 100);
}

function replay_pause() {
    clearInterval(timer);
    timer = false;
}

function replay_update() {
    canvas = document.getElementById('replay_canvas');
    turn = Number(document.getElementById('replay_turn').value);

    document.getElementById('replay_lbl_turn').innerHTML =
        Math.floor(turn / 4); // 4 phases per turn

    state = gameData[turn];
    cells = state["map"]["cells"];
    players = state["players"];

    map = canvas.getContext('2d');
    map.fillStyle = "#44bb88";
    map.fillRect(0, 0, canvas.width, canvas.height);

    assignColors();
    grid();
    drawCells();
    scores();
}

function grid() {
    map.strokeStyle = '#55cc99';
    for (var i = 0; i < MAP_SIZE; i++) {
        map.beginPath();
        map.moveTo(i * CELL_SIZE, 0);
        map.lineTo(i * CELL_SIZE, MAP_SIZE);
        map.stroke();
    }

    for (var i = 0; i < MAP_SIZE; i++) {
        map.beginPath();
        map.moveTo(0, i * CELL_SIZE);
        map.lineTo(MAP_SIZE, i * CELL_SIZE);
        map.stroke();
    }
}

function countWizardsAndTowers(pid) {
    player = players[pid];

    wizards = 0;
    towers = 0;
    pid = Number(pid);
    for (var i = 0; i < cells.length; i++) {
        cell = cells[i];
        if (cell["object-player"] == pid) {
            wizards += cell["wizards"];
            if (cell["type"] == 1)
                towers++;
        }
    }
    player["wizards"] = wizards;
    player["towers"] = towers;
}

function assignColors() {
    colors = ["#00f", "#060", "#f00", "#ff0"];
    objcolors = ["#0089fd", "#0f0", "#f00", "#fdb300"];

    var i = 0;
    for (var pid in players) {
        players[pid]["color"] = colors[i];
        players[pid]["objcolor"] = objcolors[i];
        i++;
    }
}

function scores() {
    for (var i in players) {
        countWizardsAndTowers(i);
        player = players[i];
        name = player["name"];
        s = '';
        s = '<span style="width: 20px; display: inline-block; background: ' + player["color"] + '">&nbsp;</span>';
        s += player["magic"] +
            '<img src="/static/replay/sprites/magic.png" alt="magie" />';
        s += player["wizards"] +
            '<img src="/static/replay/sprites/wizard_gui.png" alt="sorciers" />';
        s += player["towers"] +
            '<img src="/static/replay/sprites/tower_icon.png" alt="tourelles" />';
        document.getElementById("replay_p" + name).innerHTML = s;
    }
}

function drawImg(img, x, y, pid) {

    map.drawImage(img, x * CELL_SIZE, y * CELL_SIZE);

    var color = hexToRgb("#CCCCCC");
    if(pid >= 0)
        color = hexToRgb(players[pid]["objcolor"]);

    var new_img = map.getImageData(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);

    for (var i=0; i < new_img.data.length; i+=4)
    {
        if (Math.abs(new_img.data[i] - new_img.data[i+2]) < 5 &&
            new_img.data[i+1] < new_img.data[i])
        {
            var light = new_img.data[i] / 255;
            new_img.data[i] = color.r * light;
            new_img.data[i+1] = color.g * light;
            new_img.data[i+2] = color.b * light;
        }

    }

    map.putImageData(new_img, x * CELL_SIZE, y * CELL_SIZE);
}

function drawCircle(pid, x, y) {
    if (pid < 0) return;

    state = gameData[0];
    x = (x + 1) * CELL_SIZE - 3;
    y = y * CELL_SIZE + 2;
    map.beginPath();
    map.arc(x, y, 3, 0, 2 * Math.PI);
    map.fillStyle = players[pid]["color"];
    map.fill();
    map.strokeStyle = players[pid]["color"];
    map.stroke();
}

function drawCells() {
    for (var i = 0; i < cells.length; i++) {
        cell = cells[i];
        img = images[Number(cell["type"])];
        drawImg(img, cell["x"], cell["y"], cell["object-player"]);
        //drawCircle(cell["object-player"], cell["x"], cell["y"]);
    }
}

