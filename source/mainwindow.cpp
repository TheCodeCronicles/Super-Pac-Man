#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("pac-man"));

    /* Initialize graphicsview and game scene */
    ui->graphicsView->setStyleSheet("QGraphicsView {border: none;}");
    ui->graphicsView->setBackgroundBrush(Qt::black);
    ui->graphicsView->setFocusPolicy(Qt::NoFocus);

    int map_height = 20, map_width = 57;            // 20x57 game map
    int x = 50, y = 50;                             // x y in mainwindow
    int w = (map_width * GameObject::Width);
    int h = (map_height * GameObject::Width);

    ui->graphicsView->setGeometry(x, y, w, h);
    game = new Game(x, y, map_width, map_height, ":/game_objects/map_objects/map.txt");
    ui->graphicsView->setScene(game);
    initLabels();
    game->start();
}


/* Initialize UI */
void MainWindow::initLabels()
{
    // Title label
    QLabel *title = new QLabel(this);
    title->setText("Super Pacman");
    title->setStyleSheet("QLabel {font-family: Fixedsys; color: yellow; font-size: 24px;}");
    title->setGeometry(280, 12, 200, 26);

    // Score Title label
    score_title = new QLabel(this);
    score_title->setText("Score");
    score_title->setStyleSheet("QLabel {font-family: Fixedsys;color: white;font-size: 16px;}");
    score_title->setGeometry(50, 12, 60, 26);

    // Score label
    score = new QLabel(this);
    score->setIndent(-80);
    score->setText("0");
    score->setStyleSheet("QLabel {font-family: Fixedsys;color: white;font-size: 16px;}");
    score->setGeometry(110, 12, 150, 26);

    win_label = new QLabel(this);
    win_label->hide();
    win_label->setText("You win!");
    win_label->setStyleSheet("QLabel {font-family: Fixedsys;color: yellow;font-size: 16px;}");
    win_label->setGeometry((width() - win_label->width()) / 2, height() - 40, win_label->width(), win_label->height());

    lose_label = new QLabel(this);
    lose_label->hide();
    lose_label->setText("You lose!");
    lose_label->setStyleSheet("QLabel {font-family: Fixedsys;color: red;font-size: 16px;}");
    lose_label->setGeometry((width() - lose_label->width()) / 2, height() - 40, lose_label->width(), lose_label->height());

    kill_mode = new QLabel(this);
    kill_mode->hide();
    kill_mode->setText("KILL MODE!");
    kill_mode->setStyleSheet("QLabel {font-family: Fixedsys;color: red;font-size: 16px;}");
    kill_mode->setGeometry((width() - kill_mode->width()) / 2, height() - 40, kill_mode->width(), kill_mode->height());

    score_timer = new QTimer(this);
    score_timer->start(25);
    connect(score_timer, SIGNAL(timeout()), this , SLOT(update_score()));
}


/* Update score UI */
void MainWindow::update_score()
{
    score->setText(QString::number(game->get_score()));
    if (game->stat == Game::Win)
    {
        win_label->show();
        score_timer->stop();
    }
    else if (game->stat == Game::Lose)
    {
        lose_label->show();
        score_timer->stop();
    }
}


void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_Up:
        game->pacman_next_direction(GameObject::Up);
        break;
    case Qt::Key_Left:
        game->pacman_next_direction(GameObject::Left);
        break;
    case Qt::Key_Down:
        game->pacman_next_direction(GameObject::Down);
        break;
    case Qt::Key_Right:
        game->pacman_next_direction(GameObject::Right);
        break;
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}



