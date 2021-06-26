// **********************************************************************
// PUCRS/Escola Politécnica
// COMPUTAÇÃO GRÁFICA
//
// Simulador de Cidade
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>

using namespace std;


#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif

#include "Temporizador.h"
Temporizador T;
double AccumDeltaT=0;

#include "Ponto.h"
#include "ListaDeCoresRGB.h"
#include "player.h"

GLfloat AspectRatio, angulo=0;

// Controle do modo de projecao
// 0: Projecao Paralela Ortografica; 1: Projecao Perspectiva
// A funcao "PosicUser" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'p'

int ModoDeProjecao = 1;
int playerView = 1;

// Controle do modo de projecao
// 0: Wireframe; 1: Faces preenchidas
// A funcao "Init" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'w'
int ModoDeExibicao = 1;

double nFrames=0;
double TempoTotal=0;

Ponto Curva1[3];

// Qtd de ladrilhos do piso. Inicialzada na INIT
int QtdX;
int QtdZ;
float speed;

float truckLookingAt[3];

// Representa o conteudo de uma celula do piso
class Elemento{
public:
    int tipo;
    int cor;
    int corPredio;
    float altura;
};

// codigos que definem o o tipo do elemento que está em uma célula
#define VAZIO 0
#define PREDIO 2
#define RUA 1
#define COMBUSTIVEL 3

// Matriz que armazena informacoes sobre o que existe na cidade
Elemento Cidade[100][100];


Player  truck = Player(0,0);



void playerHandler()
{
    glPushMatrix();

        truckLookingAt[0] = truck.posX + cos(truck.angle*3.14/180);
        truckLookingAt[1] = 0.2;
        truckLookingAt[2] = truck.posZ + sin(truck.angle*3.14/180);

        truck.rotateEntity();
        truck.movePlayer(speed);
        truck.updateHitbox();
    glPopMatrix();
}
// **********************************************************************
//
// **********************************************************************
void InicializaCidade(int QtdX, int QtdZ)
{
    for (int i=0;i<QtdZ;i++)
        for (int j=0;j<QtdX;j++)
            Cidade[i][j].tipo = VAZIO;


    string nome = "mapa.txt";
    ifstream input;
    input.open(nome, ios::in);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...";
    string S;
    unsigned int linhas, colunas;

    input >> linhas >> colunas;
    for (int i=0; i< linhas; i++)
    {
        for(int j = 0; j < colunas; j++)
        {
            double x,y;
        // Le cada elemento da linha
        input >> Cidade[i][j].tipo;
        if(Cidade[i][j].tipo == RUA)
            Cidade[i][j].cor = Bronze;
        else if (Cidade[i][j].tipo == VAZIO)
            Cidade[i][j].cor = Red;
        else
        {
            Cidade[i][j].cor = Red;
            Cidade[i][j].corPredio = rand() % LAST_COLOR;
            Cidade[i][j].altura = 0.2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2));

        }


        if(!input)
            break;
        }

    }
    input.close();


    cout << "Mapa lido com sucesso!" << endl;


}


