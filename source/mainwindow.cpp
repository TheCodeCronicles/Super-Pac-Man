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
    start->hide();
    start->setFocusPolicy(Qt::NoFocus);

    join = new QPushButton(this);
    join_icon = QPixmap(":/game_objects/map_objects/Join_Image.png");
    start_icon_inverted = QPixmap(":/game_objects/map_objects/start_inverted.png");
    join->setIcon(QIcon(join_icon));
    join->setFixedSize(join->size());
    join->setIconSize(join->size());
    join->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
    join->move((width()-join->width())/2 + 100, (height() - 60));
    connect(join, &QPushButton::clicked, this, &MainWindow::join_button);
    join->hide();
    join->setFocusPolicy(Qt::NoFocus);

    host = new QPushButton(this);
    host_icon = QPixmap(":/game_objects/map_objects/Host_Image.png");
    start_icon_inverted = QPixmap(":/game_objects/map_objects/start_inverted.png");
    host->setIcon(QIcon(host_icon));
    host->setFixedSize(host->size());
    host->setIconSize(host->size());
    host->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
    host->move((width()-join->width())/2 - 100, (height() - 60));
    connect(host, &QPushButton::clicked, this, &MainWindow::host_button);
    host->hide();
    host->setFocusPolicy(Qt::NoFocus);

    ret = new QPushButton(this);
    ret_icon = QPixmap(":/game_objects/map_objects/RETURN.png");
    start_icon_inverted = QPixmap(":/game_objects/map_objects/start_inverted.png");
    ret->setIcon(QIcon(ret_icon));
    ret->setFixedSize(ret->size());
    ret->setIconSize(ret->size());
    ret->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
    ret->move((width()-join->width())/2 + 500, (height() - 60));
    connect(ret, &QPushButton::clicked, this, &MainWindow::ret_button);
    ret->hide();
    ret->setFocusPolicy(Qt::NoFocus);

    singlePlayer = new QPushButton(this);
    singleP_icon = QPixmap(":/game_objects/map_objects/SP_ICON.png");
    singlePlayer->setIcon(QIcon(singleP_icon));
    singlePlayer->setFixedSize(300,20); // Adjust these values to your desired size
    singlePlayer->setIconSize(QSize(290, 19)); // Adjust the icon size to fit nicely within the button
    singlePlayer->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
    singlePlayer->move((width() - singlePlayer->width()) / 2, height() - singlePlayer->height() - 60);
    connect(singlePlayer, &QPushButton::clicked, this, &MainWindow::singleP_button);
    singlePlayer->show();
    singlePlayer->setFocusPolicy(Qt::NoFocus);

    multiPlayer = new QPushButton(this);
    multiP_icon = QPixmap(":/game_objects/map_objects/MP_ICON.png");
    multiPlayer->setIcon(QIcon(multiP_icon));
    multiPlayer->setFixedSize(300,20); // Adjust these values to your desired size
    multiPlayer->setIconSize(QSize(290, 19)); // Adjust the icon size to fit nicely within the button
    multiPlayer->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
    multiPlayer->move((width() - multiPlayer->width()) / 2, height() - multiPlayer->height() - 20);
    connect(multiPlayer, &QPushButton::clicked, this, &MainWindow::multiP_button);
    multiPlayer->show();
    multiPlayer->setFocusPolicy(Qt::NoFocus);

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

    // Initialize NetworkManager
    networkManager = new NetworkManager(this);
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
    score_title->hide();

    // Buff label
    Buff_Label = new QLabel(this);
    Buff_Label->setText("BUFFS:");
    Buff_Label->setStyleSheet("QLabel {font-family: Fixedsys;color: Blue;font-size: 32px;}");
    Buff_Label->setGeometry(50, 540, 60, 26);
    Buff_Label->hide();

    // Nerf label
    Nerf_Label = new QLabel(this);
    Nerf_Label->setText("NERFS:");
    Nerf_Label->setStyleSheet("QLabel {font-family: Fixedsys;color: Red;font-size: 32px;}");
    Nerf_Label->setGeometry(1125, 540, 60, 26);
    Nerf_Label->hide();

    // GO label
    GO_label = new QLabel(this);
    GO_label->setText("GO!");
    GO_label->setStyleSheet("QLabel {font-family: Fixedsys;color: Yellow;font-size: 64px;}");
    GO_label->setGeometry((width() - GO_label->width()) / 2+30, height() - 75, GO_label->width(), GO_label->height());
    GO_label->hide();

    // Score label
    score = new QLabel(this);
    score->setIndent(-80);
    score->setText("0");
    score->setStyleSheet("QLabel {font-family: Fixedsys;color: white;font-size: 16px;}");
    score->setGeometry(110, 12, 150, 26);
    score->hide();

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

    Gen_Label = new QLabel(this);
    Gen_Label->setStyleSheet("QLabel {font-family: Fixedsys;color: White;font-size: 32px;}");
    Gen_Label->hide();

    panic_label = new QLabel(this);
    QImage panic_icon;
    panic_icon.load(":/game_objects/map_objects/panic.png");
    QPixmap panic_pixmap = QPixmap::fromImage(panic_icon);
    panic_label->setPixmap(panic_pixmap);
    panic_label->setScaledContents(true);
    panic_label->setFixedSize(30, 30);
    panic_label->hide();

    speedboost_label = new QLabel(this);
    QImage speedboost_icon;
    speedboost_icon.load(":/game_objects/map_objects/speedboost.png");
    QPixmap speedboost_pixmap = QPixmap::fromImage(speedboost_icon);
    speedboost_label->setPixmap(speedboost_pixmap);
    speedboost_label->setScaledContents(true);
    speedboost_label->setFixedSize(45, 30);
    speedboost_label->hide();

    speednerf_label = new QLabel(this);
    QImage speednerf_icon;
    speednerf_icon.load(":/game_objects/map_objects/speednerf.png");
    QPixmap speednerf_pixmap = QPixmap::fromImage(speednerf_icon);
    speednerf_label->setPixmap(speednerf_pixmap);
    speednerf_label->setScaledContents(true);
    speednerf_label->setFixedSize(35, 30);
    speednerf_label->hide();

    kaboom_label = new QLabel(this);
    QImage kaboom_icon;
    kaboom_icon.load(":/game_objects/map_objects/kaboom.png");
    QPixmap kaboom_pixmap = QPixmap::fromImage(kaboom_icon);
    kaboom_label->setPixmap(kaboom_pixmap);
    kaboom_label->setScaledContents(true);
    kaboom_label->setFixedSize(30, 30);
    kaboom_label->hide();

    confusion_label = new QLabel(this);
    QImage confusion_icon;
    confusion_icon.load(":/game_objects/map_objects/confusion.png");
    QPixmap confusion_pixmap = QPixmap::fromImage(confusion_icon);
    confusion_label->setPixmap(confusion_pixmap);
    confusion_label->setScaledContents(true);
    confusion_label->setFixedSize(30, 30);
    confusion_label->hide();

    invisibility_label = new QLabel(this);
    QImage invisibility_icon;
    invisibility_icon.load(":/game_objects/map_objects/invisibility.png");
    QPixmap invisibility_pixmap = QPixmap::fromImage(invisibility_icon);
    invisibility_label->setPixmap(invisibility_pixmap);
    invisibility_label->setScaledContents(true);
    invisibility_label->setFixedSize(30, 30);
    invisibility_label->hide();

    blindness_label = new QLabel(this);
    QImage blindness_icon;
    blindness_icon.load(":/game_objects/map_objects/blindness.png");
    QPixmap blindness_pixmap = QPixmap::fromImage(blindness_icon);
    blindness_label->setPixmap(blindness_pixmap);
    blindness_label->setScaledContents(true);
    blindness_label->setFixedSize(30, 30);
    blindness_label->hide();

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
        Gen_Label->hide();
        win_label->show();
        score_timer->stop();
        restart->show();
        flash_timer->start();
    }
    else if (game->stat == Game::Lose)
    {
        Gen_Label->hide();
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

    if (networkManager->playerJoined == true)
        start->show();

    if (game->Panic == true && !game->buffs.contains(panic_label))
    {
        game->buffs.append(panic_label);
        Gen_Label->setText(powerball_messages[QRandomGenerator::global()->bounded(0,2)]);
        Gen_Label->adjustSize();
        Gen_Label->move((width()-Gen_Label->width())/2, height()-75);
        Gen_Label->show();
    }
    else if (game->Panic == false && game->buffs.contains(panic_label))
    {
        game->buffs.removeOne(panic_label);
        panic_label->hide();
        Gen_Label->hide();
    }

    if (game->SpeedBoost == true && !game->buffs.contains(speedboost_label))
    {
        game->buffs.append(speedboost_label);
        Gen_Label->setText(powerball_messages[QRandomGenerator::global()->bounded(3,5)]);
        Gen_Label->adjustSize();
        Gen_Label->move((width()-Gen_Label->width())/2, height()-75);
        Gen_Label->show();
    }
    else if (game->SpeedBoost == false && game->buffs.contains(speedboost_label))
    {
        game->buffs.removeOne(speedboost_label);
        speedboost_label->hide();
        Gen_Label->hide();
    }

    if (game->Kaboom == true && !game->buffs.contains(kaboom_label))
    {
        game->buffs.append(kaboom_label);
        Gen_Label->setText(powerball_messages[QRandomGenerator::global()->bounded(6,8)]);
        Gen_Label->adjustSize();
        Gen_Label->move((width()-Gen_Label->width())/2, height()-75);
        Gen_Label->show();

    }
    else if (game->Kaboom == false && game->buffs.contains(kaboom_label))
    {
        game->buffs.removeOne(kaboom_label);
        kaboom_label->hide();
        Gen_Label->hide();
    }

    if (game->SpeedNerf == true && !game->nerfs.contains(speednerf_label))
    {
        game->nerfs.append(speednerf_label);
        Gen_Label->setText(powerball_messages[QRandomGenerator::global()->bounded(9,11)]);
        Gen_Label->adjustSize();
        Gen_Label->move((width()-Gen_Label->width())/2, height()-75);
        Gen_Label->show();
    }
    else if (game->SpeedNerf == false && game->nerfs.contains(speednerf_label))
    {
        game->nerfs.removeOne(speednerf_label);
        speednerf_label->hide();
        Gen_Label->hide();
    }

    if (game->Confusion == true && !game->nerfs.contains(confusion_label))
    {
        game->nerfs.append(confusion_label);
        Gen_Label->setText(powerball_messages[QRandomGenerator::global()->bounded(12,14)]);
        Gen_Label->adjustSize();
        Gen_Label->move((width()-Gen_Label->width())/2, height()-75);
        Gen_Label->show();
    }
    else if (game->Confusion == false && game->nerfs.contains(confusion_label))
    {
        game->nerfs.removeOne(confusion_label);
        confusion_label->hide();
        Gen_Label->hide();
    }

    if (game->Invisible == true && !game->buffs.contains(invisibility_label))
    {
        game->buffs.append(invisibility_label);
        Gen_Label->setText(powerball_messages[QRandomGenerator::global()->bounded(15,17)]);
        Gen_Label->adjustSize();
        Gen_Label->move((width()-Gen_Label->width())/2, height()-75);
        Gen_Label->show();
    }
    else if (game->Invisible == false && game->buffs.contains(invisibility_label))
    {
        game->buffs.removeOne(invisibility_label);
        invisibility_label->hide();
        Gen_Label->hide();
    }

    if (game->Blindness == true && !game->nerfs.contains(blindness_label))
    {
        game->nerfs.append(blindness_label);
        Gen_Label->setText(powerball_messages[QRandomGenerator::global()->bounded(18,20)]);
        Gen_Label->adjustSize();
        Gen_Label->move((width()-Gen_Label->width())/2, height()-75);
        Gen_Label->show();
    }
    else if (game->Blindness == false && game->nerfs.contains(blindness_label))
    {
        game->nerfs.removeOne(blindness_label);
        blindness_label->hide();
        Gen_Label->hide();
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
        x += nerf->width() + 10; // Adjust for spacing
    }

    if (game->GO_Away == true)
        GO_label->hide();

    if (networkManager->hostFound == true)
        Gen_Label->hide();

    if (networkManager->playerJoined == true)
        Gen_Label->hide();
}



