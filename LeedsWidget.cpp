#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <QGLWidget>
#include <QSlider>
#include <cmath>
#include "LeedsWidget.h"

#define PI 3.1415927
GLfloat RADIUS = 2.0;
double COLUMN_HEIGHT = 3; // cylinder

// for the horse
double HEAD_HEIGHT = 1.5;
double LEG_LENGTH = 1.0;
double NECK_LENGTH = 2.0;
double NECK_ANGLE = 55;
double SHOULDER_WIDTH = 3.0;
double SHOULDER_RADIUS = 1.0;
double butt_HEIGHT = 1.5;

static int flag1=0; // to see if we've hit the limit
static int flag2=0;
static int flag3=0;
static int flag4=0;

float horse_y1=0.5;
float horse_y2=1.5;
float horse_y3=1.0;
float horse_y4=2;

// Setting up material properties
typedef struct materialStruct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
} materialStruct;

static materialStruct brassMaterials = {
        {0.43, 0.22, 0.03, 1.0},
        {0.78, 0.57, 0.11, 1.0},
        {0.99, 0.91, 0.81, 1.0},
        27.8
};

static materialStruct bronzeMaterials = {
        {0.25f, 0.148f, 0.06475f, 1.0f},
        {0.4f, 0.2368f, 0.1036f, 1.0f },
        {0.774597f, 0.458561f, 0.200621f, 1.0f},
        76.8f
};

static materialStruct whiteShinyMaterials = {
        {1.0, 1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0, 1.0},
        100.0
};

static materialStruct turquoiseMaterials = {
        {0.10, 0.19, 0.17, 1.0},
        {0.40, 0.74, 0.69, 1.0},
        {0.29, 0.31, 0.30, 1.0},
        0.1
};

static materialStruct rubyMaterials = {
        {0.17, 0.01, 0.01, 1.0},
        {0.80, 0.04, 0.04, 1.0},
        {0.73, 0.63, 0.63, 1.0},
        0.8
};

static materialStruct greenMaterials = {
        {0.00, 0.05, 0.00, 1.0},
        {0.40, 0.50, 0.40, 1.0},
        {0.04, 0.07, 0.04, 1.0},
        0.3
};

static materialStruct chromeMaterials = {
        {0.25, 0.25, 0.25, 1.0 },
        {0.40, 0.40, 0.40, 1.0 },
        {0.77, 0.77, 0.77, 1.0 },
        50.8
};

static materialStruct yellowMaterials = {
        {0.05, 0.05, 0.00, 1.0 },
        {0.50, 0.50, 0.40, 1.0 },
        {0.70, 0.70, 0.04, 1.0 },
        10
};


// constructor
LeedsWidget::LeedsWidget(QWidget *parent)
        : QGLWidget(parent),
          globalangle(0),
          angle(0),
          legangle(0)
          ,_image("./Moi.ppm")
          ,_image2("./earth.ppm")
          ,_image3("./ground.png")
{ // constructor
    pcolumn = gluNewQuadric();
    phorsepole = gluNewQuadric();
    pstage = gluNewQuadric();

    phead = gluNewQuadric();
    pneck = gluNewQuadric();
    pface = gluNewQuadric();
    pshoulders = gluNewQuadric();
    pupperleg = gluNewQuadric();
    plowerleg = gluNewQuadric();

    ptreetrunk = gluNewQuadric();
    pbacon = gluNewQuadric();

    proof = gluNewQuadric();
    ppillar = gluNewQuadric();
    pshortpillar = gluNewQuadric();
} // constructor

// called when OpenGL context is set up
void LeedsWidget::initializeGL()
{
    // set the widget background colour
    glClearColor(	0.717, 0.776, 0.529, 0.0);
    // set drawing color to white
    glColor3f(1.0, 1.0, 1.0);
}

LeedsWidget::~LeedsWidget()
{
    gluDeleteQuadric(phead);
    gluDeleteQuadric(pneck);
    gluDeleteQuadric(pface);
    gluDeleteQuadric(pshoulders);
    gluDeleteQuadric(pcolumn);
    gluDeleteQuadric(phorsepole);
    gluDeleteQuadric(pstage);
    gluDeleteQuadric(pupperleg);
    gluDeleteQuadric(plowerleg);
    gluDeleteQuadric(ptreetrunk);
    gluDeleteQuadric(pbacon);
    gluDeleteQuadric(proof);
    gluDeleteQuadric(ppillar);
    gluDeleteQuadric(pshortpillar);
}


