#ifndef GAMECAMERA_H
#define GAMECAMERA_H


class gamecamera
{
private:
    int     m_etat = 0;
    float   m_rotX = -45.0;
    float   m_rotY = -45.0;
    float   m_ss   = 1.0f;

public:
    int     m_temps = 0;

    gamecamera();

    scale();

    int getEtat();
    float getRotX();
    float getRotY();
    float getScale();

    void setEtat(int etat);
    void setRotX(float rotX);
    void setRotY(float rotY);
    void setScale(float scale);
};

#endif // GAMECAMERA_H