void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (CanMove == true)
     {
        if (game->Confusion == false)
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
        else
        {
            switch (e->key())
            {
            case Qt::Key_Down:
                game->pacman_next_direction(GameObject::Up);
                break;
            case Qt::Key_S:
                game->pacman_next_direction(GameObject::Up);
                break;
            case Qt::Key_Right:
                game->pacman_next_direction(GameObject::Left);
                break;
            case Qt::Key_D:
                game->pacman_next_direction(GameObject::Left);
                break;
            case Qt::Key_Up:
                game->pacman_next_direction(GameObject::Down);
                break;
            case Qt::Key_W:
                game->pacman_next_direction(GameObject::Down);
                break;
            case Qt::Key_Left:
                game->pacman_next_direction(GameObject::Right);
                break;
            case Qt::Key_A:
                game->pacman_next_direction(GameObject::Right);
                break;
            }
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
    Buff_Label->show();
    Nerf_Label->show();
    score_title->show();
    score->show();
    host->hide();
    join->hide();
    singlePlayer->hide();
    multiPlayer->hide();
    ret->hide();
    game->start();
}

void MainWindow::onConnected()
{
    qDebug() << "Connected!";
    start->show();
}

void MainWindow::onConnectionFailed()
{
    qDebug() << "Connection Failed!";
    // Handle connection failure (e.g., show an error message)
}

void MainWindow::onMessageReceived(const QString &message)
{
    qDebug() << "Message received:" << message;
    // Handle received messages
}

// Slot for handling join button click
void MainWindow::join_button()
{
    isHost = false;
    networkManager->on_joinButton_clicked();
    join->hide();
    host->hide();
    Gen_Label->setText("Listening for host broadcast...");
    Gen_Label->adjustSize();
    Gen_Label->move((width()-Gen_Label->width())/2, height()-75);
    Gen_Label->show();
}

// Slot for handling host button click
void MainWindow::host_button()
{
    isHost = true;
    networkManager->on_hostButton_clicked();
    join->hide();
    host->hide();
    Gen_Label->setText("Hosting a game...");
    Gen_Label->adjustSize();
    Gen_Label->move((width()-Gen_Label->width())/2, height()-75);
    Gen_Label->show();
}

void MainWindow::ret_button()
{
    is_inverted = false;
    singlePlayer->show();
    multiPlayer->show();
    start->hide();
    ret->hide();
    join->hide();
    host->hide();
    Gen_Label->hide();

}
void MainWindow::singleP_button()
{
    is_inverted = false;
    singlePlayer->hide();
    multiPlayer->hide();
    start->show();
    ret->show();
}

void MainWindow::multiP_button()
{
    is_inverted = false;
    singlePlayer->hide();
    multiPlayer->hide();
    host->show();
    join->show();
    ret->show();
}

void MainWindow::start_game()
{
    networkManager->playerJoined = false;
    CanMove = true;
    ready_label->hide();
    initial_delay->stop();
    GO_label->show();
    networkManager->sendData("Game started", networkManager->hostAddress, networkManager->hostPort);
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
        join->setIcon((QIcon(start_icon_inverted)));
        host->setIcon((QIcon(start_icon_inverted)));
        ret->setIcon((QIcon(start_icon_inverted)));
        singlePlayer->setIcon((QIcon(start_icon_inverted)));
        multiPlayer->setIcon((QIcon(start_icon_inverted)));
        is_inverted = true;
        flash_timer->setInterval(RESTART_FLASH_INTERVAL/2);
    }
    else
    {
        restart->setIcon(QIcon(restart_icon));
        start->setIcon((QIcon(start_icon)));
        join->setIcon((QIcon(join_icon)));
        host->setIcon((QIcon(host_icon)));
        ret->setIcon((QIcon(ret_icon)));
        singlePlayer->setIcon((QIcon(singleP_icon)));
        multiPlayer->setIcon((QIcon(multiP_icon)));
        is_inverted = false;
        flash_timer->setInterval(RESTART_FLASH_INTERVAL);
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}





