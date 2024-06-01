#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QUdpSocket>
#include <QTimer>
#include <iostream>

using namespace std;

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    void sendData(const QString &message, const QHostAddress &address, quint16 port);

    bool hostFound = false;
    bool playerJoined = false;

    QUdpSocket *udpSocket;
    QHostAddress hostAddress;
    quint16 hostPort;
    QTimer *broadcastTimer;
    bool isHost;
    QString selfId;

public slots:
    void on_hostButton_clicked();
    void on_joinButton_clicked();
    void processPendingDatagrams();
    void broadcastPresence();


};

#endif // NETWORKMANAGER_H
