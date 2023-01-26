#include "lattice.h"

Lattice::Lattice(QWidget *parent) :QLabel(parent)
{}

Lattice::~Lattice()
{}

void Lattice::enterEvent(QEnterEvent * e)
{
	this->enter(this->id);
}

void Lattice::leaveEvent(QEvent* e)
{
	this->leave({});
}

//void Lattice::mouseMoveEvent(QMouseEvent* e)
//{
//	this->enter(this->id);
//	this->move();
//	qDebug() << "ssss";3
//}
