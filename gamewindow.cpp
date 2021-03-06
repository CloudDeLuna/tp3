#include "gamewindow.h"

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

using namespace std;

#define MAX_PARTICLES 10000

typedef struct
{
    float x;
    float y;
    float z;
} particle;

particle Particles[MAX_PARTICLES];

GameWindow::GameWindow(int fps, gamecamera* camera, int saison)
    : m_saison(saison)
{
    this->m_camera = camera;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(renderNow()));
    m_timer->start(1000/fps);
    this->m_fps = fps;
}

void GameWindow::initialize()
{
    const qreal retinaScale = devicePixelRatio();

    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);

    loadMap(":/heightmap-2.png");
    initAll();
}

void GameWindow::loadMap(QString localPath)
{
    if (QFile::exists(localPath))
    {
        m_image = QImage(localPath);
    }

    uint id = 0;
    p = new point[m_image.width() * m_image.height()];
    QRgb pixel;
    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {

            pixel = m_image.pixel(i,j);

            id = i*m_image.width() +j;

            p[id].x = (float)i/(m_image.width()) - ((float)m_image.width()/2.0)/m_image.width();
            p[id].y = (float)j/(m_image.height()) - ((float)m_image.height()/2.0)/m_image.height();
            p[id].z = 0.001f *(float)(qRed(pixel));
        }
    }
}

gamecamera* GameWindow::getCamera()
{
    return this->m_camera;
}

void GameWindow::setCamera(gamecamera* camera)
{
    this->m_camera = camera;
}

void GameWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    update();
    glLoadIdentity();
    //float ss = this->camera->getScale();
    //glScalef(ss,ss,ss);
    m_camera->scale();
    glRotatef(this->m_camera->getRotX(),1.0f,0.0f,0.0f);

    if(this->m_hasToRotate)
        this->m_camera->setRotY(this->m_camera->getRotY() + 1.0f);

    glRotatef(this->m_camera->getRotY(),0.0f,0.0f,1.0f);

    switch(this->m_camera->getEtat())
    {
    case 0:
        displayPoints();
        break;
    case 1:
        displayLines();
        break;
    case 2:
        displayTriangles();
        break;
    case 3:
        displayTrianglesC();
        break;
    case 4:
        displayTrianglesTexture();
        break;
    case 5:
        displayLines();
        displayTrianglesTexture();
        break;
    default:
        displayPoints();
        break;
    }

    ++m_frame;
    if(m_camera->m_temps != RIEN)
    {
        if(m_camera->m_temps == PLUIE || (m_camera->m_temps == NORMALE && m_saison == AUTOMNE))
        {
            glColor3f(0, 0, 1);
            glBegin(GL_POINTS);

            for(int i = 0; i <= MAX_PARTICLES; i++)
            {
                glVertex3f(Particles[i].x, Particles[i].y, Particles[i].z);
            }
            glEnd();

            glPointSize(1);
        }
        else if(m_camera->m_temps == NEIGE || (m_camera->m_temps == NORMALE && m_saison == HIVER))
        {
            glPointSize(2);
            glColor3f(1, 1, 1);

            glBegin(GL_POINTS);

            for(int i = 0; i <= MAX_PARTICLES; i++)
            {
                glVertex3f(Particles[i].x, Particles[i].y, Particles[i].z);
            }
            glEnd();

            glPointSize(1);
        }
    }
}

bool GameWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:

        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case 'Z':
        this->m_camera->setScale(this->m_camera->getScale()+0.10f);
        break;
    case 'S':
        this->m_camera->setScale(this->m_camera->getScale()-0.10f);
        break;
    case 'A':
        this->m_camera->setRotX(this->m_camera->getRotX()+1.0f);
        break;
    case 'E':
        this->m_camera->setRotX(this->m_camera->getRotX()-1.0f);
        break;
    case 'Q':
        this->m_camera->setRotY(this->m_camera->getRotY()+1.0f);
        break;
    case 'D':
        this->m_camera->setRotY(this->m_camera->getRotY()-1.0f);
        break;
    case 'W':
        this->m_camera->setEtat((this->m_camera->getEtat() + 1) % 6);
        break;
    case 'C':
        this->m_hasToRotate = !this->m_hasToRotate;
        break;
    case 'V':
        this->m_saison = (m_saison+ 1)%4;
        break;
    case 'F':
        this->m_camera->m_temps = (m_camera->m_temps+1) %4;
        break;
    case 'P':
        if(this->m_fps < 2000)
            this->m_fps *= 2;

        this->m_timer->stop();
        this->m_timer->start(1000/m_fps);
        break;
    case 'M':
        if(this->m_fps > 2)
            this->m_fps /= 2;

        this->m_timer->stop();
        this->m_timer->start(1000/m_fps);
        break;
    case 'X':
        carte++;
        if(carte > 3)
            carte = 1;
        QString depth (":/heightmap-");
        depth += QString::number(carte) ;
        depth += ".png" ;

        loadMap(depth);
        break;
    }
    //renderNow();
}


