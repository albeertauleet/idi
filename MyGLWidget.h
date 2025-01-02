#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio ();
    virtual void projectTransform ();
    virtual void viewTransform ();

    int num_cub = 0;
    int pinta = 0;
    int llum_blanca = 0;
    int rotationIndex = 0;

    float angles[3] = {0, 2*M_PI/3, 4*M_PI/3};


    //
    //virtual void paintGL();
    //virtual void modelTransformCube(float escala, float angle);

  private:
    int printOglError(const char file[], int line, const char func[]);
};
