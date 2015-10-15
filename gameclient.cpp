#include "gameclient.h"

GameClient::GameClient(int fps, gamecamera* camera, char* title)
    :GameWindow(fps, camera), m_title(title)
{
    m_socket = new QTcpSocket(this);

    connect(m_socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(m_socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(m_socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(m_socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << "connecting...";

    // this is not blocking call
    m_socket->connectToHost("127.0.0.1", 9999);

    // we need to wait...
    if(!m_socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << m_socket->errorString();
    }

    m_socket->waitForReadyRead();
    m_socket->waitForDisconnected();
}


void GameClient::connected()
{
    qDebug() << "connected...";

    // Hey server, tell me about you.
    //m_socket->write("HEAD / HTTP/1.0\r\n\r\n\r\n\r\n");
    qDebug() << "connected end...";
}

void GameClient::disconnected()
{
    qDebug() << "disconnected...";
}

void GameClient::bytesWritten(qint64 bytes)
{
    //qDebug() << bytes << " bytes written...";
}

void GameClient::readyRead()
{
    // read the data from the socket
    qDebug() << m_title << " : " << m_socket->readAll();
    //m_socket->waitForReadyRead();
}
