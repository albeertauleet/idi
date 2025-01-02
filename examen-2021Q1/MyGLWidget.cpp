// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Pintem el Patricio
  if (pinta == 0)
  {
    glBindVertexArray (VAO_Patr);
    modelTransformPatricio();
    glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  }
  // Pintem el cub
  else
  {
    glBindVertexArray(VAO_Cub);
    modelTransformCub (4.0f, angles[rotationIndex]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    modelTransformCub(5.0f, angles[(rotationIndex+1)%3]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    modelTransformCub(6.0f, angles[(rotationIndex+2)%3]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  
  glBindVertexArray(0);
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG, angle, glm::vec3(0, 1, 0));
  TG = glm::translate(TG, glm::vec3(5.0f, 0.0f, 0.0f));
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  TG = glm::mat4(1.0f);
  if (num_cub == 1)
  {
    TG = glm::rotate(TG, angles[(rotationIndex+1)%3], glm::vec3(0, 1, 0));
  }

  else if (num_cub == 2)
  {
    TG = glm::rotate(TG, angles[(rotationIndex+2)%3], glm::vec3(0, 1, 0));
  }
  else
  {
    TG = glm::rotate(TG, angles[rotationIndex], glm::vec3(0, 1, 0));
  }
  

  TG = glm::translate(TG, glm::vec3(5.0f, 0.0f, 0.0f));
  TG = glm::rotate(TG, glm::radians(90.0f), glm::vec3(0, -1, 0));
  TG = glm::scale(TG, glm::vec3 (2*escala, 2*escala, 2*escala));
  TG = glm::translate(TG, -centreBasePat);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, glm::radians(90.0f), glm::vec3(1, 0, 0));
    View = glm::rotate(View, glm::radians(90.0f), glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    glm::mat4 Proj;  // Matriu de projecció
   
   float l = -radiEsc;
   float r = radiEsc;
   float t = radiEsc;
   float b = -radiEsc;

   Proj = glm::ortho(l, r, b, t, zn, zf);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
      if (pinta == 0) pinta = 1;
      else pinta = 0;
    break;
	}
  case Qt::Key_1: {
      num_cub = 0;
    break;
	}
  case Qt::Key_2: {
      num_cub = 1;
    break;
	}
  case Qt::Key_3: {
      num_cub = 2;
    break;
	}
  case Qt::Key_F: {
      if (llum_blanca == 0)
      {
        colFoc = glm::vec3(1.0, 1.0, 0.0);
        llum_blanca = 1;
      }
      else
      {
        colFoc = glm::vec3(1.0, 1.0, 1.0);
        llum_blanca = 0;
      }
      enviaColFocus();

    break;
	}
  case Qt::Key_C: {
    if (camPlanta)
      camPlanta = false;
    else 
      camPlanta = true;
    projectTransform();
    viewTransform();
    break;
	}
  case Qt::Key_Right: {
    rotationIndex++;
    if (rotationIndex == 3) rotationIndex = 0;
    break;
	}
  case Qt::Key_Left: {
    rotationIndex--;
    if (rotationIndex == -1) rotationIndex = 2;
    break;
	}
  case Qt::Key_R: {
      rotationIndex = 0;
      camPlanta = false;
      num_cub = 0;
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}


//

