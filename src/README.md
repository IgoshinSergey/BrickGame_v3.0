## Install requirements

Ubuntu

```bash
sudo apt update
sudo apt install libcurl4-openssl-dev
sudo apt install nlohmann-json3-dev
```

```bash
python3.12 -m venv .venv
source .venv/bin/activate
pip install -r src/requirements.txt
```

## Install cli and desktop

```bash
cd src
```

```bash
make install
```

## Run server

* Run in new terminal

```bash
make server
```

## Run games

| Клавиша | Действие |
|---------|----------|
| Стрелка влево  | Перемещение влево |
| Стрелка вправо | Перемещение вправо |
| Стрелка вверх  | Перемещение вперед |
| Z       | Выполнение действия (например, поворот фигуры в тетрисе) |
| P       | Пауза игры |
| ESC     | Выход из игры |


### Run Command-line interface
```
./build/BrickGame_cli     
```

### Run Desktop interface
```bash
./build/BrickGame_desktop
```
### Run Web interface

* open in browser http://localhost:8080