// called every time the widget is resized
void LeedsWidget::resizeGL(int w, int h) { // resizeGL()
    // set the viewport to the entire widget
    glViewport(0, 0, w, h);

    // for lighting
    GLfloat light_pos[] = {0.5, 0.25, 0.5, 0.};
    glEnable(GL_LIGHTING); // enable lighting in general
    glEnable(GL_LIGHT0);   // each light source must also be enabled
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 150.);

    // for texturing
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 100.0);
} // resizeGL()


void LeedsWidget::updateGlobalAngle()
{
    globalangle += 1.;
    this->repaint();
}

void LeedsWidget::updateAngle(int j)
{
    angle = j;
    this->repaint();
}

void LeedsWidget::updateLegAngle(int i)
{
  legangle = i;
  this->repaint();
}

// update horse moving up and down along the pole
void LeedsWidget::updateHorse1() {
  if (!flag1){
    horse_y1 += 0.01; //can randomize
    if (horse_y1 >= 2.0) flag1 = 1;
  }

  if (flag1){
    horse_y1 -= 0.01;
    if(horse_y1 <= 0.6) flag1 = 0;
  }
}

void LeedsWidget::updateHorse2() {
  if (!flag2){
    horse_y2 += 0.015; //can randomize
    if (horse_y2 >= 2.0) flag2 = 1;
  }

  if (flag2){
    horse_y2 -= 0.015;
    if(horse_y2 <= 0.6) flag2 = 0;
  }
}

void LeedsWidget::updateHorse3() {
  if (!flag3){
    horse_y3 += 0.01; //can randomize
    if (horse_y3 >= 2.0) flag3 = 1;
  }

  if (flag3){
    horse_y3 -= 0.01;
    if(horse_y3 <= 0.6) flag3 = 0;
  }
}

void LeedsWidget::updateHorse4() {
  if (!flag4){
    horse_y4 += 0.015; //can randomize
    if (horse_y4 >= 2.0) flag4 = 1;
  }

  if (flag4){
    horse_y4 -= 0.015;
    if(horse_y4 <= 0.6) flag4 = 0;
  }
}

// for merrygoround
void LeedsWidget::stage() {
    GLfloat height = 0.2;
    gluCylinder(pstage, RADIUS, RADIUS, height, 30, 2);

    GLfloat x = 0.0;
    GLfloat y = 0.0;
    GLfloat aangle = 0.0;
    GLfloat angle_stepsize = 0.1;

    // top of the cylinder
    glNormal3f(0.,0.,-1);
    glBegin(GL_POLYGON);
    while (aangle < 2 * PI) {
        x = RADIUS * cos(aangle);
        y = RADIUS * sin(aangle);
        glVertex3f(x, y, 0.0);
        aangle = aangle + angle_stepsize;
    }
    glVertex3f(RADIUS, 0.0, 0.0);
    glEnd();

    // bottom of the cyl
    glBegin(GL_POLYGON);
    while (aangle < 2 * PI) {
        x = RADIUS * cos(aangle);
        y = RADIUS * sin(aangle);
        glVertex3f(x, y, height);
        aangle = aangle + angle_stepsize;
    }
    glVertex3f(RADIUS, 0.0, height);
    glEnd();
}

void LeedsWidget::column() {
    // quad, base, top, height, slices, stacks
    gluCylinder(pcolumn, 0.1, 0.1, COLUMN_HEIGHT, 7, 5);
}

// for horse
void LeedsWidget::horsepole() {
    // quad, base, top, height, slices, stacks
    gluCylinder(phorsepole, 0.05, 0.05, COLUMN_HEIGHT, 7, 5);
}

void LeedsWidget::head() {
    gluSphere(phead, 1.0, 20, 20);
}

void LeedsWidget::neck() {
  // quad, base, top, height, slices, stacks
    gluCylinder(pneck, 0.5, 0.5, NECK_LENGTH, 10, 5);
}

void LeedsWidget::face() {
    gluCylinder(pface, 0.5, 0.2, 1.5, 10, 5);
}

void LeedsWidget::shoulders() {
    gluCylinder(pshoulders,SHOULDER_RADIUS, SHOULDER_RADIUS, 3, 10, 4);
}

void LeedsWidget::upper_leg() {
    gluCylinder(pupperleg, 0.3, 0.3, LEG_LENGTH, 4, 4);
}

void LeedsWidget::lower_leg() {
    gluCylinder(plowerleg, 0.3, 0.2, LEG_LENGTH, 4, 4);
}

