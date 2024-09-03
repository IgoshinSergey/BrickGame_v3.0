from contextlib import asynccontextmanager
import os
import sys

from fastapi import FastAPI
from fastapi.responses import JSONResponse, FileResponse
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel
from typing import List, Optional
import uvicorn

from libraries import tetris, snake, convert_pointer_to_bool_matrix

current_dir = os.path.dirname(os.path.abspath(__file__))
src_path = os.path.abspath(os.path.join(current_dir, "../../"))
sys.path.append(src_path)

from brick_game.race.race import update_current_state, user_input  # noqa


class GameInfo(BaseModel):
    id: int
    name: str


class GamesList(BaseModel):
    games: List[GameInfo]


class UserAction(BaseModel):
    action_id: int
    hold: bool


class GameState(BaseModel):
    field: List[List[bool]]
    next: List[List[bool]]
    score: int
    high_score: int
    level: int
    speed: int
    pause: bool


class ErrorMessage(BaseModel):
    message: str


games = [
    GameInfo(id=1, name="Tetris"),
    GameInfo(id=2, name="Snake"),
    GameInfo(id=3, name="Race"),
]


@asynccontextmanager
async def lifespan(application: FastAPI):
    tetris.InitAllInfo()
    yield
    tetris.DestroyGame()


app = FastAPI(lifespan=lifespan)
current_game_id: Optional[int] = None
current_game_state: Optional[GameState] = None


@app.get("/")
async def root() -> FileResponse:
    return FileResponse("./gui/web_gui/index.html")


@app.get("/api/games", response_model=GamesList)
async def get_games() -> GamesList:
    return GamesList(games=games)


@app.post("/api/games/{game_id}")
async def post_game_id(game_id: int) -> JSONResponse:
    global current_game_id
    if game_id not in [game.id for game in games]:
        error = ErrorMessage(message="The game ID was not found")
        return JSONResponse(status_code=404, content=error.dict())
    if current_game_id is not None:
        error = ErrorMessage(message="The user has already started another game")
        return JSONResponse(status_code=409, content=error.dict())
    current_game_id = game_id
    return JSONResponse(status_code=200, content={})


@app.post("/api/actions")
async def post_user_action(action: UserAction) -> JSONResponse:
    global current_game_id
    if current_game_id is None:
        return JSONResponse(
            status_code=400,
            content=ErrorMessage(message="The user did not launch the game").dict(),
        )

    if current_game_id == 1:
        tetris.userInput(action.action_id, action.hold)
    elif current_game_id == 2:
        snake.userInput(action.action_id, action.hold)
    else:
        user_input(action.action_id, action.hold)

    if action.action_id == 2:
        current_game_id = None

    return JSONResponse(status_code=200, content={})


@app.get("/api/state", response_model=GameState)
async def get_game_state():
    if current_game_id is None:
        error = ErrorMessage(message="The user did not launch the game")
        return JSONResponse(status_code=400, content=error.dict())

    if current_game_id == 1:
        game_info = tetris.updateCurrentState()
    elif current_game_id == 2:
        game_info = snake.updateCurrentState()
    else:
        game_info = update_current_state()

    return GameState(
        field=convert_pointer_to_bool_matrix(game_info.field, 20, 10),
        next=convert_pointer_to_bool_matrix(game_info.next, 4, 4),
        score=game_info.score,
        high_score=game_info.high_score,
        level=game_info.level,
        speed=game_info.speed,
        pause=game_info.pause,
    )


app.mount("/static", StaticFiles(directory="./gui/web_gui"), name="static")

if __name__ == "__main__":
    uvicorn.run(app, host="localhost", port=8080)
