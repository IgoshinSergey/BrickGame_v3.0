#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), currentGame(nullptr) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete currentGame; }

void MainWindow::on_tetrisButton_clicked() {
  GameWidget game = GameWidget(nullptr, 1);
  game.setModal(true);
  game.exec();
}

void MainWindow::on_snakeButton_clicked() {
  GameWidget game = GameWidget(nullptr, 2);
  game.setModal(true);
  game.exec();
}

void MainWindow::on_raceButton_clicked() {
  GameWidget game = GameWidget(nullptr, 3);
  game.setModal(true);
  game.exec();
}
