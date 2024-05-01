#include "game.h"
#include <QString>
#include <QFile>
#include <QRandomGenerator>
#define W (GameObject::Width)


// interval number before ghosts going out the cage
int GHOST_RELEASE_TIME[] = {500, 700, 900, 1100};

Game::Game(int x, int y, int map_w, int map_h, QString map_src)
    : QGraphicsScene(x, y, W * map_w, W * map_h)
{
    geo_x = x;
    geo_y = y;
    stat = Playing;

    /* Initialize map pointers */
    map_size = map_w * map_h;
    map_width = map_w;
    map_height = map_h;
    map = new GameObject**[map_height];
    for (int i = 0; i < map_height; i++)
    {
        map[i] = new GameObject*[map_width];
        for (int j = 0; j < map_width; j++)
            map[i][j] = nullptr;
    }

    /* Initialize map graphics */
    ball_num = eat_num = score = 0;
    int ghost_i = 0;
    QPixmap wallpix(":/game_objects/map_objects/wall.png");
    QPixmap ballpix(":/game_objects/map_objects/dot.png");
    QPixmap powerballpix(":/game_objects/map_objects/power_ball.png");
    QPixmap powerball02pix(":/game_objects/map_objects/power_ball_02.png");
    QPixmap powerball03pix(":/game_objects/map_objects/power_ball_03.png");
    QPixmap powerball04pix(":/game_objects/map_objects/power_ball_04.png");
    QPixmap gatepix(":/game_objects/map_objects/gate.png");
    QPixmap blankpix;
    QFile mapfile(map_src);
    mapfile.open(QIODevice::ReadOnly|QIODevice::Text);

    // PLAY STARTING TUNE
    StartupTune = new QMediaPlayer();
    StartupTune->setMedia(QUrl("qrc:/game_objects/Sounds/StartupTune.wav"));

    // PLAY STARTING TUNE
    DeathTune = new QMediaPlayer();
    DeathTune->setMedia(QUrl("qrc:/game_objects/Sounds/pacman_death.wav"));

    // Create a new instance of Pacman
    pacman = new Pacman();

    // Loop through each row of the map
    for (int i = 0; i < map_h; i++)
    {
        // Read a line from the map file
        QByteArray line = mapfile.readLine();
        for (int j = 0; j < map_w; j++)
        {
            // Calculate the temporary x and y coordinates based on the current position in the loop
            int tmp_x = x + (j * W);
            int tmp_y = y + (i * W);
            if (line[j] == '4')
            {
                //line[j] = QRandomGenerator::global()->bounded('4', '7');
                line[j] = gen_probabalistic_char();
                cout << line[j] << endl;
            }

            switch (line[j])
            {
            case '1':
                map[i][j] = new GameObject(GameObject::Wall, wallpix);
                map[i][j]->setPos(tmp_x, tmp_y);
                addItem(map[i][j]);
                break;
            case '0':
                map[i][j] = new GameObject(GameObject::Ball, ballpix);
                map[i][j]->set_score(BALL_SCORE);
                map[i][j]->setPos(tmp_x, tmp_y);
                addItem(map[i][j]);
                ball_num++;
                break;
            case '4':
                map[i][j] = new GameObject(GameObject::PowerBall, powerballpix);
                map[i][j]->set_score(POWERBALL_SCORE);
                map[i][j]->setPos(tmp_x, tmp_y);
                addItem(map[i][j]);
                powerball.push_back(map[i][j]);
                ball_num++;
                break;
            case '5':
                map[i][j] = new GameObject(GameObject::PowerBall02, powerball02pix);
                map[i][j]->set_score(POWERBALL_SCORE);
                map[i][j]->setPos(tmp_x, tmp_y);
                addItem(map[i][j]);
                powerball02.push_back(map[i][j]);
                ball_num++;
                break;
            case '6':
                map[i][j] = new GameObject(GameObject::PowerBall03, powerball03pix);
                map[i][j]->set_score(POWERBALL_SCORE);
                map[i][j]->setPos(tmp_x, tmp_y);
                addItem(map[i][j]);
                powerball03.push_back(map[i][j]);
                ball_num++;
                break;
            case '7':
                map[i][j] = new GameObject(GameObject::PowerBall04, powerball04pix);
                map[i][j]->set_score(POWERBALL_SCORE);
                map[i][j]->setPos(tmp_x, tmp_y);
                addItem(map[i][j]);
                powerball04.push_back(map[i][j]);
                ball_num++;
                break;
            case '3':
                map[i][j] = new GameObject(GameObject::Blank, blankpix);
                break;
            case '2':
                gate = new GameObject(GameObject::Gate, gatepix);
                gate->_x = j;
                gate->_y = i;
                gate->setPos(tmp_x, tmp_y);
                addItem(gate);
                map[i][j] = gate;
                break;
            case 'p':
                pacman = new Pacman();
                pacman->game = this;
                pacman->setZValue(2);
                pacman->setPos(tmp_x, tmp_y);
                addItem(pacman);
                map[i][j] = pacman;
                break;
            case 'g':
                map[i][j] = new GameObject(GameObject::Blank, blankpix);
                ghost[ghost_i] = new Ghost(ghost_i);
                ghost[ghost_i]->game = this;
                ghost[ghost_i]->setZValue(2);
                ghost[ghost_i]->release_time = GHOST_RELEASE_TIME[ghost_i];
                ghost[ghost_i]->_x = j;
                ghost[ghost_i]->_y = i;
                ghost[ghost_i]->set_score(GHOST_SCORE);
                ghost[ghost_i]->setPos(tmp_x, tmp_y);
                addItem(ghost[ghost_i]);
                ghost_i++;
                break;
            }
            if (map[i][j])
            {
                map[i][j]->_x = j;
                map[i][j]->_y = i;
            }
        }
    }

    ghost[Ghost::Red]->chase_strategy = &strategy1;
    ghost[Ghost::Pink]->chase_strategy = &strategy2;
    ghost[Ghost::Green]->chase_strategy = &strategy3;
    ghost[Ghost::Yellow]->chase_strategy = &strategy4;
}


