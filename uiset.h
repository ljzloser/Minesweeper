#pragma once

#include <QDialog>
#include "ui_uiset.h"

class UiSet : public QDialog
{
	Q_OBJECT

public:
	UiSet(QWidget *parent = nullptr);
	~UiSet();
	int px = 30;
private:
	Ui::UiSetClass ui;
signals:
	void setSave();
};
