#ifndef LATTICEWIDGET_H
#define LATTICEWIDGET_H

#include <QColor>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

#define RGB_MIN 1
#define RGB_MAX 255

enum ModoDibujo{Lattice, Automata};

class LatticeWidget: public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    LatticeWidget(QWidget *parent = nullptr);
    ~LatticeWidget();

    virtual void CrearLattice(); //metodo que construye la lattice
    virtual void initializeGL();
    virtual void paintGL(); //este metodo dibuja la lattice
    //virtual void paintGL2(); //este metodo activa al automata Nota: solo se utilizara paintGL()
    virtual void resizeGL(int w, int h);

public slots:
    virtual void Evolucionar(); //metodo que comenzara con la simulacion del automata
private:
   virtual void qColorToRGB(const QColor &C, float &r, float &g, float &b) const;
   virtual float normaliza_0_1(float val, float min, float max) const;
   bool MatrizGenerada = false;

   public:
    //para la lattice
    int **matriz;
    int **ciclos;
    int x, y;
    const int cellSize = 10;
    int CicloActual = 0;
    const int maxCiclos = 100;
    enum ModoDibujo{Lattice, Automata};
    ModoDibujo modo = Lattice; //definimos que LatticeNormal es solo la representacion de la red
    QTimer *timer;

    //para la propagacion
    int tiempoInfeccion, infectadosIniciales;

    void setTiempoInfeccion(int t); //donde el parametro t = tiempo (entrada del tiempo)
    void setHostInfectados(int h); //donde el parametro h = host (numero de host infectados)

    virtual int ContarVecinos(int i, int j);
    virtual void setModoDibujo(ModoDibujo NuevoModo); //cambia el color y actualiza la lattice sobre wl widget
    int** getMatriz() const {return matriz;}
    //bool SimulationRunning = false; //para comenzar la simulacion del automata se ocupa un booleano
    //void setSimulationRunning(bool running);
    void ReiniciarLattice();
};
#endif // LATTICEWIDGET_H

