#pragma once
#include <qlabel.h>
#include <qevent.h>
class Lattice  : public QLabel
{
	Q_OBJECT

public:
	explicit Lattice(QWidget* parent = 0);
	~Lattice();
	bool is_set = false;
	QList<QList<int>> AroundList;
	QList<int> id; //格子的ID
	int row = 0;//所在行
	int col = 0;//所在列
	int num = 0;//周围的雷数
	bool is_mine = false;//是不是雷
	bool is_open = false;//是不是打开的状态
	bool is_flag = false;//是不是标记的状态
	bool is_show = false;//是不是展示的状态
	void enterEvent(QEnterEvent* e);
	void leaveEvent(QEvent* e);
	//void mouseMoveEvent(QMouseEvent* e);
signals:
	void enter(QList<int> id);
	void leave(QList<int> id);
	void move();

};
