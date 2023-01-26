#pragma once

#include <QtWidgets/QWidget>
#include <QMenuBar>
#include "ui_minesweeper.h"
#include "lattice.h"
#include "levelset.h"
#include "uiset.h"
class Minesweeper : public QWidget
{
	Q_OBJECT

public:
	Minesweeper(QWidget* parent = nullptr);
	~Minesweeper();
	QMap<QString, QPixmap> latticePix;
	void loadPixmap();


private:
	Ui::MinesweeperClass ui;
	QList<QList<Lattice*>> latticeList;
	QList<QList<int>> showLatticeList;//记录展示的格子
	QSet<QList<int>> changeLatticeList;//0记录发生状态变化的格子
	void createLattice(int row, int col, int num);
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
	int row = 0;
	int col = 0;
	int num = 0;
	QList<int> latticeId;
	bool is_left = false;
	bool is_right = false;
	bool is_mid = false;
	//显示该格子
	void showLattice();
	//显示周围及自身的格子
	void showAroundLattice();
	//打开这个格子
	void openLattice();
	//打开自身和周围的格子
	void openAroundLattice();
	//标记格子
	void flagLattice();
	bool lastClickIsLeft = false;
	LevelSet* levelSet;
	QMenuBar* pMenuBar;
	UiSet* uiSet;
	bool is_begin = false;
	int latticeSize = 30;
	//布雷
	void calMineNum();

public slots:
	void latticeChange(QList<int> id);
};
