#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Super Pac-Man"));

    /* Initialize graphicsview and game scene */
    ui->graphicsView->setStyleSheet("QGraphicsView {border: none;}");
    ui->graphicsView->setBackgroundBrush(Qt::black);
    ui->graphicsView->setFocusPolicy(Qt::NoFocus);

    start = new QPushButton(this);
    start_icon = QPixmap(":/game_objects/map_objects/start.png");
    start_icon_inverted = QPixmap(":/game_objects/map_objects/start_inverted.png");
    start->setIcon(QIcon(start_icon));
    start->setFixedSize(start->size());
    start->setIconSize(start->size());
    start->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
    start->move((width()-start->width())/2 - 4, (height() - 60));
    connect(start, &QPushButton::clicked, this, &MainWindow::start_button);
    start->show();
    start->setFocusPolicy(Qt::NoFocus);

    restart = new QPushButton(this);
    restart_icon = QPixmap(":/game_objects/map_objects/restart.png");
    restart_icon_inverted = QPixmap(":/game_objects/map_objects/restart_inverted.png");
    restart->setIcon(QIcon(restart_icon));
    restart->setFixedSize(restart->size()*2);
    restart->setIconSize(restart->size());
    restart->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
    restart->move((width()-restart->width())/2 - 4, (height() - 60));
    connect(restart, &QPushButton::clicked, this, &MainWindow::restart_game);
    restart->hide();

    flash_timer = new QTimer(this);
    connect(flash_timer, SIGNAL(timeout()), this, SLOT(flash_button()));
    flash_timer->setInterval(RESTART_FLASH_INTERVAL);

    initial_delay = new QTimer(this);
    connect(initial_delay, SIGNAL(timeout()), this, SLOT(start_game()));
    initial_delay->setInterval(INITIAL_DELAY);

    label_display_timer = new QTimer(this);
    connect(label_display_timer, SIGNAL(timeout()), this, SLOT(display_labels()));
    label_display_timer->start(10);


    int map_height = 20, map_width = 57;            // 20x57 game map
    int x = 50, y = 130;                             // x y in mainwindow
    int w = (map_width * GameObject::Width);
    int h = (map_height * GameObject::Width);

    ui->graphicsView->setGeometry(x, y, w, h);
    game = new Game(x, y, map_width, map_height, ":/game_objects/map_objects/map.txt");
    ui->graphicsView->setScene(game);
    initLabels();
    flash_timer->start();
    //game->start();            // Uncomment to bypass start button (For Development)
}


/* Initialize UI */
void MainWindow::initLabels()
{
    // Title label
    QLabel *title = new QLabel(this);
    QImage game_title;
    game_title.load(":/game_objects/map_objects/GameLogo.png");
    QPixmap title_pixmap = QPixmap::fromImage(game_title);
    title->setPixmap(title_pixmap);
    title->setScaledContents(true);
    title->setGeometry((width() - title_pixmap.width()/4)/2, 12, title_pixmap.width()/4, title_pixmap.height()/4);
    title->show();

    // Score Title label
    score_title = new QLabel(this);
    score_title->setText("Score");
    score_title->setStyleSheet("QLabel {font-family: Fixedsys;color: white;font-size: 16px;}");
    score_title->setGeometry(50, 12, 60, 26);

    // Buff label
    Buff_Label = new QLabel(this);
    Buff_Label->setText("BUFFS:");
    Buff_Label->setStyleSheet("QLabel {font-family: Fixedsys;color: Blue;font-size: 32px;}");
    Buff_Label->setGeometry(50, 540, 60, 26);

    // Nerf label
    Nerf_Label = new QLabel(this);
    Nerf_Label->setText("NERFS:");
    Nerf_Label->setStyleSheet("QLabel {font-family: Fixedsys;color: Red;font-size: 32px;}");
    Nerf_Label->setGeometry(1125, 540, 60, 26);

    // Score label
    score = new QLabel(this);
    score->setIndent(-80);
    score->setText("0");
    score->setStyleSheet("QLabel {font-family: Fixedsys;color: white;font-size: 16px;}");
    score->setGeometry(110, 12, 150, 26);

    ready_label = new QLabel(this);
    QImage game_ready;
    game_ready.load(":/game_objects/map_objects/ready.png");
    QPixmap ready_pixmap = QPixmap::fromImage(game_ready);
    ready_label->setPixmap(ready_pixmap);
    ready_label->setScaledContents(true);
    ready_label->setGeometry((width() - ready_label->width()) / 2-10, height() - 75, ready_label->width(), ready_label->height());
    ready_label->hide();

    win_label = new QLabel(this);
    QImage game_win;
    game_win.load(":/game_objects/map_objects/Win.png");
    QPixmap win_pixmap = QPixmap::fromImage(game_win);
    win_label->setPixmap(win_pixmap);
    win_label->setScaledContents(true);
    win_label->setGeometry((width() - win_label->width()) / 2, height() - 75, win_label->width(), win_label->height());
    win_label->hide();

    lose_label = new QLabel(this);
    QImage game_over;
    game_over.load(":/game_objects/map_objects/GameOver.png");
    QPixmap lose_pixmap = QPixmap::fromImage(game_over);
    lose_label->setPixmap(lose_pixmap);
    lose_label->setScaledContents(true);
    lose_label->setGeometry((width() - lose_label->width()*1.35) / 2, height() - 75, lose_label->width()*1.2, lose_label->height()*1.2);
    lose_label->hide();

    panic_label = new QLabel(this);
    QImage panic_icon;
    panic_icon.load(":/game_objects/map_objects/panic.png");
    QPixmap panic_pixmap = QPixmap::fromImage(panic_icon);
    panic_label->setPixmap(panic_pixmap);
    panic_label->setScaledContents(true);
    panic_label->setFixedSize(25, 25);
    panic_label->hide();

    speedboost_label = new QLabel(this);
    QImage speedboost_icon;
    speedboost_icon.load(":/game_objects/map_objects/speedboost.png");
    QPixmap speedboost_pixmap = QPixmap::fromImage(speedboost_icon);
    speedboost_label->setPixmap(speedboost_pixmap);
    speedboost_label->setScaledContents(true);
    speedboost_label->setFixedSize(40, 25);
    speedboost_label->hide();

    speednerf_label = new QLabel(this);
    QImage speednerf_icon;
    speednerf_icon.load(":/game_objects/map_objects/speednerf.png");
    QPixmap speednerf_pixmap = QPixmap::fromImage(speednerf_icon);
    speednerf_label->setPixmap(speednerf_pixmap);
    speednerf_label->setScaledContents(true);
    speednerf_label->setFixedSize(30, 25);
    speednerf_label->hide();

    kaboom_label = new QLabel(this);
    QImage kaboom_icon;
    kaboom_icon.load(":/game_objects/map_objects/kaboom.png");
    QPixmap kaboom_pixmap = QPixmap::fromImage(kaboom_icon);
    kaboom_label->setPixmap(kaboom_pixmap);
    kaboom_label->setScaledContents(true);
    kaboom_label->setFixedSize(25, 25);
    kaboom_label->hide();

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
        restart->show();
        flash_timer->start();
    }
    else if (game->stat == Game::Lose)
    {
        lose_label->show();
        score_timer->stop();
        restart->show();
        flash_timer->start();
    }
}

