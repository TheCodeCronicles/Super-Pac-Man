#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    void startHost();
    void joinServer(const QString &hostAddress);

    QTcpSocket *socket;
    QTcpServer *server;

signals:
    void connected();
    void connectionFailed();
    void messageReceived(const QString &message);

private slots:
    void handleNewConnection();
    void readMessage();

private:
    void setupSocket();
};

#endif // NETWORKMANAGER_H
