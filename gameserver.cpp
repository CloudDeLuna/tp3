#include "gameserver.h"

GameServer::GameServer(int fps, gamecamera* camera)
    :GameWindow(fps, camera)
{
    m_server = new QTcpServer(this);
    m_indice = 0;
    m_indice_tab = 0;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(sendSeasons()));

    connect(m_server, SIGNAL(newConnection()), this, SLOT(getSocket()));

    if(!m_server->listen(QHostAddress::Any, 9999))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void GameServer::getSocket()
{
    if(m_indice < 4)
    {
        clients[m_indice++] = m_server->nextPendingConnection();
        m_timer->start(5000);
    }
}

void GameServer::sendSeasons()
{
    char* saisons [] = {"HIVER", "PRINTEMPS", "ETE", "AUTOMNE"};

    qDebug() << "sendSeasons";
    for(int i =0; i < 4; ++i)
    {
        clients[i]->write(saisons[(m_indice_tab + i)%4]);
        clients[i]->flush();
    }

    ++m_indice_tab;
    m_timer->start(5000);
}
