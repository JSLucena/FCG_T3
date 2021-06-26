#ifndef player_hpp
#define player_hpp

#include <iostream>
using namespace std;


#ifdef WIN32
#include <windows.h>
#include <glut.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif


#include <vector>

#define TURNSTRENGTH 5


class Player
{

    float speed;
    int direction; ///direita ou esquerda





public:
    float posX,posZ;
    float angle = 0.0;
    Ponto dirPoint;
    Ponto Posicao;
    Ponto target;


    Player(float startPosX, float startPosZ);

    void setspeed(float f){
        speed = f;
    };
    float getspeed(){ return speed; };

    void setDirection(int i){
        direction = i;
    };
    int getDirection(){ return direction; };


    void updateHitbox();
    void movePlayer(float speed);
    void rotateEntity();
    void updateTarget(Ponto dir);


};
Player::Player(float startPosX, float startPosZ)
{
    speed = 0;
    direction = 0;
    Posicao = Ponto(startPosX,0,startPosZ);

}

void Player::updateTarget(Ponto dir)
{
    cout << "dir: ";
    dir.imprime();
    cout << endl;
    dir.rotacionaY(direction*TURNSTRENGTH);

    dirPoint = dir;
    target = Posicao + dir;
}
void Player::updateHitbox() ///atualizamos a hitbox, recriando ela do zero
{
      //  glScalef(0.2, 0.2, 0.2);
        defineCor(Silver);
        glTranslatef(Posicao.x,0,Posicao.z);
        glBegin ( GL_QUADS );
            // Front Face
            glNormal3f(0,0,1);
            glVertex3f(-0.2f, -0.2f,  0.2f);
            glVertex3f( 0.2f, -0.2f,  0.2f);
            glVertex3f( 0.2f,  0.2f,  0.2f);
            glVertex3f(-0.2f,  0.2f,  0.2f);
            // Back Face
            glNormal3f(0,0,-1);
            glVertex3f(-0.2f, -0.2f, -0.2f);
            glVertex3f(-0.2f,  0.2f, -0.2f);
            glVertex3f( 0.2f,  0.2f, -0.2f);
            glVertex3f( 0.2f, -0.2f, -0.2f);
            // Top Face
            glNormal3f(0,1,0);
            glVertex3f(-0.2f,  0.2f, -0.2f);
            glVertex3f(-0.2f,  0.2f,  0.2f);
            glVertex3f( 0.2f,  0.2f,  0.2f);
            glVertex3f( 0.2f,  0.2f, -0.2f);
            // Bottom Face
            glNormal3f(0,-1,0);
            glVertex3f(-0.2f, -0.2f, -0.2f);
            glVertex3f( 0.2f, -0.2f, -0.2f);
            glVertex3f( 0.2f, -0.2f,  0.2f);
            glVertex3f(-0.2f, -0.2f,  0.2f);
            // Right face
            glNormal3f(1,0,0);
            glVertex3f( 0.2f, -0.2f, -0.2f);
            glVertex3f( 0.2f,  0.2f, -0.2f);
            glVertex3f( 0.2f,  0.2f,  0.2f);
            glVertex3f( 0.2f, -0.2f,  0.2f);
            // Left Face
            glNormal3f(-1,0,0);
            glVertex3f(-0.2f, -0.2f, -0.2f);
            glVertex3f(-0.2f, -0.2f,  0.2f);
            glVertex3f(-0.2f,  0.2f,  0.2f);
            glVertex3f(-0.2f,  0.2f, -0.2f);
            glEnd();
   // Hitbox.imprime();
   // cout << endl;
}

void Player::movePlayer(float speed)
{

   // posX = posX + speed * direction;
  // Posicao.rotacionaY(direction);
   Posicao = Posicao + dirPoint * speed;
   Posicao.imprime();
   cout << endl;
 //  posZ = posZ + speed * sin(angle*3.14/180);
 //  posZ = posZ + speed * sin(angle);
  //  glTranslatef(posX,0,0);


    direction = 0; ///resetamos a direcao para voltar o controle para o teclado
}
void Player::rotateEntity()
{
    if(direction == 1)
        angle = angle + 1.0 * TURNSTRENGTH;
    else if(direction == -1)
        angle = angle - 1.0 * TURNSTRENGTH;

      //  Posicao.rotacionaY(direction);
        glTranslatef(Posicao.x,0,Posicao.z);
        glRotatef(angle, 0,1,0);
        glTranslatef(-Posicao.x,0,-Posicao.z);




//    Ponto P = Ponto(posX - Sprite.width/2,posZ - Sprite.height/2);

   // glTranslatef(P.x, P.y, P.z);
    //glRotatef(shotAngle, 0,0,1);
  //  glTranslatef(-P.x, -P.y, -P.z);
}




#endif
