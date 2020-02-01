#ifndef __GL_POLYGON_WIDGET_H__
#define __GL_POLYGON_WIDGET_H__ 1

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <QGLWidget>
#include "Image.h"


class LeedsWidget: public QGLWidget
	{ //

	Q_OBJECT

	public:

	LeedsWidget(QWidget *parent);
	~LeedsWidget();

	public slots:
	void updateGlobalAngle();


	public slots:
        void updateLegAngle(int i);
				void updateAngle(int j);


	protected:
	// called when OpenGL context is set up
	void initializeGL();
	// called every time the widget is resized
	void resizeGL(int w, int h);
	// called every time the widget needs painting
	void paintGL();

	private:

	// Image _image;
	Image _image;
	Image _image2;
	Image _image3;

	// update horse movement up and down
	void updateHorse1();
	void updateHorse2();
	void updateHorse3();
	void updateHorse4();

	// draw functions
	void drawMerrygoround();
	void drawHorse();
	void drawPlane();
	void drawCouncil();
	void drawTree();
	void drawBacon();

  //merrygoround
	void column();
	void horsepole();
	void stage();

  // horse body
	void head();
	void neck();
	void face();
	void shoulders();
	void upper_leg();
	void lower_leg();

	// tree
	void treetrunk();
	void star();

	void bacon();

	// city councl
	void roof();
	void pillar();
	void shortpillar();
	void miniSquare();


	GLUquadricObj* pcolumn;
	GLUquadricObj* phorsepole;
	GLUquadricObj* pstage;
	GLUquadricObj* phead;
	GLUquadricObj* pneck;
	GLUquadricObj* pface;
	GLUquadricObj* pshoulders;
	GLUquadricObj* pupperleg;
	GLUquadricObj* plowerleg;
	GLUquadricObj* ptreetrunk;
	GLUquadricObj* pbacon;
	GLUquadricObj* proof;
	GLUquadricObj* ppillar;
	GLUquadricObj* pshortpillar;

	int globalangle;
	int angle;
	int legangle;

	}; // class GLPolygonWidget

#endif