void Game::start()
{
    powerball_flash_timer = new QTimer(this);
    connect(powerball_flash_timer, SIGNAL(timeout()), this , SLOT(powerball_flash()));
    powerball_flash_timer->start(FLASH_INTERVAL);

    powerball02_flash_timer = new QTimer(this);
    connect(powerball02_flash_timer, SIGNAL(timeout()), this , SLOT(powerball02_flash()));
    powerball02_flash_timer->start(FLASH_INTERVAL);

    speed_boost_timer = new QTimer(this);
    connect(speed_boost_timer, SIGNAL(timeout()), this, SLOT(stop_speed_boost()));
    speed_boost_timer->setInterval(SPEED_BOOST_DURATION);

    speed_nerf_timer = new QTimer(this);
    connect(speed_nerf_timer, SIGNAL(timeout()), this, SLOT(stop_speed_nerf()));
    speed_nerf_timer->setInterval(SPEED_NERF_DURATION);

    powerball03_flash_timer = new QTimer(this);
    connect(powerball03_flash_timer, SIGNAL(timeout()), this , SLOT(powerball03_flash()));
    powerball03_flash_timer->start(FLASH_INTERVAL);

    powerball04_flash_timer = new QTimer(this);
    connect(powerball04_flash_timer, SIGNAL(timeout()), this , SLOT(powerball04_flash()));
    powerball04_flash_timer->start(FLASH_INTERVAL);

    pacman_timer = new QTimer(this);
    connect(pacman_timer, SIGNAL(timeout()), this , SLOT(pacman_handler()));
    pacman_timer->start(INTERVAL);

    for (int i = 0; i < Ghost::GhostNum; i++)
    {
        ghost_timer[i] = new QTimer(this);
        // Managed to pass ghost id to ghost_handler.
        connect(ghost_timer[i], &QTimer::timeout, [=](){ghost_handler(i);} );
        ghost_timer[i]->start(NORMAL_INTERVAL);

        ghost_retreat_timer[i] = new QTimer(this);
        connect(ghost_retreat_timer[i], &QTimer::timeout, [=](){ghost_retreat_handler(i);});
        ghost_retreat_timer[i]->setInterval(MAX_GHOST_RETREAT_TIME);
    }   
}


void Game::stop()
{    
    DeathTune->play();

    pacman_timer->stop();
    powerball_flash_timer->stop();
    powerball02_flash_timer->stop();
    powerball03_flash_timer->stop();
    powerball04_flash_timer->stop();

    for (int i = 0; i < Ghost::GhostNum; i++)
    {
        ghost_timer[i]->stop();
        ghost_retreat_timer[i]->stop();
    }
}

