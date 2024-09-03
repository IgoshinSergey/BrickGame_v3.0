import unittest
from unittest.mock import patch, mock_open
import time

import brick_game.race.race as race


class TestRaceGame(unittest.TestCase):

    def setUp(self):
        self.game = race.Game()

    def test_enemy_car_movement(self):
        enemy_car = race.EnemyCar()
        initial_position = enemy_car.i
        enemy_car.down()
        self.assertEqual(enemy_car.i, initial_position + 1)

    def test_generate_field(self):
        field = self.game.generate_field()
        self.assertEqual(len(field), 20)
        for i in range(20):
            self.assertEqual(len(field[i]), 10)

    def test_increment_score(self):
        self.game.increment_score()
        self.assertEqual(self.game.score, 1)
        self.assertEqual(self.game.level, 0)
        self.assertEqual(self.game.speed, 300)

        for _ in range(5):
            self.game.increment_score()
        self.assertEqual(self.game.level, 1)
        self.assertEqual(self.game.speed, 280)

    def test_is_collision(self):
        self.game.my_car.j = 4
        self.game.enemy_cars.append(race.EnemyCar())
        self.game.enemy_cars[0].i = 14
        self.game.enemy_cars[0].j = 4
        self.assertTrue(self.game.is_collision())

    def test_reset_values(self):
        self.game.level = 1
        self.game.speed = 280
        self.game.score = 6
        self.game.reset_values()
        self.assertEqual(self.game.level, 0)
        self.assertEqual(self.game.score, 0)
        self.assertEqual(self.game.speed, 300)

    def test_get_state(self):
        state = self.game.get_state()
        self.assertEqual(state.level, 0)
        self.assertEqual(state.score, 0)
        self.assertEqual(state.speed, 300)
        self.assertEqual(len(state.next), 4)
        for i in range(4):
            self.assertEqual(len(state.next[i]), 4)
        self.assertEqual(len(state.field), 20)
        for i in range(20):
            self.assertEqual(len(state.field[i]), 10)


class TestPlayerCar(unittest.TestCase):

    def setUp(self):
        self.my_car = race.PlayerCar()

    @patch("brick_game.race.race.current_state", new=race.CurrentState.MOVING)
    def test_left_when_moving(self):
        j = self.my_car.j
        self.my_car.left()
        self.assertEqual(self.my_car.j, j - 1)

    @patch("brick_game.race.race.current_state", new=race.CurrentState.START)
    def test_left_when_not_moving(self):
        j = self.my_car.j
        self.my_car.left()
        self.assertEqual(self.my_car.j, j)

    @patch("brick_game.race.race.current_state", new=race.CurrentState.MOVING)
    def test_right_when_moving(self):
        j = self.my_car.j
        self.my_car.right()
        self.assertEqual(self.my_car.j, j + 1)

    @patch("brick_game.race.race.current_state", new=race.CurrentState.START)
    def test_right_when_not_moving(self):
        j = self.my_car.j
        self.my_car.right()
        self.assertEqual(self.my_car.j, j)


class TestHighScoreFunctions(unittest.TestCase):
    @patch("builtins.open", new_callable=mock_open, read_data="100")
    @patch("os.path.exists", return_value=True)
    def test_load_high_score(self, mock_exists, mock_file):
        score = race.load_high_score()
        self.assertEqual(score, 100)
        mock_file.assert_called_once_with("race_score.txt", "r")

    @patch("builtins.open", new_callable=mock_open)
    def test_load_high_score_invalid(self, mock_file):
        mock_file.side_effect = ValueError
        score = race.load_high_score()
        self.assertEqual(score, 0)

    @patch("builtins.open", new_callable=mock_open)
    def test_save_high_score(self, mock_file):
        race.save_high_score(150)
        mock_file().write.assert_called_once_with("150")

    @patch("os.path.exists", return_value=True)
    @patch("builtins.open", new_callable=mock_open, read_data="invalid")
    def test_load_high_score_file_exists_invalid(self, mock_file, mock_exists):
        score = race.load_high_score()
        self.assertEqual(score, 0)
        mock_file.assert_called_once_with("race_score.txt", "r")