// for tree
void LeedsWidget::treetrunk() {
    gluCylinder(ptreetrunk, 0.4, 0.4, 3, 10, 5);
}

void LeedsWidget::star() {
  glEnable(GL_COLOR_MATERIAL);
    glColor3f(255,255,0);
    glNormal3f(0,0,0.31);
    glBegin(GL_POLYGON);
      glVertex3f(0.0, -0.5, 0.0);
      glVertex3f(0.5, -0.8, 0.0);
      glVertex3f(0.3, -0.3, 0.0);
      glVertex3f(0.8, 0.1, 0.0);
      glVertex3f(0.2, 0.1, 0.0);
      glVertex3f(0.0, 0.8, 0.0);
      glVertex3f(-0.2, 0.1, 0.0);
      glVertex3f(-0.8, 0.1, 0.0);
      glVertex3f(-0.3, -0.3, 0.0);
      glVertex3f(-0.5, -0.8, 0.0);
    glEnd();
  glDisable(GL_COLOR_MATERIAL);
}

// for bacon
void LeedsWidget::bacon() {
    gluCylinder(pbacon, 0.3, 0.3, 2, 10, 5);
}

// for city council
void LeedsWidget::roof() {
    gluCylinder(proof, 0.4, 0.4, 2, 3, 5);
}

void LeedsWidget::pillar() {
    gluCylinder(ppillar, 0.1, 0.1, 2, 10, 5);
}

void LeedsWidget::shortpillar() {
    gluCylinder(pshortpillar, 0.1, 0.1, 1.5, 10, 5);
}

