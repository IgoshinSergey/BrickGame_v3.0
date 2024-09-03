export async function initializeGamesList() {
    try {
        const response = await fetch('/api/games');
        if (response.ok) {
            const data = await response.json();
            renderGameButtons(data.games);
        } else {
            console.error(response.statusText);
        }
    } catch (error) {
        console.error(error);
    }
}

function renderGameButtons(games) {
    const gamesListContainer = document.getElementById('games-list');
    gamesListContainer.innerHTML = '';

    games.forEach(game => {
        const button = document.createElement('button');
        button.textContent = game.name;
        button.onclick = () => selectGame(game.id);
        gamesListContainer.appendChild(button);
    });
}

async function selectGame(gameId) {
    const response = await fetch(`/api/games/${gameId}`, {
        method: 'POST'
    });
    if (response.ok) {
        alert(`Игра запущена`);
    } else {
        const errorData = await response.json();
        console.error(errorData.message);
    }
}