void MainWindow::display_labels()
{
    int x = 50;
    int y = height() - 25;

    if (game->Panic == true && !game->buffs.contains(panic_label))
    {
        game->buffs.append(panic_label);
    }
    else if (game->Panic == false && game->buffs.contains(panic_label))
    {
        game->buffs.removeOne(panic_label);
        panic_label->hide();
    }

    if (game->SpeedBoost == true && !game->buffs.contains(speedboost_label))
    {
        game->buffs.append(speedboost_label);
    }
    else if (game->SpeedBoost == false && game->buffs.contains(speedboost_label))
    {
        game->buffs.removeOne(speedboost_label);
        speedboost_label->hide();
    }

    if (game->SpeedNerf == true && !game->nerfs.contains(speednerf_label))
    {
        game->nerfs.append(speednerf_label);
    }
    else if (game->SpeedNerf == false && game->nerfs.contains(speednerf_label))
    {
        game->nerfs.removeOne(speednerf_label);
        speednerf_label->hide();
    }

    if (game->Kaboom == true && !game->buffs.contains(kaboom_label))
    {
        game->buffs.append(kaboom_label);
    }
    else if (game->Kaboom == false && game->buffs.contains(kaboom_label))
    {
        game->buffs.removeOne(kaboom_label);
        kaboom_label->hide();
    }

    for (QLabel* buff : game->buffs) {
        buff->move(x, y - buff->height());
        buff->show();
        x += buff->width() + 10; // Adjust for spacing
    }

    x = 100;

    for (QLabel* nerf : game->nerfs) {
        nerf->move(width()- x, y - nerf->height());
        nerf->show();
        x -= nerf->width() - 10; // Adjust for spacing
    }
}



void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (CanMove == true)
     {
    switch (e->key())
    {
    case Qt::Key_Up:
        game->pacman_next_direction(GameObject::Up);
        break;
    case Qt::Key_W:
        game->pacman_next_direction(GameObject::Up);
        break;
    case Qt::Key_Left:
        game->pacman_next_direction(GameObject::Left);
        break;
    case Qt::Key_A:
        game->pacman_next_direction(GameObject::Left);
        break;
    case Qt::Key_Down:
        game->pacman_next_direction(GameObject::Down);
        break;
    case Qt::Key_S:
        game->pacman_next_direction(GameObject::Down);
        break;
    case Qt::Key_Right:
        game->pacman_next_direction(GameObject::Right);
        break;
    case Qt::Key_D:
        game->pacman_next_direction(GameObject::Right);
        break;
    }
    }
}

void MainWindow::start_button()
{
    CanMove = false;
    game->StartupTune->play();
    initial_delay->start();
    start->hide();
    ready_label->show();
    flash_timer->stop();
    is_inverted = false;
    game->start();
}

void MainWindow::start_game()
{
    CanMove = true;
    ready_label->hide();
    initial_delay->stop();
}

void MainWindow::restart_game()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void MainWindow::flash_button()
{
    if (is_inverted == false)
    {
        restart->setIcon(QIcon(restart_icon_inverted));
        start->setIcon((QIcon(start_icon_inverted)));
        is_inverted = true;
        flash_timer->setInterval(RESTART_FLASH_INTERVAL/2);
    }
    else
    {
        restart->setIcon(QIcon(restart_icon));
        start->setIcon((QIcon(start_icon)));
        is_inverted = false;
        flash_timer->setInterval(RESTART_FLASH_INTERVAL);
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}



