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

    bool hostFound = false;
    bool playerJoined = false;

public slots:
    void on_hostButton_clicked();
    void on_joinButton_clicked();
    void processPendingDatagrams();
    void broadcastPresence();

private:
    QUdpSocket *udpSocket;
    QHostAddress hostAddress;
    quint16 hostPort;
    QTimer *broadcastTimer;
    bool isHost;
    QString selfId; // Unique identifier for this instance

    void sendData(const QString &message, const QHostAddress &address, quint16 port);
};

#endif // NETWORKMANAGER_H
