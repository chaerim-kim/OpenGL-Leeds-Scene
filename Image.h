#ifndef _IMAGE_
#define _IMAGE_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <string>
#include <QImage>

class Image {
 public:

  Image(const std::string& fn);

  ~Image();

  const GLubyte* imageField() const;

  unsigned int Width()  const { return _width;}
  unsigned int Height() const {return _height;}
 private:
  Image(const Image&);

  unsigned int _width;
  unsigned int _height;

  QImage* p_qimage;


  GLubyte* _image;
  GLubyte* _image2;
  GLubyte* _image3;
};

#endif