// **********************************************************************
//  void init(void)
//    Inicializa os parametros globais de OpenGL
// **********************************************************************
void init(void)
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto

    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);
    glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    if (ModoDeExibicao) // Faces Preenchidas??
    {
        glEnable(GL_DEPTH_TEST);
        glEnable (GL_CULL_FACE );
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
    {
        glEnable(GL_DEPTH_TEST);
        glEnable (GL_CULL_FACE );
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glEnable(GL_NORMALIZE);

    Curva1[0] = Ponto (-6,0,1);
    Curva1[1] = Ponto (0,10,1);
    Curva1[2] = Ponto (6,0,1);

    srand((unsigned int)time(NULL));

    QtdX = 12;
    QtdZ = 12;

    InicializaCidade(QtdX, QtdZ);


    truckLookingAt[0] = truck.posX + 1;
    truckLookingAt[1] = 0.1;
    truckLookingAt[2] = truck.posZ;



}

// **********************************************************************
//
// **********************************************************************
void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualização da tela em 30
    {
        AccumDeltaT = 0;
        angulo+= 1;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        cout << "Nros de Frames sem desenho: " << nFrames << endl;
        cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
}

// **********************************************************************
Ponto CalculaBezier3(Ponto PC[], double t)
{
    Ponto P;
    double UmMenosT = 1-t;

    P =  PC[0] * UmMenosT * UmMenosT + PC[1] * 2 * UmMenosT * t + PC[2] * t*t;
    //P.z = 5;
    return P;
}
// **********************************************************************
void TracaBezier3Pontos()
{
    double t=0.0;
    double DeltaT = 1.0/10;
    Ponto P;
    //cout << "DeltaT: " << DeltaT << endl;
    glBegin(GL_LINE_STRIP);

    while(t<1.0)
    {
        P = CalculaBezier3(Curva1, t);
        glVertex3f(P.x, P.y, P.z);
        t += DeltaT;
       // P.imprime(); cout << endl;
    }
    P = CalculaBezier3(Curva1, 1.0); // faz o fechamento da curva
    glVertex3f(P.x, P.y, P.z);
    glEnd();
}
// **********************************************************************
//  void DesenhaCubo()
//
//
// **********************************************************************
void DesenhaCubo()
{
    glBegin ( GL_QUADS );
    // Front Face
    glNormal3f(0,0,1);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    // Back Face
    glNormal3f(0,0,-1);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    // Top Face
    glNormal3f(0,1,0);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    // Bottom Face
    glNormal3f(0,-1,0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    // Right face
    glNormal3f(1,0,0);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    // Left Face
    glNormal3f(-1,0,0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
}

// **********************************************************************
//  Desenha um prédio no meio de uam célula
// **********************************************************************
void DesenhaPredio(float altura)
{
    glPushMatrix();
       // glTranslatef(0, -1, 0);
        glScalef(0.2, altura, 0.2);
        glTranslatef(0, 1, 0);
        DesenhaCubo();
    glPopMatrix();

}
// **********************************************************************
// void DesenhaLadrilho(int corBorda, int corDentro)
// Desenha uma célula do piso.
// Eh possivel definir a cor da borda e do interior do piso
// O ladrilho tem largula 1, centro no (0,0,0) e está sobre o plano XZ
// **********************************************************************
void DesenhaLadrilho(int corBorda, int corDentro)
{
    defineCor(corBorda); // desenha QUAD preenchido
    glBegin ( GL_QUADS );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();

    defineCor(corDentro);
    glBegin ( GL_LINE_STRIP ); // desenha borda do ladrilho
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();

}

// **********************************************************************
// DesenhaCidade(int nLinhas, int nColunas)
// QtdX: nro de células em X
// QtdZ: nro de células em Z
// Desenha elementos que compiem a cidade
// **********************************************************************
void DesenhaCidade(int QtdX, int QtdZ)
{
/*
    glPushMatrix();
        glTranslatef(0, 0, 0);
        DesenhaLadrilho(Red, Black);
        defineCor(Yellow);
        DesenhaPredio(2);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-1, 0, 0);
        DesenhaLadrilho(Gray, Black);
        defineCor(Green);
        DesenhaPredio(1.2);
    glPopMatrix();
    */

    for (int i=0;i<QtdZ;i++)
        for (int j=0;j<QtdX;j++)
            {
                glPushMatrix();
                    glTranslatef(i,0,j);
                    DesenhaLadrilho(Cidade[i][j].cor,Black);
                    if(Cidade[i][j].tipo == PREDIO)
                    {
                        defineCor(Cidade[i][j].corPredio);
                        DesenhaPredio(Cidade[i][j].altura);
                    }

                glPopMatrix();
            }
}


// **********************************************************************
//  void DefineLuz(void)
// **********************************************************************
void DefineLuz(void)
{
  // Define cores para um objeto dourado
  GLfloat LuzAmbiente[]   = {0.4, 0.4, 0.4f } ;
  GLfloat LuzDifusa[]   = {0.7, 0.7, 0.7};
  GLfloat LuzEspecular[] = {0.9f, 0.9f, 0.9 };
  GLfloat PosicaoLuz0[]  = {0.0f, 3.0f, 5.0f };  // Posição da Luz
  GLfloat Especularidade[] = {1.0f, 1.0f, 1.0f};

   // ****************  Fonte de Luz 0

 glEnable ( GL_COLOR_MATERIAL );

   // Habilita o uso de iluminação
  glEnable(GL_LIGHTING);

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
  // Define os parametros da luz número Zero
  glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifusa  );
  glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspecular  );
  glLightfv(GL_LIGHT0, GL_POSITION, PosicaoLuz0 );
  glEnable(GL_LIGHT0);

  // Ativa o "Color Tracking"
  glEnable(GL_COLOR_MATERIAL);

  // Define a reflectancia do material
  glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade);

  // Define a concentraçãoo do brilho.
  // Quanto maior o valor do Segundo parametro, mais
  // concentrado será o brilho. (Valores válidos: de 0 a 128)
  glMateriali(GL_FRONT,GL_SHININESS,51);

}

