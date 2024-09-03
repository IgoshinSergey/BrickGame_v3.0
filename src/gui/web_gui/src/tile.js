export class Tile {
    constructor(x, y) {
        const $tile = document.createElement('div');
        $tile.className = 'tile';
        $tile.id = `position-${x}-${y}`;
    }
}