void LeedsWidget::drawPlane() {
  glEnable(GL_COLOR_MATERIAL);
    glColor3f(1., 1., 1.); // set whit bg
    // texture mapping to cobble
    glEnable(GL_TEXTURE_2D);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _image3.Width(), _image3.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, _image3.imageField());
      // Binding ground
      glNormal3f(0., 1., 0.);
      glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(  20.0,  0,  20.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(  20.0,  0, -20.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f( -20.0,  0, -20.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f( -20.0,  0,  20.0);
      glEnd();
    glDisable(GL_TEXTURE_2D);
  glDisable(GL_COLOR_MATERIAL);
}

void LeedsWidget::drawMerrygoround() {
    materialStruct *p_front = &turquoiseMaterials;
    glMaterialfv(GL_FRONT, GL_AMBIENT, p_front->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, p_front->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, p_front->specular);
    glMaterialf(GL_FRONT, GL_SHININESS, p_front->shininess);

    // placing the bototm stage
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    this->stage();
    glPopMatrix();

    // rotation by itself (merry goround)
    glRotatef((double) globalangle/2, 0., 1., 0.);

    // top cone
    glPushMatrix();
    glTranslatef(0.0, COLUMN_HEIGHT, 0.0);
    glRotatef(-90., 1., 0., 0.);
    glutSolidCone(RADIUS, 1.25, 20, 10);
    glPopMatrix(); // restore the world

    // COLUMNs
    p_front = &whiteShinyMaterials;
    glMaterialfv(GL_FRONT, GL_AMBIENT, p_front->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, p_front->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, p_front->specular);
    glMaterialf(GL_FRONT, GL_SHININESS, p_front->shininess);

    // creating lt
    glPushMatrix();
      glTranslatef(-RADIUS, COLUMN_HEIGHT, 0.0);
      glRotatef(90., 1., 0., 0.);
      glRotatef((double) legangle, 0., 0., 1.);
      this->column();
    glPopMatrix();

    // creating rb
    glPushMatrix();
      glTranslatef(RADIUS, COLUMN_HEIGHT, 0.0);
      glRotatef(90., 1., 0., 0.);
      glRotatef((double) legangle, 0., 0., 1.);
      this->column();
    glPopMatrix();

    // creating lb
    glPushMatrix();
      glTranslatef(0.0, COLUMN_HEIGHT, RADIUS);
      glRotatef(90., 1., 0., 0.);
      glRotatef((double) legangle, 0., 0., 1.);
      this->column();
    glPopMatrix();

    // creating rt
    glPushMatrix();
      glTranslatef(0.0, COLUMN_HEIGHT,-RADIUS);
      glRotatef(90., 1., 0., 0.);
      glRotatef((double) legangle, 0., 0., 1.);
      this->column();
    glPopMatrix();

    p_front = &rubyMaterials;
    glMaterialfv(GL_FRONT, GL_AMBIENT, p_front->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, p_front->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, p_front->specular);
    glMaterialf(GL_FRONT, GL_SHININESS, p_front->shininess);

    // placing the circle
    glPushMatrix();
      glTranslatef(0., 1.5 + COLUMN_HEIGHT ,0.);
      glutSolidSphere(0.5, 20, 20);
    glPopMatrix();

    // moving pole 1
    glPushMatrix();
      glRotatef((double) globalangle/2.5, 0., 1., 0.);
      glTranslatef(-1.2, COLUMN_HEIGHT, 0.0);
      glRotatef(90., 1., 0., 0.);
      this->horsepole();
    glPopMatrix();

    // moving pole 2
    glPushMatrix();
      glRotatef((double) globalangle/2.5, 0., 1., 0.);
      glTranslatef(1.2, COLUMN_HEIGHT, 0.0);
      glRotatef(90., 1., 0., 0.);
      this->horsepole();
    glPopMatrix();

    // moving pole 3
    glPushMatrix();
      glRotatef((double) globalangle/2.5, 0., 1., 0.);
      glTranslatef(0., COLUMN_HEIGHT, 1.2);
      glRotatef(90., 1., 0., 0.);
      this->horsepole();
    glPopMatrix();

    // moving pole 4
    glPushMatrix();
      glRotatef((double) globalangle/2.5, 0., 1., 0.);
      glTranslatef(0., COLUMN_HEIGHT, -1.2);
      glRotatef(90., 1., 0., 0.);
      this->horsepole();
    glPopMatrix();

    glEnable(GL_COLOR_MATERIAL);

    // moving horse 1
    glPushMatrix();
      glColor4f(0.855, 0.647, 0.125,1 ); // yellow
      glTranslatef(0, horse_y1, 0.);
      glRotatef((double) globalangle/2.5, 0., 1., 0.);
      glTranslatef(-1.2, 0, 0.);
      glScalef(0.25, 0.25, 0.25);
      this->drawHorse();
    glPopMatrix();
    updateHorse1(); // updates horse movement up and down

    // moving horse 2
    glPushMatrix();
      glColor4f(0.420, 0.557, 0.137, 1); // green
      glTranslatef(0, horse_y3, 0.);
      glRotatef((double) globalangle/2.5, 0., 1., 0.);
      glTranslatef(1.2, 0, 0.);
      glRotatef(180, 0., 1., 0.);
      glScalef(0.25, 0.25, 0.25);
      this->drawHorse();
    glPopMatrix();
    updateHorse3();

    // moving horse 3
    glPushMatrix();
      glColor4f(0.373, 0.620, 0.627, 1); // blue
      glTranslatef(0, horse_y2, 0.);
      glRotatef((double) globalangle/2.5, 0., 1., 0.);
      glTranslatef(0.0, 0, 1.2);
      glRotatef(90, 0., 1., 0.);
      glScalef(0.25, 0.25, 0.25);
      this->drawHorse();
    glPopMatrix();
    updateHorse2();

    // moving horse 4
    glPushMatrix();
      glColor4f(0.859, 0.439, 0.576, 1); //pink
      glTranslatef(0, horse_y4, 0.);
      glRotatef((double) globalangle/2.5, 0., 1., 0.);
      glTranslatef(0.0, 0, -1.2);
      glRotatef(-90, 0., 1., 0.);
      glScalef(0.25, 0.25, 0.25);
      this->drawHorse();
    glPopMatrix();
    updateHorse4();

    glDisable(GL_COLOR_MATERIAL);
}

void LeedsWidget::drawHorse() {
  // torso
  glPushMatrix();
    glTranslatef(0., 0., -SHOULDER_WIDTH / 2);
    this->shoulders();
  glPopMatrix();

  // front torso cover
  glPushMatrix();
    glTranslatef(0, 0, SHOULDER_WIDTH/2);
    glRotatef(180, 0.0, 1.0, 0.);
    glRotatef(180, 1.0, .0, 0.);
    glutSolidCone(SHOULDER_RADIUS, 0.6 ,15, 15);
  glPopMatrix();

  // neck
  glPushMatrix();
    glTranslatef(0., 0., SHOULDER_WIDTH / 2);
    glRotatef(-NECK_ANGLE, 1., 0., 0.);
    this->neck();
  glPopMatrix();

  // face
  glPushMatrix();
    // coz we know h, and angle, we can find the y axis
    glTranslatef(0., NECK_LENGTH*sin(-NECK_ANGLE)-0.3, (SHOULDER_WIDTH/2)+NECK_LENGTH*cos(NECK_ANGLE)+1);
    glRotatef(20, 1., 0., 0.);
    this->face();
  glPopMatrix();

  //sphere
  glPushMatrix();
    glTranslatef(0., NECK_LENGTH*sin(-NECK_ANGLE)-0.35, (SHOULDER_WIDTH/2)+NECK_LENGTH*cos(NECK_ANGLE)+1.1);
    glutSolidSphere(0.475,15,15);
  glPopMatrix();

  // butt
  glPushMatrix();
    glTranslatef(0, 0, -SHOULDER_WIDTH/2);
    glRotatef(180, 0.0, 1.0, 0.);
    glutSolidCone(SHOULDER_RADIUS, 0.5 ,15, 15);
  glPopMatrix();

  // tail
  glPushMatrix();
    glTranslatef(0, 0, -SHOULDER_WIDTH/2-0.3);
    glRotatef(120, 1.0, 0.0, 0.);
    glutSolidCone(0.2, 2 ,15, 15);
  glPopMatrix();

  // front left leg
  glPushMatrix();
    glTranslatef(SHOULDER_RADIUS/2, -0.7, (SHOULDER_WIDTH / 2.)); // -0.3 to bring it closer to the body
    glRotatef((double) legangle/-1, 1., 0., 0.);
    glRotatef(90, 1., 0., 0.);
    this->upper_leg();
    glTranslatef(0., 0., LEG_LENGTH); // now end of leg
    glRotatef((double) legangle, 1., 0., 0.);
    glRotatef(45, 1., 0., 0.);
    this->lower_leg();
  glPopMatrix();

  // front right leg
  glPushMatrix();
    glTranslatef(-SHOULDER_RADIUS/2, -0.7, (SHOULDER_WIDTH / 2.)-0.3);
    glRotatef(90, 1., 0., 0.);
    this->upper_leg();
    glTranslatef(0., 0., LEG_LENGTH);
    glRotatef((double) legangle, 1., 0., 0.);
    glRotatef(-45, 1., 0., 0.);
    this->lower_leg();
  glPopMatrix();

  // back left leg
  glPushMatrix();
    glTranslatef(SHOULDER_RADIUS/2, -0.7, (-SHOULDER_WIDTH / 2.)+0.3);
    glRotatef(90, 1., 0., 0.);
    this->upper_leg();
    glTranslatef(0., 0., LEG_LENGTH);
    glRotatef((double) legangle, 1., 0., 0.);
    glRotatef(-45, 1., 0., 0.);
    this->lower_leg();
  glPopMatrix();

  // back right leg
  glPushMatrix();
    glTranslatef(-SHOULDER_RADIUS/2, -0.7, (-SHOULDER_WIDTH / 2.)+0.3);
    glRotatef((double) legangle/-1, 1., 0., 0.);

    glRotatef(90, 1., 0., 0.);
    this->upper_leg();
    glTranslatef(0., 0., LEG_LENGTH);
    glRotatef((double) legangle, 1., 0., 0.);
    glRotatef(45, 1., 0., 0.);
    this->lower_leg();
  glPopMatrix();
}

void LeedsWidget::miniSquare() {
  // front
  glEnable(GL_TEXTURE_2D);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _image.Width(), _image.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, _image.imageField());

  glNormal3f(-1., 0., 0.);
  glBegin(GL_POLYGON);
    // texture mapping
    glTexCoord2f(0.0, 0.0);
    glVertex3f( -1.0, -1.0, 1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f( -1.0, -1.0, -1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f( -1.0,  1.0, -1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f( -1.0,  1.0,  1.0);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  // //backside
  glNormal3f(1., 0. ,0.);
  glBegin(GL_POLYGON);
  // glColor4f(0.980, 0.980, 0.824, 1);
    glVertex3f( 1.0, -1.0,  1.0);
    glVertex3f( 1.0, -1.0, -1.0);
    glVertex3f( 1.0,  1.0, -1.0);
    glVertex3f( 1.0,  1.0,  1.0);
  glEnd();

  // side
  glNormal3f(0., 0., -1.);
  glBegin(GL_POLYGON);
    // glColor4f(0.980, 0.980, 0.824, 1);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f( 1.0, -1.0, -1.0);
    glVertex3f( 1.0,  1.0, -1.0);
    glVertex3f(-1.0,  1.0, -1.0);
  glEnd();

  //side2
  glNormal3f(0., 0., 1);
  glBegin(GL_POLYGON);
    // glColor4f(0.980, 0.980, 0.824, 1);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f( 1.0, -1.0, 1.0);
    glVertex3f( 1.0,  1.0, 1.0);
    glVertex3f(-1.0,  1.0, 1.0);
  glEnd();

  // top
  glNormal3f(0, 1, 0);
  glBegin(GL_POLYGON);
    // glColor4f(0.980, 0.980, 0.824, 1);
    glVertex3f(  1.0,  1.0, 1.0);
    glVertex3f(  1.0,  1.0, -1.0);
    glVertex3f( -1.0,  1.0, -1.0);
    glVertex3f( -1.0,  1.0,  1.0);
  glEnd();

  // bottom
  glNormal3f(0, -1, 0);
  glBegin(GL_POLYGON);
    // glColor4f(0.980, 0.980, 0.824, 1);
    glVertex3f(  1.0,  -1.0,  1.0);
    glVertex3f(  1.0,  -1.0, -1.0);
    glVertex3f( -1.0,  -1.0, -1.0);
    glVertex3f( -1.0,  -1.0,  1.0);
  glEnd();


}

void LeedsWidget::drawCouncil() {
    // to allow for color, not material
    glEnable(GL_COLOR_MATERIAL);
    glColor4f(0.961, 0.902, 0.769, 1);

    // front
    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _image2.Width(), _image2.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, _image2.imageField());

    glNormal3f(-1., 0., 0.);
    glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0);
      glVertex3f( -1.0, -1.0,  2.0);
      glTexCoord2f(1.0, 0.0);
      glVertex3f( -1.0, -1.0, -2.0);
      glTexCoord2f(1.0, 1.0);
      glVertex3f( -1.0,  1.0, -2.0);
      glTexCoord2f(0.0, 1.0);
      glVertex3f( -1.0,  1.0,  2.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // //backside
    glNormal3f(1., 0. ,0.);
    glBegin(GL_POLYGON);
      glVertex3f( 1.0, -1.0,  2.0);
      glVertex3f( 1.0, -1.0, -2.0);
      glVertex3f( 1.0,  1.0, -2.0);
      glVertex3f( 1.0,  1.0,  2.0);
    glEnd();

    // side
    glNormal3f(0., 0., -1.);
    glBegin(GL_POLYGON);
      glVertex3f(-1.0, -1.0, -2.0);
      glVertex3f( 1.0, -1.0, -2.0);
      glVertex3f( 1.0,  1.0, -2.0);
      glVertex3f(-1.0,  1.0, -2.0);
    glEnd();

    //side2
    glNormal3f(0., 0., 1);
    glBegin(GL_POLYGON);
      glVertex3f(-1.0, -1.0, 2.0);
      glVertex3f( 1.0, -1.0, 2.0);
      glVertex3f( 1.0,  1.0, 2.0);
      glVertex3f(-1.0,  1.0, 2.0);
    glEnd();

    // top
    glNormal3f(0, 1, 0);
    glBegin(GL_POLYGON);
      glVertex3f(  1.0,  1.0,  2.0);
      glVertex3f(  1.0,  1.0, -2.0);
      glVertex3f( -1.0,  1.0, -2.0);
      glVertex3f( -1.0,  1.0,  2.0);
    glEnd();

    // bottom
    glNormal3f(0, -1, 0);
    glBegin(GL_POLYGON);
      glVertex3f(  1.0,  -1.0,  2.0);
      glVertex3f(  1.0,  -1.0, -2.0);
      glVertex3f( -1.0,  -1.0, -2.0);
      glVertex3f( -1.0,  -1.0,  2.0);
    glEnd();

    // left square with Marc's face
    glPushMatrix();
      glTranslatef(-0.5, 1.5, -1.5);
      glScalef(0.5, 0.5, 0.5);
      this->miniSquare();
    glPopMatrix();

    // right square with Marc's face
    glPushMatrix();
      glTranslatef(-0.5, 1.5, 1.5);
      glScalef(0.5, 0.5, 0.5);
      this->miniSquare();
    glPopMatrix();

    // roof cone
    glPushMatrix();
      glTranslatef(-0.4, 1, 0);
      glRotatef(-90, 1.,0., 0.);
      glutSolidCone(0.7, 1., 20, 5);
    glPopMatrix();

    // mini cubes right
    glPushMatrix();
      glTranslatef(-0.9, 1.125, 0.60);
      glutSolidCube(0.25);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-0.9, 1.125, 0.85);
      glutSolidCube(0.25);
    glPopMatrix();
    // mini cubes left
    glPushMatrix();
      glTranslatef(-0.9, 1.125, -0.60);
      glutSolidCube(0.25);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-0.9, 1.125, -0.85);
      glutSolidCube(0.25);
    glPopMatrix();

    //cone left
    glPushMatrix();
      glColor4f(0.961, 0.871, 0.702, 1);
      glTranslatef(-0.5, 1.5, -1.5);
      glRotatef(-90, 1.,0., 0.);
      glutSolidCone(0.3, 2.5, 10, 5);
    glPopMatrix();

    // cone right
    glPushMatrix();
      glTranslatef(-0.5, 1.5, 1.5);
      glRotatef(-90, 1.,0., 0.);
      glutSolidCone(0.3, 2.5, 10, 5);
    glPopMatrix();

    // right pillar
    glPushMatrix();
      glColor4f(0.824, 0.706, 0.549, 1);
      glTranslatef(-1, 1, 1);
      glRotatef(90, 1., 0., 0.);
      this->pillar();
    glPopMatrix();
    // left pillar
    glPushMatrix();
      glColor4f(0.824, 0.706, 0.549, 1);
      glTranslatef(-1, 1, -1);
      glRotatef(90, 1., 0., 0.);
      this->pillar();
    glPopMatrix();

    // right short pillar
    glPushMatrix();
      glColor4f(0.824, 0.706, 0.549, 1);
      glTranslatef(-1, 1, 0.5);
      glRotatef(90, 1., 0., 0.);
      this->shortpillar();
    glPopMatrix();
    // left short pillar
    glPushMatrix();
      glColor4f(0.824, 0.706, 0.549, 1);
      glTranslatef(-1, 1, -0.5);
      glRotatef(90, 1., 0., 0.);
      this->shortpillar();
    glPopMatrix();

        // // mini cubes
    glPushMatrix();
      glColor4f(	0.867, 0.776, 0.659, 1);
      glTranslatef(-1.1, -0.875, 0.);
      glutSolidCube(0.25);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-1.1, -0.875, 0.25);
      glutSolidCube(0.25);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-1.1, -0.875, 0.5);
      glutSolidCube(0.25);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-1.1, -0.875, 0.75);
      glutSolidCube(0.25);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-1.1, -0.875, 1);
      glutSolidCube(0.25);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-1.1, -0.875, 1.25);
      glutSolidCube(0.25);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-1.1, -0.875, -0.25);
      glutSolidCube(0.25);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-1.1, -0.875, -0.5);
      glutSolidCube(0.25);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-1.1, -0.875, -0.75);
      glutSolidCube(0.25);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-1.1, -0.875, -1);
      glutSolidCube(0.25);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-1.1, -0.875, -1.25);
      glutSolidCube(0.25);
    glPopMatrix();

    // for other objects to be materialized
    glDisable(GL_COLOR_MATERIAL);

}

