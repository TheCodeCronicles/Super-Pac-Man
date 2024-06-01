#include "networkmanager.h"

#include <QNetworkDatagram>
#include <QInputDialog>
#include <QMessageBox>
#include <QUuid>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , udpSocket(new QUdpSocket(this))
    , hostPort(12345) // Default port
    , broadcastTimer(new QTimer(this))
    , isHost(false)
    , selfId(QUuid::createUuid().toString()) // Unique identifier
{
    connect(udpSocket, &QUdpSocket::readyRead, this, &NetworkManager::processPendingDatagrams);
    connect(broadcastTimer, &QTimer::timeout, this, &NetworkManager::broadcastPresence);
}

NetworkManager::~NetworkManager()
{
    udpSocket->close();
}

void NetworkManager::on_hostButton_clicked()
{
    //udpSocket->close();
    udpSocket->bind(QHostAddress::Any, hostPort);
    isHost = true;
    hostFound = false;
    playerJoined = false;
    broadcastTimer->start(2000); // Broadcast every 2 seconds
}

void NetworkManager::on_joinButton_clicked()
{
    //udpSocket->close();
    udpSocket->bind(QHostAddress::Any, hostPort);
    isHost = false;
    hostFound = false;
    playerJoined = false;
}

void NetworkManager::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        message = QString::fromUtf8(datagram.data());


        // Ignore messages sent by itself using selfId
        if (message.contains(selfId)) {
            continue;
        }

        if (isHost && datagram.senderAddress() == QHostAddress::LocalHost) {
            continue;
        }

        if (message.startsWith("Host here")) {
            if (!isHost) { // Only the client should process this
                hostAddress = datagram.senderAddress();
                hostFound = true;
                cout << "Joined Host" << endl;
                sendData("Player joined the game. " + selfId, hostAddress, hostPort);
            }
        } else if (message.startsWith("Player joined the game.")) {
            if (isHost) {
                playerJoined = true;
                cout << "Player Joined" << endl;
                broadcastTimer->stop(); // Stop broadcasting once a player joins
            }
        } else {
            // Additional message processing if needed
        }


        // Assuming the rest of the message is JSON data
        QByteArray jsonData = message.toUtf8(); // Convert QString to QByteArray
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

        if (!jsonDoc.isNull() && jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            int P1X = jsonObj["X"].toInt();
            int P1Y = jsonObj["Y"].toInt();
            cout << "Received coordinates: " << P1X << ":" << P1Y << endl;
        } else {
            cout << "Failed to parse JSON data" << endl;
        }


    }
}

void NetworkManager::broadcastPresence()
{
    if (isHost) {
        sendData("Host here " + selfId, QHostAddress::Broadcast, hostPort);
    }
}

void NetworkManager::sendData(const QString &message, const QHostAddress &address, quint16 port)
{
    QByteArray data = message.toUtf8();
    udpSocket->writeDatagram(data, address, port);
    // ui->messageTextEdit->append("Sent: " + message); //QLabel setText!!!!
}
