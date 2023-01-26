#include "uiset.h"

UiSet::UiSet(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton_ok, &QPushButton::clicked, [=]()
		{
			this->px = ui.comboBox_lapx->currentText().toInt();
			this->setSave(), this->close();
		});
	connect(ui.pushButton_cancel, &QPushButton::clicked, [=]() {this->close(); });
}

UiSet::~UiSet()
{}
