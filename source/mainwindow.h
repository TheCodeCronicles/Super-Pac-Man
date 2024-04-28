#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QKeyEvent>
#include <QTimer>
#include <QPushButton>
#include <QProcess>
#include "game.h"

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

private slots:
    void update_score();
    void restart_game();
    void flash_button();

private:
    Ui::MainWindow *ui;
    QLabel *score_title, *score;
    QLabel *win_label, *lose_label;
    QTimer *score_timer;
    QTimer *flash_timer;
    QLabel *kill_mode;
    Game *game;
    QPixmap restart_icon;
    QPixmap restart_icon_inverted;

    QPushButton *restart;
};
#endif // MAINWINDOW_H
