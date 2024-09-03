from enum import Enum, auto
from typing import List
from random import randint
import time
import os


class Action(Enum):
    START = 0
    PAUSE = 1
    TERMINATE = 2
    LEFT = 3
    RIGHT = 4
    UP = 5
    DOWN = 6
    ACTION = 7


class State:
    def __init__(
        self,
        field: List[List[bool]],
        next: List[List[bool]],
        score: int,
        high_score: int,
        level: int,
        speed: int,
        pause: bool,
    ):
        self.field = field
        self.next = next
        self.score = score
        self.high_score = high_score
        self.level = level
        self.speed = speed
        self.pause = pause


class CurrentState(Enum):
    START = auto()
    MOVING = auto()
    SHIFTING = auto()
    PAUSE = auto()


current_state: CurrentState = CurrentState.START


class PlayerCar:
    def __init__(self):
        self.i: int = 14
        self.j: int = 4
        self.matrix: List[List[bool]] = [
            [False, True, False],
            [True, True, True],
            [False, True, False],
            [True, True, True],
        ]

    def left(self):
        if (
            current_state == CurrentState.MOVING
            or current_state == CurrentState.SHIFTING
        ) and self.j > 0:
            self.j -= 1

    def right(self):
        if (
            current_state == CurrentState.MOVING
            or current_state == CurrentState.SHIFTING
        ) and self.j < 7:
            self.j += 1


class EnemyCar:
    def __init__(self):
        self.i: int = -4
        self.j: int = randint(0, 7)
        self.matrix: List[List[bool]] = [
            [True, True, True],
            [False, True, False],
            [True, True, True],
            [False, True, False],
        ]

    def down(self):
        self.i += 1


def load_high_score() -> int:
    if os.path.exists("race_score.txt"):
        with open("race_score.txt", "r") as file:
            try:
                return int(file.read().strip())
            except ValueError:
                return 0
    return 0


def save_high_score(score: int) -> None:
    with open("race_score.txt", "w") as file:
        file.write(str(score))


class Game:
    def __init__(self):
        self.my_car: PlayerCar = PlayerCar()
        self.enemy_cars: List[EnemyCar] = [EnemyCar()]
        self.level: int = 0
        self.score: int = 0
        self.high_score: int = load_high_score()
        self.speed: int = 300
        self.pause: bool = False
        self.step: int = 0

    def generate_field(self) -> List[List[bool]]:
        res = [[False for _ in range(10)] for _ in range(20)]

        for i in range(4):
            for j in range(3):
                if self.my_car.matrix[i][j]:
                    res[i + self.my_car.i][j + self.my_car.j] = True

        for car in self.enemy_cars:
            for i in range(4):
                if i + car.i < 0 or i + car.i > 19:
                    continue
                for j in range(3):
                    if car.matrix[i][j]:
                        res[i + car.i][j + car.j] = True
        return res

    def generate_next(self):
        return [[False for _ in range(4)] for _ in range(4)]

    def get_state(self) -> State:
        return State(
            self.generate_field(),
            self.generate_next(),
            self.score,
            self.high_score,
            self.level,
            self.speed,
            self.pause,
        )

    def is_collision(self) -> bool:
        res: bool = False
        for car in self.enemy_cars:
            for i in range(4):
                if res:
                    break
                for j in range(3):
                    field_i: int = car.i + i
                    field_j: int = car.j + j
                    if (
                        car.matrix[i][j] is True
                        and 14 <= field_i <= 17
                        and self.my_car.j <= field_j <= self.my_car.j + 2
                        and self.my_car.matrix[field_i - self.my_car.i][
                            field_j - self.my_car.j
                        ]
                        is True
                    ):
                        res = True
                        break
        return res

    def increment_score(self) -> None:
        self.score += 1
        if self.high_score < self.score:
            self.high_score: int = self.score
        if self.level < 10 and self.score > 4:
            self.level: int = self.score // 5
            self.speed: int = 300 - self.level * 20

    def reset_values(self) -> None:
        self.step = self.score = self.level = 0
        self.speed: int = 300


game: Game = Game()
timer: float = time.time()


def user_input(action: int, hold: bool) -> None:
    global current_state
    if action == Action.START.value and current_state == CurrentState.START:
        current_state = CurrentState.MOVING
    elif action == Action.PAUSE.value:
        if current_state == CurrentState.PAUSE:
            current_state = CurrentState.MOVING
        else:
            current_state = CurrentState.PAUSE
        game.pause = not game.pause
    elif action == Action.TERMINATE.value:
        current_state = CurrentState.START
        save_high_score(game.high_score)
        game.enemy_cars.clear()
        game.enemy_cars.append(EnemyCar())
        game.reset_values()
    elif action == Action.LEFT.value and current_state == CurrentState.MOVING:
        game.my_car.left()
        if game.is_collision():
            game.enemy_cars.clear()
            game.enemy_cars.append(EnemyCar())
            game.reset_values()
            current_state = CurrentState.START
    elif action == Action.RIGHT.value and current_state == CurrentState.MOVING:
        game.my_car.right()
        if game.is_collision():
            game.enemy_cars.clear()
            game.enemy_cars.append(EnemyCar())
            game.reset_values()
            current_state = CurrentState.START
    elif action == Action.UP.value and current_state == CurrentState.MOVING:
        current_state = CurrentState.SHIFTING
    elif action == Action.DOWN.value:
        pass
    elif action == Action.ACTION.value:
        pass


def update_current_state() -> State:
    global current_state
    global timer
    if current_state == CurrentState.MOVING:
        cur_time: float = time.time()
        if cur_time - timer >= (game.speed / 1000.0):
            timer = cur_time
            current_state = CurrentState.SHIFTING
    if current_state == CurrentState.SHIFTING:
        for car in game.enemy_cars:
            car.down()
            if car.i == 19:
                game.increment_score()
        game.enemy_cars = [car for car in game.enemy_cars if car.i < 20]
        if game.step == 15:
            game.enemy_cars.append(EnemyCar())
            game.step = 0
        game.step += 1
        if game.is_collision():
            save_high_score(game.high_score)
            game.enemy_cars.clear()
            game.enemy_cars.append(EnemyCar())
            game.reset_values()
            current_state = CurrentState.START
        else:
            current_state = CurrentState.MOVING
    return game.get_state()
