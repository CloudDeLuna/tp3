#include "thread.h"

thread::thread(gamecamera* camera, char* title)
    : m_camera(camera), m_title(title)
{

}

void thread::run()
{
    QSurfaceFormat format;
    format.setSamples(16);

    GameClient window (120, m_camera, m_title);
    window.setFormat(format);
    window.setTitle(m_title);
    window.resize(320, 240);
    window.show();
    //wait(50000);
    window.setAnimating(true);
}