void LeedsWidget::drawTree() {
    materialStruct *p_front = &greenMaterials;
    glMaterialfv(GL_FRONT, GL_AMBIENT, p_front->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, p_front->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, p_front->specular);
    glMaterialf(GL_FRONT, GL_SHININESS, p_front->shininess);

    glPushMatrix();
      glTranslatef(0, 3 ,0);
      glRotatef(-90, 1., 0., 0.);
      glutSolidCone(2, 2.5, 10, 2);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0, 4.5, 0);
      glRotatef(-90, 1., 0., 0.);
      glutSolidCone(1.5, 2, 10, 2);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0, 6, 0);
      glRotatef(-90, 1., 0., 0.);
      glutSolidCone(1., 1.5, 10, 2);
    glPopMatrix();

    p_front = &brassMaterials;
    glMaterialfv(GL_FRONT, GL_AMBIENT, p_front->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, p_front->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, p_front->specular);
    glMaterialf(GL_FRONT, GL_SHININESS, p_front->shininess);

    glPushMatrix();
      glTranslatef(0, 1, 0);
      glRotatef(-90, 1., 0., 0.);
      this->treetrunk();
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0, 8, 0);
      this->star();
    glPopMatrix();
}

void LeedsWidget::drawBacon() {
  // Make it rotate
  // glRotatef((double) globalangle, 0., 1., 0.);

  materialStruct *p_front = &chromeMaterials;
  glMaterialfv(GL_FRONT, GL_AMBIENT, p_front->ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, p_front->diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, p_front->specular);
  glMaterialf(GL_FRONT, GL_SHININESS, p_front->shininess);

  // below bacon
  glPushMatrix();
    glTranslatef(0., 0., 0.);
    glRotatef(70, 1., 0., 0.);
    this->bacon();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0., -0.2, 0.);
    glRotatef(-70, 1., 0., 0.);
    this->bacon();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0., 3.4 , 0.);
    glRotatef(70, 1., 0., 0.);
    this->bacon();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0., 3.3 , 0.);
    glRotatef(-70, 1., 0., 0.);
    this->bacon();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0., 0., 1.5);
    glRotatef(70, 1., 0., 0.);
    this->bacon();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0., -0.2, 1.5);
    glRotatef(-70, 1., 0., 0.);
    this->bacon();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0., 3.4 , 1.5);
    glRotatef(70, 1., 0., 0.);
    this->bacon();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0., 3.3 , 1.5);
    glRotatef(-70, 1., 0., 0.);
    this->bacon();
  glPopMatrix();
}

