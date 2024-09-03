export async function updateState(gameBoard, nextBoard, $sidePanel) {
    try {
        const response = await fetch('/api/state');
        if (response.ok) {
            const state = await response.json();
            render(gameBoard, nextBoard, $sidePanel, state);
        } else {
            console.error(response.statusText);
        }
    } catch(error) {
        console.error(error);
    }
}

function render(gameBoard, nextBoard, $sidePanel, state) {
    $sidePanel.querySelector('#score-value').textContent = state.score;
    $sidePanel.querySelector('#high-score-value').textContent = state.high_score;
    $sidePanel.querySelector('#level-value').textContent = state.level;
    $sidePanel.querySelector('#speed-value').textContent = state.speed;
    $sidePanel.querySelector('#pause-value').textContent = state.pause;
    gameBoard.disableAllTiles()
    state.field.forEach((row, i) => {
        row.forEach((elem, j) => {
            if (elem === true) {
                gameBoard.enableTile(j, i);
            }
        });
    });
    nextBoard.disableAllTiles();
    state.next.forEach((row, i) => {
        row.forEach((elem, j) => {
            if (elem === true) {
                nextBoard.enableTile(j, i);
            }
        });
    });

}