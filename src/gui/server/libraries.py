import ctypes


class GameInfo(ctypes.Structure):
    _fields_ = [
        ("field", ctypes.POINTER(ctypes.POINTER(ctypes.c_int))),
        ("next", ctypes.POINTER(ctypes.POINTER(ctypes.c_int))),
        ("score", ctypes.c_int),
        ("high_score", ctypes.c_int),
        ("level", ctypes.c_int),
        ("speed", ctypes.c_int),
        ("pause", ctypes.c_int),
    ]


tetris = ctypes.CDLL("./libtetris.so")
snake = ctypes.CDLL("./libsnake.so")

tetris.InitAllInfo.argtypes = []
tetris.InitAllInfo.restype = None

tetris.userInput.argtypes = [ctypes.c_int, ctypes.c_bool]
tetris.userInput.restype = None

tetris.updateCurrentState.argtypes = []
tetris.updateCurrentState.restype = GameInfo

tetris.DestroyGame.argtypes = []
tetris.DestroyGame.restype = None

snake.userInput.argtypes = [ctypes.c_int, ctypes.c_bool]
snake.userInput.restype = None

snake.updateCurrentState.argtypes = []
snake.updateCurrentState.restype = GameInfo


def convert_pointer_to_bool_matrix(pointer, rows, cols):
    bool_matrix = []
    if pointer is None:
        return [[False for _ in range(cols)] for _ in range(rows)]
    for i in range(rows):
        row_pointer = pointer[i]
        bool_row = []
        for j in range(cols):
            value = row_pointer[j]
            bool_row.append(bool(value))
        bool_matrix.append(bool_row)
    return bool_matrix
