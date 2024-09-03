export const GAME_BOARD_WIDTH = 10;
export const GAME_BOARD_HEIGHT = 20;

export const NEXT_BOARD_WIDTH = 4;
export const NEXT_BOARD_HEIGHT = 4;

export const rootStyles = {
    '--tile-size': '20px',
    '--tile-color': '#8beff1',
    '--tile-active-color': '#000000',
    '--game-board-width': GAME_BOARD_WIDTH,
    '--game-board-height': GAME_BOARD_HEIGHT,
    '--game-board-gap': '2px',
    '--game-board-background': '#333',
    '--next-board-width': NEXT_BOARD_WIDTH,
    '--next-board-height': NEXT_BOARD_HEIGHT,
    '--next-board-gap': '2px',
    '--next-board-background': '#333',
};

export const keyCodes = {
    up: ['ArrowUp', 'KeyW', 'KeyI'],
    right: ['ArrowRight', 'KeyD', 'KeyL'],
    down: ['ArrowDown', 'KeyS', 'KeyK'],
    left: ['ArrowLeft', 'KeyA', 'KeyJ'],
    pause: ['KeyP', 'Keyp'],
    exit: ['Escape'],
    action: ['KeyZ', 'Keyz'],
    start: ['Enter'],
};
