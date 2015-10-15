#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include "gameserver.h"
#include "gameclient.h"

class thread : public QThread
{
public:
    thread(gamecamera *camera, char* title);

private:
    void run();
    gamecamera* m_camera;
    char* m_title;
};

#endif // THREAD_H
