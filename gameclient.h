#ifndef GAMECLIENT_H
#define GAMECLIENT_H


#include "gamewindow.h"
#include <QTcpServer>
#include <QTcpSocket>

class GameClient : public GameWindow
{
    Q_OBJECT
public:
    GameClient(int fps, gamecamera* camera, char* title);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket *m_socket;
    QTcpServer *m_server;
    char* m_title;
};

#endif // GAMECLIENT_H
