#include "LeedsWindow.h"
#include <iostream>

// constructor / destructor
LeedsWindow::LeedsWindow(QWidget *parent)
        : QWidget(parent) { // constructor

    // create menu bar
    menuBar = new QMenuBar(this);

    // create file menu
    fileMenu = menuBar->addMenu("&File");

    // create the action
    actionQuit = new QAction("&Quit", this);


    // add the item to the menu
    fileMenu->addAction(actionQuit);

    // create the window layout
    windowLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    // create main widget
    cubeWidget = new LeedsWidget(this);
    windowLayout->addWidget(cubeWidget);

    // create slider
    nSlider = new QSlider(Qt::Horizontal);
            connect(nSlider, SIGNAL(valueChanged(int)), cubeWidget, SLOT(updateLegAngle(int)));
            nSlider->setMinimum(0);
            nSlider->setMaximum(90);
            nSlider->setValue(0);
            windowLayout->addWidget(nSlider);

    nSlider2 = new QSlider(Qt::Horizontal);
            connect(nSlider2, SIGNAL(valueChanged(int)), cubeWidget, SLOT(updateAngle(int)));
            nSlider2->setMinimum(4);
            nSlider2->setMaximum(20);
            nSlider2->setValue(0);
            windowLayout->addWidget(nSlider2);

    ptimer = new QTimer(this);
            connect(ptimer, SIGNAL(timeout()), cubeWidget, SLOT(updateGlobalAngle()));
            ptimer->start(0);
} // constructor

LeedsWindow::~LeedsWindow() { // destructor
    delete ptimer;
    delete nSlider2;
    delete nSlider;
    delete cubeWidget;
    delete windowLayout;
    delete actionQuit;
    delete fileMenu;
    delete menuBar;
} // destructor

// resets all the interface elements
void LeedsWindow::ResetInterface() { // ResetInterface()


    // now force refresh
    cubeWidget->update();
    update();
} // ResetInterface()
