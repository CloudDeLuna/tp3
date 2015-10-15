#include "gameserver.h"
#include "gameclient.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <time.h>
#include <sys/time.h>
#include <iostream>

#include <QtCore>
#include <QtGui>

#include "thread.h"

using namespace std;

int main(int argc, char **argv)
{
    srand(time(NULL));
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    gamecamera* camera = new gamecamera();
    camera->m_temps = RIEN;
    /**********Partie 1******************/
    GameServer window (120, camera);
    window.setFormat(format);
    window.setTitle("Serveur");
    window.resize(320, 240);
    window.show();
    window.setAnimating(true);

    thread client(camera, "client 1");
    client.start();

    thread client2(camera, "client 2");
    client2.start();

    thread client3(camera, "client 3");
    client3.start();

    thread client4(camera, "client 4");
    client4.start();

    /*************Partie 2***************/
    /* <--- Supprimer cette ligne et commenter la partie 1 pour tester la partie 2
    GameWindow window1 (120, camera, PRINTEMPS);
    window1.setFormat(format);
    window1.setTitle("Printemps");
    window1.resize(640, 480);
    window1.show();
    window1.setAnimating(true);

    GameWindow window2 (120, camera, ETE);
    window2.setFormat(format);
    window2.setTitle("Ete");
    window2.resize(640, 480);
    window2.show();
    window2.setAnimating(true);

    GameWindow window3 (120, camera, AUTOMNE);
    window3.setFormat(format);
    window3.setTitle("Automne");
    window3.resize(640, 480);
    window3.show();
    window3.setAnimating(true);

    GameWindow window4 (120, camera, HIVER);
    window4.setFormat(format);
    window4.setTitle("Hiver");
    window4.resize(640, 480);
    window4.show();
    window4.setAnimating(true);

    /********************************************/

    return app.exec();
}



