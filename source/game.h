#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QPair>
#include <QLabel>
#include <QList>
#include <iostream>
#include "gameobject.h"
#include <QMediaPlayer>

using namespace std;


/* Changeable game options */
#define INITIAL_DELAY   5000
#define BALL_SCORE      10          // score of balls
#define POWERBALL_SCORE 30          // score of powerballs
#define GHOST_SCORE     50          // score of ghosts
#define INTERVAL        10          // move interval of pacman
#define NORMAL_INTERVAL 10          // move interval of normal ghosts
#define PANNIC_INTERVAL 15          // move interval of pannic ghosts
#define RUNNING_INTERVAL 5          // move interval of running ghosts
#define PANNIC_TIME     500        // interval number of pannic ghosts
#define FLASH_INTERVAL 200          // flash interval of powerballs
#define RESTART_FLASH_INTERVAL 600  // flash interval for restart button
#define SPEED_BOOST_DURATION 5000   // Speed boost duration in milliseconds (adjust as needed)
#define SPEED_NERF_DURATION 5000
#define CONFUSION_DURATION 5000// Speed nerf duration in milliseconds (adjust as needed)
#define MAX_GHOST_RETREAT_TIME 10000 // Maximum time a ghost is allowed to retreat to cage (To avoid ghosts getting stuck)

/* Game control class */
class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    enum GameStatus {Playing, Win, Lose, Pause};
    Game(int, int, int, int, QString);
    ~Game();
    void start();
    void stop();
    const static int PowerballNum = 5;
    void pacman_next_direction(GameObject::Dir);
    int get_score();
    char gen_probabalistic_char();
    bool Panic = false;
    bool SpeedBoost = false;
    bool SpeedNerf = false;
    bool Confusion = false;
    bool Kaboom = false;
    bool GO_Away = false;


    GameObject ***map;                                                  // the map of pacman game
    GameObject *gate;                                                   // pointer of the gate of cage of ghosts
    Pacman *pacman;                                                     // pointer of pacman
    Ghost *ghost[Ghost::GhostNum];                                      // pointers of ghosts
    char powerballs[Game::PowerballNum] = {'4', '5', '6', '7', '8'};         // powerball definition characters (for case statement)
    double probabilities[Game::PowerballNum] = {0.4, 0.2, 0.15, 0.15, 0.1};  // probabilities of powerballs spawning
    QVector<GameObject*> powerball;                                     // pointers of powerball
    QVector<GameObject*> powerball02;                                   // pointers of powerball 2
    QVector<GameObject*> powerball03;                                   // pointers of powerball 3
    QVector<GameObject*> powerball04;                                   // pointers of powerball 4
    QVector<GameObject*> powerball05;                                   // pointers of powerball 5
    GameStatus stat;
    QMediaPlayer *StartupTune;
    QMediaPlayer *DeathTune;
    QList<QLabel*> buffs;
    QList<QLabel*> nerfs;


    friend class Pacman;
    friend class Ghost;


private slots:
    void pacman_handler();
    void ghost_retreat_handler(int);
    void powerball_flash();
    void powerball02_flash();
    void powerball03_flash();
    void powerball04_flash();
    void powerball05_flash();
    void ghost_handler(int);
    void stop_speed_boost();
    void stop_speed_nerf();
    void stop_confusion();

private:
    int map_height, map_width;      // game map (20 x 29 in this app)
    int map_size;                   // map_width * map_height
    int ball_num, eat_num, score;
    int geo_x, geo_y;               // geometric coordinate
    bool retreat[Ghost::GhostNum] = {true, true, true, true};
    bool retreat_timer_running[Ghost::GhostNum] = {false, false, false, false};
    bool panic_tune_playing = false;

    QTimer *ghost_timer[Ghost::GhostNum];
    QTimer *ghost_retreat_timer[Ghost::GhostNum];
    QTimer *pacman_timer;
    QTimer *powerball_flash_timer;
    QTimer *powerball02_flash_timer;
    QTimer *speed_boost_timer;
    QTimer *speed_nerf_timer;
    QTimer *confusion_timer;
    QTimer *powerball03_flash_timer;
    QTimer *powerball04_flash_timer;
    QTimer *powerball05_flash_timer;
    bool flash_tick;
    bool flash02_tick;
    bool flash03_tick;
    bool flash04_tick;
    bool flash05_tick;
    int GeneralCounter = 0;


};

#endif // GAME_H
