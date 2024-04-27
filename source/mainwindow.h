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

private slots:
    void update_score();
    void restart_game();

private:
    Ui::MainWindow *ui;
    QLabel *score_title, *score;
    QLabel *win_label, *lose_label;
    QTimer *score_timer;
    QLabel *kill_mode;
    Game *game;

    QPushButton *restart;
};
#endif // MAINWINDOW_H
