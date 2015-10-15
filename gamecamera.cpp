#include "gamecamera.h"

#include <QtCore>
#include <QtGui>

gamecamera::gamecamera()
{

}

gamecamera::scale()
{
    glScalef(this->m_ss,this->m_ss,this->m_ss);
}

int gamecamera::getEtat()
{
    return this->m_etat;
}

float gamecamera::getRotX()
{
    return this->m_rotX;
}

float gamecamera::getRotY()
{
    return this->m_rotY;
}

float gamecamera::getScale()
{
    return this->m_ss;
}

void gamecamera::setEtat(int etat)
{
    this->m_etat = etat;
}

void gamecamera::setRotX(float rotX)
{
    this->m_rotX = rotX;
}

void gamecamera::setRotY(float rotY)
{
    this->m_rotY = rotY;
}

void gamecamera::setScale(float scale)
{
    this->m_ss = scale;
}
