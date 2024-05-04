#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{

}

void NetworkManager::setupServer()
{
    // Server setup
    server = new QTcpServer(this);
    if (!server->listen(QHostAddress::Any, 1234)) {
        qDebug() << "Error: Unable to start server";
        // Handle error, throw exception, etc.
    } else {
        qDebug() << "Server started successfully!";
    }
}

void NetworkManager::setupClient()
{
    // Client setup
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 1234);
    if (!socket->waitForConnected()) {
        qDebug() << "Error: Unable to connect to server";
        // Handle error, throw exception, etc.
    } else {
        qDebug() << "Connected to server!";
    }
}
