#include "networkmanager.h"

#include <QNetworkDatagram>
#include <QInputDialog>
#include <QMessageBox>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , udpSocket(new  QUdpSocket(this))
    , hostPort(12345) // Default port
    , broadcastTimer(new QTimer(this))
    , isHost(false)
{
    connect(udpSocket, &QUdpSocket::readyRead, this, &NetworkManager::processPendingDatagrams);
    connect(broadcastTimer, &QTimer::timeout, this, &NetworkManager::broadcastPresence);
}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::on_hostButton_clicked()
{
    udpSocket->bind(QHostAddress::Any, hostPort);
    isHost = true;
    broadcastTimer->start(2000); // Broadcast every 2 seconds
}

void NetworkManager::on_joinButton_clicked()
{
    udpSocket->bind(QHostAddress::Any, hostPort);
}

void NetworkManager::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        QString message = QString::fromUtf8(datagram.data());

        // Ignore messages sent by itself if it's the host
        if (isHost && datagram.senderAddress() == QHostAddress::LocalHost) {
            continue;
        }

        if (message == "Host here") {
            if (!isHost) { // Only the client should process this
                hostAddress = datagram.senderAddress();
                hostFound = true;
                cout << "Joined Host" << endl;
                sendData("Player joined the game.", hostAddress, hostPort);
            }
        } else if (message == "Player joined the game.") {
            if (isHost) {
                playerJoined = true;
                cout << "Player Joined" << endl;
                broadcastTimer->stop(); // Stop broadcasting once a player joins
            }
        } else {
            // ui->messageTextEdit->append("Received: " + message); //QLabel setText!!!!
        }
    }
}

void NetworkManager::broadcastPresence()
{
    if (isHost) {
        sendData("Host here", QHostAddress::Broadcast, hostPort);
    }
}

void NetworkManager::sendData(const QString &message, const QHostAddress &address, quint16 port)
{
    QByteArray data = message.toUtf8();
    udpSocket->writeDatagram(data, address, port);
    // ui->messageTextEdit->append("Sent: " + message); //QLabel setText!!!!
}
