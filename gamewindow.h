#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "openglwindow.h"
#include "gamecamera.h"
#include <QTimer>

#define HIVER 0
#define PRINTEMPS 1
#define ETE 2
#define AUTOMNE 3

#define RIEN 0
#define NORMALE 1
#define PLUIE 2
#define NEIGE 3

struct point
{
    float x, y ,z;
};

class GameWindow : public OpenGLWindow
{
public:
    GameWindow(int fps, gamecamera* camera, int saison = PRINTEMPS);

    void initialize();
    void render();
    bool event(QEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void displayTriangles();
    void displayLines();
    void displayTrianglesC();
    void displayPoints();
    void displayTrianglesTexture();

    void displayColor(float);
    void loadMap(QString localPath);
    void setCamera(gamecamera* camera);
    gamecamera* getCamera();

    void initAll();
    void initEntity(int index);
    void update();

protected:

    int m_frame;
    QImage m_image;
    point *p;

    int carte=1;
    gamecamera* m_camera;
    bool m_hasToRotate = false;
    QTimer* m_timer;
    int m_fps;
    int m_temps = RIEN;
    int m_saison = PRINTEMPS;
};


#endif // GAMEWINDOW_H
