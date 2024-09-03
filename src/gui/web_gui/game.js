import { applyRootStyles } from './src/utils.js';
import { GameBoard } from './src/game-board.js';
import { rootStyles, keyCodes } from './src/config.js';
import { initializeGamesList } from './src/games-list.js';
import { NextBoard } from './src/next-board.js';
import { userInput } from './src/user-input.js';
import { updateState } from './src/update-state.js';

applyRootStyles(rootStyles);
const gameBoard = new GameBoard(document.querySelector('#game-board'));
const nextBoard = new NextBoard(document.querySelector('#next-board'));

const $sidePanel = document.querySelector('#side-panel');

document.addEventListener('DOMContentLoaded', initializeGamesList);
document.addEventListener('keydown', function (event) {
    if (keyCodes.up.includes(event.code)) {
        userInput(5, false);
    }
    if (keyCodes.right.includes(event.code)) {
        userInput(4, false);
    }
    if (keyCodes.down.includes(event.code)) {
        userInput(6, false);
    }
    if (keyCodes.left.includes(event.code)) {
        userInput(3, false);
    }
    if (keyCodes.start.includes(event.code)) {
        userInput(0 , false);
    }
    if (keyCodes.pause.includes(event.code)) {
        userInput(1 , false);
    }
    if (keyCodes.action.includes(event.code)) {
        userInput(7 , false);
    }
    if (keyCodes.exit.includes(event.code)) {
        gameBoard.disableAllTiles();
        nextBoard.disableAllTiles();
        document.querySelector('#score-value').textContent = '0';
        document.querySelector('#high-score-value').textContent = '0';
        document.querySelector('#level-value').textContent = '0';
        document.querySelector('#speed-value').textContent = '0';
        document.querySelector('#pause-value').textContent = 'false';
        userInput(2 , false);
    }
});

let isGameRunning = true;

const intervalId = setInterval(() => {
    if (isGameRunning) {
        updateState(gameBoard, nextBoard, $sidePanel);
    } else {
        clearInterval(intervalId);
    }
}, 20);
