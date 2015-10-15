#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "gamewindow.h"
#include <QTcpServer>
#include <QTcpSocket>

class GameServer : public GameWindow
{
    Q_OBJECT
public:
    GameServer(int fps, gamecamera* camera);
   //~GameServer();

public slots:
    void getSocket();
    void sendSeasons();

private:
    QTcpServer * m_server;
    QTimer*     m_timer;

    QTcpSocket* clients [4];
    int         m_indice;
    int         m_indice_tab;

};

#endif // GAMESERVER_H
