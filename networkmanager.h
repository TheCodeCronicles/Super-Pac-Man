#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    void setupServer();
    void setupClient();
    bool ClientConnected = false;

private:
    QTcpServer *server;
    QTcpSocket *socket;
};

#endif // NETWORKMANAGER_H
