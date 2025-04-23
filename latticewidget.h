#ifndef LATTICEWIDGET_H
#define LATTICEWIDGET_H

#include <QColor>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

#define RGB_MIN 1
#define RGB_MAX 255

class LatticeWidget: public QOpenGLWidget, public QOpenGLFunctions
{
public:
    LatticeWidget(QWidget *parent = nullptr);
    ~LatticeWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private slots:
    void Evolucionar();

private:
    void qColorToRGB(const QColor &C, float &r, float &g, float &b) const;
    float normaliza_0_1(float val, float min, float max) const;

    //para la lattice
    int **matriz;
    int **ciclos;
    int x, y;
    const int cellSize = 10;
    int CicloActual = 0;
    const int maxCiclos = 100;
    QTimer *timer;

    int ContarVecinos(int i, int j);
};

#endif // LATTICEWIDGET_H
