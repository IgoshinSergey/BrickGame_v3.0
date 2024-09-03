import { NEXT_BOARD_WIDTH, NEXT_BOARD_HEIGHT } from './config.js';

export class NextBoard {
    constructor($nextBoard) {
        this.element = $nextBoard;
        this.tiles = [];
        for (let i = 0; i < NEXT_BOARD_WIDTH; ++i) {
            for (let j = 0; j < NEXT_BOARD_HEIGHT; ++j) {
                const $tile = document.createElement('div');
                $tile.classList.add('tile');
                $tile.id = `position-${i}-${j}`;
                this.tiles.push($tile);
                this.element.append($tile);
            }
        }
    }

    getTile(x, y) {
        return this.tiles[y * NEXT_BOARD_WIDTH + x];
    }

    enableTile(x, y) {
        this.getTile(x, y).classList.add('active');
    }

    disableTile(x, y) {
        this.getTile(x, y).classList.remove('active');
    }

    disableAllTiles() {
        this.tiles.forEach((tile) => {
            tile.classList.remove('active');
        })
    }
}