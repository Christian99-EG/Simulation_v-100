#include "latticewidget.h"
#include <QDebug>
#include <unistd.h>
#include <QPainter>
#include <cstdlib>
#include <ctime>

LatticeWidget::LatticeWidget(QWidget *parent)
    : QOpenGLWidget {parent}
{


    x = 50;
    y = 50;

    matriz = nullptr; //new int*[x];
    ciclos = nullptr; //new int*[x];

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &LatticeWidget::Evolucionar);
//    timer -> start(500);

}//fin del constructor

LatticeWidget::~LatticeWidget()
{
    if(matriz) //verificar si matriz es un valor nulo
    {
        for(int i = 0; i < x; i++)
        {
            delete[] matriz[i];
        }
        delete[] matriz;
    }
    if(ciclos) //verificar si ciclos es un valor nulo
    {
        for(int i = 0; i < x; i++)
        {
            delete[] ciclos[i];
        }
        delete[] ciclos;
    }
}//fin del destructor

void LatticeWidget::setTiempoInfeccion(int t) //para la entrada del tiempo de infeccion
{
    tiempoInfeccion = t;
}

void LatticeWidget::setHostInfectados(int h) //para la entrada de host infectados
{
    infectadosIniciales = h;
}

void LatticeWidget::CrearLattice() //implementacion del metodo CrearLattice
{
    if(MatrizGenerada)
    {
        qDebug() << "la matriz ya fue generada";
        return;
    }
    if(matriz) //verificar si matriz no es un valor nulo (nullptr)
    {

        for(int i = 0; i < x; ++i) delete[] matriz[i];
        delete[] matriz;
    }

    if(ciclos) //verificar si ciclos no es un valor nulo (nullptr)
    {
        for(int i = 0; i < x; ++i) delete[] ciclos[i];
        delete[] ciclos;
    }
    x = 50;
    y = 50;

    matriz = new int*[x];
    ciclos = new int*[x];

    for(int i =  0; i < x; i++)
    {
        matriz[i] = new int[y];
        ciclos[i] = new int[y];
    }

    std::srand(std::time(nullptr));

    for(int i = 0; i< x; i++)
    {
        for(int j = 0; j < y; j++)
        {
            matriz[i][j] = std::rand() % 2;
        }
    }

    setFixedSize(x*cellSize, y*cellSize);
    update(); //llamada al metodo paintGL
    MatrizGenerada = true;
}//fin de la implementacion CrearLattice

void LatticeWidget::ReiniciarLattice() //verificamos que la matriz (Lattice) ya fue creada
{
    MatrizGenerada = false;
    CrearLattice();
}

void LatticeWidget::setModoDibujo(ModoDibujo NuevoModo)
{
    modo = NuevoModo;
    update(); //actualizamos la lattice
}

void LatticeWidget::initializeGL()
{
    float r,g,b, a = normaliza_0_1(255.0f, RGB_MIN,RGB_MAX);
    initializeOpenGLFunctions();
    qColorToRGB(Qt::black,r,g,b);
    glClearColor(r,g,b,a);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}//end initializeGL

void LatticeWidget::paintGL() //construccion de la lattice
{
    if(!matriz) return; //si !matriz = nullptr

    QPainter painter(this);
    QColor ColorVivo = (modo == Lattice) ? QColor("#2f92de"): QColor("#6630c9");
    QColor ColorMuerto("#2E2E2E"); //Color gris de fondo (para las celulas que estan muertas)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(5.0f);

    for(int i = 0; i< x; ++i)
    {
        for(int j = 0; j < y; ++j)
        {
            painter.setBrush(matriz[i][j] == 1 ? ColorVivo : ColorMuerto);
            painter.drawRect(j * cellSize, i * cellSize, cellSize, cellSize);
        }
    }
}//end paintGL

int LatticeWidget::ContarVecinos(int i, int j)
{
    int conteo = 0;
    for(int dx = -1; dx <= 1; dx++)
    {
        for(int dy = -1; dy <= 1; dy++)
        {
            if(dx == 0 && dy == 0) continue;
            int ni = i + dx;
            int nj = j + dy;

            if(ni >= 0 && ni < x && nj >= 0 && nj < y)
            {
                conteo += matriz[ni][nj];
            }
        }
    }
    return conteo;
}//end contar vecinos

void LatticeWidget::Evolucionar()
{
    for(int i = 0; i < x; i++)
    {
        for(int j = 0; j < y; j++)
        {
            int vec = ContarVecinos(i,j);
            if(vec < 2 || vec > 3)
            {
                ciclos[i][j] = 0;
            }else if(vec == 2)
            {
                ciclos[i][j] = matriz[i][j]; //permanece igual
            }else if(vec == 3)
            {
                ciclos[i][j] = 1; //nace
            }
        }
    }

    //copiar los ciclos a la matriz
    for(int i = 0; i < x; i++)
    {
        for(int j = 0; j < y; j++)
        {
            matriz[i][j] = ciclos[i][j];
        }
    }

    CicloActual++;
    update(); //redibuja la matriz
}//fin fel metodo Evolucionar

void LatticeWidget::resizeGL(int w, int h)
{
    glViewport(0,0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}//fin del metodo resizeGL

void LatticeWidget::qColorToRGB(const QColor &C, float &r, float &g, float &b) const
{
    int red = C.red();
    int green = C.green();
    int blue = C.blue();
    r = normaliza_0_1(red, 1.0f, 255.0f);
    g = normaliza_0_1(green, 1.0f, 255.0f);
    b = normaliza_0_1(blue, 1.0f, 255.0f);
}//fin del metodo qColorToRGB


float LatticeWidget::normaliza_0_1(float val, float min, float max) const
{
    return (val - min) / (max-min);
}//fin del metodo normaliza_0_1