void GameWindow::displayPoints()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    uint id = 0;
    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {
            id = i*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

        }
    }
    glEnd();
}


void GameWindow::displayTriangles()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {

            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);



            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }

    glEnd();
}

void GameWindow::displayTrianglesC()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);


            glColor3f(1.0f, 1.0f, 1.0f);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }
    glEnd();
}

void GameWindow::displayLines()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {

            id = i*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = i*m_image.width() +(j+1);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +j+1;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +(j);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }

    glEnd();
}

void GameWindow::displayTrianglesTexture()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {

            id = i*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);



            id = i*m_image.width() +(j+1);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }
    glEnd();
}


void getPrintempsColor(float z)
{
    if(z < 10.0f * 0.001f)
        glColor3f(65/255.0f, 105/255.0f, 225/255.0f);//Bleu foncé
    else if(z < 50.0f* 0.001f)
        glColor3f(46/255.0f, 160/255.0f, 87/255.0f);//Vert
    else if(z < 200.0f* 0.001f)
        glColor3f(126/255.0f, 88/255.0f, 53/255.0f);//Marron
    else
        glColor3f(220/255.0f, 220/255.0f, 220/255.0f);//Gris clair
}

void getEteColor(float z)
{
    if(z < 10.0f * 0.001f)
        glColor3f(65/255.0f, 105/255.0f, 225/255.0f);//Bleu foncé
    else if(z < 50.0f* 0.001f)
        glColor3f(20/255.0f, 139/255.0f, 40/255.0f);//Vert
    else //if(z < 130.0f* 0.001f)
        glColor3f(126/255.0f, 88/255.0f, 53/255.0f);//Marron
}

void getAutomneColor(float z)
{
    if(z < 10.0f * 0.001f)
        glColor3f(65/255.0f, 105/255.0f, 225/255.0f);//Bleu foncé
    else if(z < 35.0f* 0.001f)
        glColor3f(46/255.0f, 90/255.0f, 87/255.0f);//Vert foncé
    else if(z < 80.0f* 0.001f)
        glColor3f(247/255.0f, 166/255.0f, 59/255.0f);//Orange
    else if(z < 130.0f* 0.001f)
        glColor3f(126/255.0f, 88/255.0f, 53/255.0f);//Marron
    else// if(z < 200.0f* 0.001f)
        glColor3f(220/255.0f, 220/255.0f, 220/255.0f);//Gris clair
}

void getHiverColor(float z)
{
    if(z < 10.0f * 0.001f)
        glColor3f(150/255.0f, 150/255.0f, 225/255.0f);//Bleu foncé
    else if(z < 50.0f* 0.001f)
        glColor3f(150/255.0f, 225/255.0f, 150/255.0f);//Vert
    else if(z < 60.0f* 0.001f)
        glColor3f(126/255.0f, 88/255.0f, 53/255.0f);//Marron
    else
        glColor3f(1, 1, 1);//Blanc
}

void GameWindow::displayColor(float alt)
{
    if(m_saison == PRINTEMPS)
        getPrintempsColor(alt);
    else if(m_saison == ETE)
        getEteColor(alt);
    else if(m_saison == AUTOMNE)
        getAutomneColor(alt);
    else if(m_saison == HIVER)
        getHiverColor(alt);
}

void GameWindow::initAll()
{
    for(int i = 0; i <= MAX_PARTICLES; i++)
    {
        Particles[i].x = -0.5 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5+0.5)));
        Particles[i].y = -0.5+ static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5+0.5)));
        Particles[i].z = 1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(-1)));
    }
}

void GameWindow::initEntity(int i)
{
    Particles[i].x = -0.5 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5+0.5)));
    Particles[i].y = -0.5+ static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5+0.5)));
    Particles[i].z = 1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(-1)));
}

void GameWindow::update()
{
    if(m_camera->m_temps == PLUIE || (m_camera->m_temps == NORMALE && m_saison == AUTOMNE) )
    {
        for(int i = 0; i <= MAX_PARTICLES; i++)
        {
            Particles[i].z -= (rand() % 20*0.001f);

            if(Particles[i].z <= 0.1)
            {
                initEntity(i);
            }
        }
    }
    else if(m_camera->m_temps == NEIGE || (m_camera->m_temps == NORMALE && m_saison == HIVER) )
    {
        for(int i = 0; i <= MAX_PARTICLES; i++)
        {
            Particles[i].x += -0.005+ static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.005+0.005)));
            Particles[i].z -= (rand() % 10*0.001f);

            if(Particles[i].z <= 0.1)
            {
                initEntity(i);
            }
        }
    }
}
