#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) : QObject(parent), socket(new QTcpSocket(this)), server(new QTcpServer(this))
{
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::readMessage);
    connect(socket, &QTcpSocket::connected, this, &NetworkManager::connected);
}

NetworkManager::~NetworkManager()
{
    delete socket;
    delete server;
}

void NetworkManager::startHost()
{
    connect(server, &QTcpServer::newConnection, this, &NetworkManager::handleNewConnection);
    if (server->listen(QHostAddress::Any, 12345)) {
        qDebug() << "Server started";
    } else {
        emit connectionFailed();
    }
}

void NetworkManager::joinServer(const QString &hostAddress)
{
    setupSocket();
    socket->connectToHost(hostAddress, 12345);
}

void NetworkManager::handleNewConnection()
{
    QTcpSocket *clientSocket = server->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &NetworkManager::readMessage);
    socket = clientSocket;
    emit connected();
}

void NetworkManager::readMessage()
{
    while (socket->canReadLine()) {
        QString line = socket->readLine().trimmed();
        emit messageReceived(line);
    }
}

void NetworkManager::setupSocket()
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::readMessage);
    connect(socket, &QTcpSocket::connected, this, &NetworkManager::connected);
}
