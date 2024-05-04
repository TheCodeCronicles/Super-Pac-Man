#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QKeyEvent>
#include <QTimer>
#include <QPushButton>
#include <QProcess>
#include "game.h"
#include "networkmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initLabels();
    void keyPressEvent(QKeyEvent*) override;
    bool is_inverted = false;
    bool isHost = false;

private slots:
    void update_score();
    void start_button();
    void join_button();
    void start_game();
    void restart_game();
    void flash_button();
    void display_labels();
    void startNetwork();

private:
    Ui::MainWindow *ui;
    QLabel *score_title, *score;
    QLabel *win_label,
           *lose_label,
           *start_label,
           *ready_label,
           *Buff_Label,
           *Nerf_Label,
           *panic_label,
           *speedboost_label,
           *speednerf_label,
           *kaboom_label,
           *GO_label;
    QTimer *score_timer;
    QTimer *label_display_timer;
    QTimer *kaboom;
    QTimer *flash_timer;
    QTimer *initial_delay;
    QLabel *kill_mode;
    Game *game;
    QPixmap restart_icon, restart_icon_inverted, start_icon, start_icon_inverted, join_icon, join_icon_inverted;
    bool CanMove = false;

    QPushButton *start;
    QPushButton *join;
    QPushButton *restart;
    NetworkManager *networkManager;
};
#endif // MAINWINDOW_H
