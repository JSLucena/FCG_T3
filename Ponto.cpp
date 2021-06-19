//
//  Ponto.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 18/08/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#include "Ponto.h"
#include "math.h"
Ponto::Ponto ()
{
    x=y=z=0;
}
Ponto::Ponto(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
void Ponto::set(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
void Ponto::imprime() {
    cout << "(" << x << ", " << y << ", " << z <<")" << flush;
}
void Ponto::imprime(char const *msg)
{
    cout << msg;
    imprime();
}
void Ponto::imprime(char const *msgAntes, char const *msgDepois)
{
    imprime(msgAntes);
    cout << msgDepois;
}
void Ponto::multiplica(double x, double y, double z)
{
    this->x *= x;
    this->y *= y;
    this->z *= z;
}

void Ponto::soma(double x, double y, double z)
{
    this->x += x;
    this->y += y;
    this->z += z;
}

void Ponto::rotacionaZ(float angulo)
{
    float xr, yr;
    //cout << "Angulo: " << angulo << " ";
    double anguloRad = angulo * 3.14159265359/180.0;
    xr = x*cos(anguloRad) - y*sin(anguloRad);
    yr = x*sin(anguloRad) + y*cos(anguloRad);
    x = xr;
    y = yr;
    //imprime();
}


void Ponto::rotacionaY(float angulo)
{
    float xr, zr;
    double anguloRad = angulo* 3.14159265359/180.0;
    xr =  x*cos(anguloRad) + z*sin(anguloRad);
    zr = -x*sin(anguloRad) + z*cos(anguloRad);
    x = xr;
    z = zr;
}

void Ponto::rotacionaX(float angulo)
{
    float yr, zr;
    double anguloRad = angulo* 3.14159265359/180.0;
    yr =  y*cos(anguloRad) - z*sin(anguloRad);
    zr =  y*sin(anguloRad) + z*cos(anguloRad);
    y = yr;
    z = zr;
}
double Ponto::modulo()
{
    return sqrt(x*x+y*y+z*z);
}

void Ponto::versor()
{
    float m = modulo();
    x /= m;
    y /= m;
    z /= m;
}


Ponto ObtemMaximo (Ponto P1, Ponto P2)
{
    Ponto Max;

    Max.x = (P2.x > P1.x) ? P2.x : P1.x;
    Max.y = (P2.y > P1.y) ? P2.y : P1.y;
    Max.z = (P2.z > P1.x) ? P2.z : P1.z;
    return Max;
}
Ponto ObtemMinimo (Ponto P1, Ponto P2)
{
    Ponto Min;

    Min.x = (P2.x < P1.x) ? P2.x : P1.x;
    Min.y = (P2.y < P1.y) ? P2.y : P1.y;
    Min.z = (P2.z < P1.x) ? P2.z : P1.z;
    return Min;
}
bool operator==(Ponto P1, Ponto P2)
{
    if (P1.x != P2.x) return false;
    if (P1.y != P2.y) return false;
    if (P1.z != P2.z) return false;
    return true;

}
Ponto operator+(Ponto P1, Ponto P2)
{
    Ponto temp;
    temp = P1;
    temp.x += P2.x;
    temp.y += P2.y;
    temp.z += P2.z;
    return temp;
}

Ponto operator- (Ponto P1, Ponto P2)
{
    Ponto temp;
    temp = P1;
    temp.x -= P2.x;
    temp.y -= P2.y;
    temp.z -= P2.z;
    return temp;
}
Ponto operator* (Ponto P1, float k)
{
    Ponto temp;
    temp.x = P1.x * k;
    temp.y = P1.y * k;
    temp.z = P1.z * k;
    return temp;
}

Ponto operator-(Ponto P1)
{
    return P1 * -1;
}
