#pragma once

#include <QDialog>
#include "ui_levelset.h"

class LevelSet : public QDialog
{
	Q_OBJECT

public:
	LevelSet(QWidget *parent = nullptr);
	~LevelSet();
	int w = 0;
	int h = 0;
	int num = 0;

private:
	Ui::LevelSetClass ui;
	void caltextChange();
signals:
	void setSave();
};