char Game::gen_probabalistic_char()
{
    double cumulative = 0.0;
    double cumulative_probabilities[Game::PowerballNum];
    for (int i = 0 ; i < Game::PowerballNum ; ++i)
    {
        cumulative += probabilities[i];
        cumulative_probabilities[i] = cumulative;
    }

    double random_value = QRandomGenerator::global()->generateDouble() * cumulative;

    for (int i = 0 ; i < Game::PowerballNum ; ++i)
    {
        if (random_value < cumulative_probabilities[i])
        {
            return powerballs[i];
        }
    }

    return '4';
}


void Game::powerball_flash()
{
    if (powerball.empty())
    {
        powerball_flash_timer->stop();
        return;
    }

    if (flash_tick)
    {
        for (int i = 0; i < powerball.size(); i++)
        {
            powerball.at(i)->hide();
        }
        flash_tick = 0;
    }
    else
    {
        for (int i = 0; i < powerball.size(); i++)
        {
            powerball.at(i)->show();
        }
        flash_tick = 1;
    }
}

void Game::powerball02_flash()
{
    if (powerball02.empty())
    {
        powerball02_flash_timer->stop();
        return;
    }

    if (flash02_tick)
    {
        for (int i = 0; i < powerball02.size(); i++)
        {
            powerball02.at(i)->hide();
        }
        flash02_tick = 0;
    }
    else
    {
        for (int i = 0; i < powerball02.size(); i++)
        {
            powerball02.at(i)->show();
        }
        flash02_tick = 1;
    }
}

void Game::stop_speed_boost()
{
    SpeedBoost = false;
    speed_boost_timer->stop();
}

void Game::stop_speed_nerf()
{
    SpeedNerf = false;
    speed_nerf_timer->stop();
}

void Game::powerball03_flash()
{
    if (powerball03.empty())
    {
        powerball03_flash_timer->stop();
        return;
    }

    if (flash03_tick)
    {
        for (int i = 0; i < powerball03.size(); i++)
        {
            powerball03.at(i)->hide();
        }
        flash03_tick = 0;
    }
    else
    {
        for (int i = 0; i < powerball03.size(); i++)
        {
            powerball03.at(i)->show();
        }
        flash03_tick = 1;
    }
}

void Game::powerball04_flash()
{
    if (powerball04.empty())
    {
        powerball04_flash_timer->stop();
        return;
    }

    if (flash04_tick)
    {
        for (int i = 0; i < powerball04.size(); i++)
        {
            powerball04.at(i)->hide();
        }
        flash04_tick = 0;
    }
    else
    {
        for (int i = 0; i < powerball04.size(); i++)
        {
            powerball04.at(i)->show();
        }
        flash04_tick = 1;
    }
}

void Game::pacman_handler()
{
    pacman->move();
    if (stat == Win)
    {
        stop();
    }
}


void Game::ghost_handler(int ghost_id)
{
    ghost[ghost_id]->move();
    if (stat == Lose)
    {
        stop();
    }

    if (ghost[0]->status == Ghost::Normal
        && ghost[1]->status == Ghost::Normal
        && ghost[2]->status == Ghost::Normal
        && ghost[3]->status == Ghost::Normal)
    {
        Kaboom = false;
    }
}

void Game::ghost_retreat_handler(int ghost_id)
{
    retreat[ghost_id] = false;
    cout << "Ghost " << ghost_id << " respawned at gate due to being stuck in a loop"<< endl;
    ghost[ghost_id]->move();
    ghost_retreat_timer[ghost_id]->stop();
}


void Game::pacman_next_direction(GameObject::Dir d)
{
    pacman->set_next_dir(d);
}

int Game::get_score()
{
    return score;
}

Game::~Game()
{
    for (int i = 0; i < map_height; i++)
    {
        for (int j = 0; j < map_width; j++)
        {
            if (map[i][j] != nullptr)
                delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
    delete pacman_timer;
    delete powerball_flash_timer;
    delete powerball02_flash_timer;
    delete powerball03_flash_timer;
    delete powerball04_flash_timer;
    for (int i = 0; i < Ghost::GhostNum; i++)
    {
        delete ghost_timer[i];
        delete ghost_retreat_timer[i];
    }
}