// **********************************************************************
//  void PosicUser()
//
//
// **********************************************************************
void PosicUser()
{

    // Define os parâmetros da projeção Perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define o volume de visualização sempre a partir da posicao do
    // observador
    if (ModoDeProjecao == 0)
        glOrtho(-10, 10, -10, 10, 0, 20); // Projecao paralela Orthografica
    else gluPerspective(90,AspectRatio,0.01,1500); // Projecao perspectiva


    if(playerView)
    {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(truck.posX , 0.35, truck.posZ,   // Posição do Observador
              truckLookingAt[0],truckLookingAt[1],truckLookingAt[2],     // Posição do Alvo
              0.0f,1.0f,0.0f); // UP
    }
    else
    {
         glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-1, 6, -1,   // Posição do Observador
              truck.posX,0,truck.posZ,     // Posição do Alvo
              0.0f,1.0f,0.0f); // UP
    }




}
// **********************************************************************
//  void reshape( int w, int h )
//		trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{

	// Evita divisão por zero, no caso de uam janela com largura 0.
	if(h == 0) h = 1;
    // Ajusta a relação entre largura e altura para evitar distorção na imagem.
    // Veja função "PosicUser".
	AspectRatio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Seta a viewport para ocupar toda a janela
    glViewport(0, 0, w, h);
    //cout << "Largura" << w << endl;

	PosicUser();

}

// **********************************************************************
//  void display( void )
//
//
// **********************************************************************
void display( void )
{

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	DefineLuz();

	PosicUser();
    glLineWidth(2);

	glMatrixMode(GL_MODELVIEW);

    glColor3f(1,1,1);
    TracaBezier3Pontos();

    playerHandler();

    DesenhaCidade(QtdX,QtdZ);

	glutSwapBuffers();
}


// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{
	switch ( key )
	{
    case 27:        // Termina o programa qdo
      exit ( 0 );   // a tecla ESC for pressionada
      break;
    case 'p':
            ModoDeProjecao = !ModoDeProjecao;
            glutPostRedisplay();
            break;
    case 'q':
            playerView = !playerView;
            glutPostRedisplay();
            break;
    case 'e':
            ModoDeExibicao = !ModoDeExibicao;
            init();
            glutPostRedisplay();
            break;
    case ' ':
            if(speed == 0)
                speed = 0.01;
            else
                speed = 0;
            cout << speed;
            break;
    case 'a':
            truck.setDirection(1);
            break;
    case 'd':
            truck.setDirection(-1);
            break;
    default:
            cout << key;
    break;
  }
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
		case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
			glutFullScreen ( ); // Go Into Full Screen Mode
			break;
	    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
			glutInitWindowSize  ( 700, 500 );
			break;
		default:
			break;
	}
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int main ( int argc, char** argv )
{
	glutInit            ( &argc, argv );
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
	glutInitWindowPosition (0,0);
	glutInitWindowSize  ( 700, 700 );
	glutCreateWindow    ( "Computacao Grafica - Exemplo Basico 3D" );

	init ();
    //system("pwd");

	glutDisplayFunc ( display );
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( arrow_keys );
	glutIdleFunc ( animate );

	glutMainLoop ( );
	return 0;
}