class TestStates(unittest.TestCase):
    def setUp(self):
        race.current_state = race.CurrentState.START

    def test_start(self):
        game_info = race.update_current_state()
        self.assertEqual(race.current_state, race.CurrentState.START)
        self.assertEqual(game_info.score, 0)
        self.assertEqual(game_info.level, 0)
        self.assertEqual(game_info.pause, False)
        self.assertEqual(game_info.speed, 300)

        race.user_input(race.Action.START.value, False)
        self.assertEqual(race.current_state, race.CurrentState.MOVING)

        race.user_input(race.Action.UP.value, False)
        self.assertEqual(race.current_state, race.CurrentState.SHIFTING)

        race.update_current_state()
        self.assertEqual(race.current_state, race.CurrentState.MOVING)

        race.user_input(race.Action.LEFT.value, False)
        time.sleep(0.3)
        race.update_current_state()
        self.assertEqual(race.current_state, race.CurrentState.MOVING)

    def test_pause(self):
        race.user_input(race.Action.START.value, False)
        race.update_current_state()
        self.assertEqual(race.current_state, race.CurrentState.MOVING)

        race.user_input(race.Action.PAUSE.value, False)
        self.assertEqual(race.current_state, race.CurrentState.PAUSE)

        race.user_input(race.Action.PAUSE.value, False)
        self.assertEqual(race.current_state, race.CurrentState.MOVING)

    def test_terminate(self):
        race.user_input(race.Action.START.value, False)
        self.assertEqual(race.current_state, race.CurrentState.MOVING)
        race.user_input(race.Action.TERMINATE.value, False)
        self.assertEqual(race.current_state, race.CurrentState.START)

        game_info = race.update_current_state()
        self.assertEqual(game_info.score, 0)
        self.assertEqual(game_info.level, 0)
        self.assertEqual(game_info.pause, False)
        self.assertEqual(game_info.speed, 300)

    def test_right(self):
        race.game.my_car.j = 0
        race.game.enemy_cars[0].i = 15
        race.game.enemy_cars[0].j = 3

        race.user_input(race.Action.START.value, False)
        self.assertEqual(race.current_state, race.CurrentState.MOVING)

        race.user_input(race.Action.RIGHT.value, False)
        self.assertEqual(race.current_state, race.CurrentState.START)

    def test_left(self):
        race.game.enemy_cars = [race.EnemyCar()]
        race.game.my_car.j = 3
        race.game.enemy_cars[0].i = 15
        race.game.enemy_cars[0].j = 0

        race.user_input(race.Action.START.value, False)
        self.assertEqual(race.current_state, race.CurrentState.MOVING)

        race.user_input(race.Action.LEFT.value, False)
        self.assertEqual(race.current_state, race.CurrentState.START)

    def test_down(self):
        race.user_input(race.Action.START.value, False)
        self.assertEqual(race.current_state, race.CurrentState.MOVING)

        race.user_input(race.Action.DOWN.value, False)
        self.assertEqual(race.current_state, race.CurrentState.MOVING)

    def test_action(self):
        race.user_input(race.Action.START.value, False)
        self.assertEqual(race.current_state, race.CurrentState.MOVING)

        race.user_input(race.Action.ACTION.value, False)
        self.assertEqual(race.current_state, race.CurrentState.MOVING)

    def test_increment(self):
        race.game.enemy_cars = [race.EnemyCar()]
        race.game.enemy_cars[0].i = 18
        race.game.enemy_cars[0].j = 7
        race.game.my_car.j = 0
        game_info = race.update_current_state()
        self.assertEqual(game_info.score, 0)
        race.user_input(race.Action.START.value, False)
        self.assertEqual(race.current_state, race.CurrentState.MOVING)
        race.user_input(race.Action.UP.value, False)
        self.assertEqual(race.current_state, race.CurrentState.SHIFTING)
        game_info = race.update_current_state()
        self.assertEqual(race.current_state, race.CurrentState.MOVING)
        self.assertEqual(game_info.score, 1)

    def test_shifting_1(self):
        race.game.enemy_cars = [race.EnemyCar()]
        race.game.enemy_cars[0].j = 0
        race.game.my_car.j = 0
        race.user_input(race.Action.START.value, False)
        self.assertEqual(race.current_state, race.CurrentState.MOVING)

        for _ in range(15):
            race.user_input(race.Action.UP.value, False)
            race.update_current_state()

        self.assertEqual(race.current_state, race.CurrentState.START)

    def test_shifting_2(self):
        race.game.enemy_cars = [race.EnemyCar()]
        race.game.enemy_cars[0].j = 7
        race.game.my_car.j = 0
        race.user_input(race.Action.START.value, False)
        self.assertEqual(race.current_state, race.CurrentState.MOVING)

        for _ in range(20):
            race.user_input(race.Action.UP.value, False)
            race.update_current_state()

        self.assertEqual(race.current_state, race.CurrentState.MOVING)


if __name__ == "__main__":
    unittest.main()