// called every time the widget needs painting
void LeedsWidget::paintGL() {
    // clear the widget
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, angle, 20.0, // updates with slider
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    glEnable (GL_NORMALIZE); // to normalized the normals when scaling

    // Draw background
    glPushMatrix();
      glTranslatef(0., -7, -10.);
      glRotatef(-50, 0., 1., 0.);
      this->drawPlane();
    glPopMatrix();

    // Draw Merry go round
    glPushMatrix();
      glTranslatef( 4., -6., 2);
      // glTranslatef( 5., -6., -5);
      glScalef(2, 2, 2);
      this->drawMerrygoround();
    glPopMatrix();

    // Draw bacon
    glPushMatrix();
      // glTranslatef(-7., -5., 0.);
      glTranslatef(-17., -5., -13.);
      glRotatef((double) globalangle, 0., 1., 0.);
      glTranslatef(-0.9, 0.,1);
      glRotatef(140, 0., 1., 0.);
      glScalef(1.5, 1.5, 1.5);
      this->drawBacon();
    glPopMatrix();

    //bacon plate
    glPushMatrix();
      materialStruct *p_front = &yellowMaterials;
      glMaterialfv(GL_FRONT, GL_AMBIENT, p_front->ambient);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, p_front->diffuse);
      glMaterialfv(GL_FRONT, GL_SPECULAR, p_front->specular);
      glMaterialf(GL_FRONT, GL_SHININESS, p_front->shininess);

      glTranslatef(-17., -6.7, -13.);
      glRotatef((double) globalangle, 0., 1., 0.);
      glScalef(1.3,1.5,1.3);
      glRotatef(90, 1., 0., 0.);
      glScalef(1.3, 1.3, 1.3);
      this->stage();
    glPopMatrix();


    // Draw City Council
    glPushMatrix();
      glTranslatef(-7, -3.25, -22.);
      glRotatef(130, 0., 1., 0.);
      glScalef(4, 4, 4);
      this->drawCouncil();
    glPopMatrix();

    // Draw Big horse
    glPushMatrix();
      p_front = &bronzeMaterials;
      glMaterialfv(GL_FRONT, GL_AMBIENT, p_front->ambient);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, p_front->diffuse);
      glMaterialfv(GL_FRONT, GL_SPECULAR, p_front->specular);
      glMaterialf(GL_FRONT, GL_SHININESS, p_front->shininess);

      glTranslatef(-6, 0, -1.);  // translate then rotate so that it goes around
      glRotatef((double) globalangle, 0., 1., 0.);
      glTranslatef(-2., -4.3, 0.);
      this->drawHorse();
    glPopMatrix();

    // Draw tree 1
    glPushMatrix();
      glTranslatef(7., -7., -20.);
      glScalef(1.5, 1.5, 1.5);
      this->drawTree();
    glPopMatrix();

    // Draw tree 2
    glPushMatrix();
      glTranslatef(11., -7, -15.);
      glScalef(1.25, 1.25, 1.25);
      this->drawTree();
    glPopMatrix();

    // Draw tree 3
    glPushMatrix();
      glTranslatef(15., -7., -12.);
      this->drawTree();
    glPopMatrix();

    // Draw tree 4
    glPushMatrix();
      glTranslatef(18., -7, -8.);
      glScalef(1.25, 1.25, 1.25);
      this->drawTree();
    glPopMatrix();

    // flush to screen
    glFlush();
} // paintGL()
