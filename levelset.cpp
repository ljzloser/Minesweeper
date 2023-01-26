#include "levelset.h"
#include <qregularexpression.h>
#include <qvalidator.h>
LevelSet::LevelSet(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.checkBox_beg, &QCheckBox::clicked, [=]()
		{
			ui.lineEdit_w->setText("8");
			ui.lineEdit_h->setText("8");
			ui.lineEdit_num->setText("10");
			float hundred = 10.0 / 8.0 / 8.0;
			ui.lineEdit_w->setReadOnly(true);
			ui.lineEdit_h->setReadOnly(true);
			ui.lineEdit_num->setReadOnly(true);
			ui.label_hundred->setText(QString::number(hundred * 100, 'f', 2) + "%");
		});
	connect(ui.checkBox_int, &QCheckBox::clicked, [=]()
		{
			ui.lineEdit_w->setText("16");
			ui.lineEdit_h->setText("16");
			ui.lineEdit_num->setText("40");
			float hundred = 40.0 / 16.0 / 16.0;
			ui.lineEdit_w->setReadOnly(true);
			ui.lineEdit_h->setReadOnly(true);
			ui.lineEdit_num->setReadOnly(true);
			ui.label_hundred->setText(QString::number(hundred * 100, 'f', 2) + "%");
		});
	connect(ui.checkBox_exp, &QCheckBox::clicked, [=]()
		{
			ui.lineEdit_w->setText("30");
			ui.lineEdit_h->setText("16");
			ui.lineEdit_num->setText("99");
			float hundred = 99.0 / 30.0 / 16.0;
			ui.lineEdit_w->setReadOnly(true);
			ui.lineEdit_h->setReadOnly(true);
			ui.lineEdit_num->setReadOnly(true);
			ui.label_hundred->setText(QString::number(hundred * 100, 'f', 2) + "%");
		});
	connect(ui.checkBox_custom, &QCheckBox::clicked, [=]()
		{
			ui.lineEdit_w->setReadOnly(false);
			ui.lineEdit_h->setReadOnly(false);
			ui.lineEdit_num->setReadOnly(false);
		});
	connect(ui.lineEdit_h, &QLineEdit::textChanged, [=]()
		{
			caltextChange();
		});
	connect(ui.lineEdit_w, &QLineEdit::textChanged, [=]()
		{
			caltextChange();
		});
	connect(ui.lineEdit_num, &QLineEdit::textChanged, [=]()
		{
			caltextChange();
		});
	connect(ui.pushButton_save, &QPushButton::clicked, [=]() 
		{
		this->w = ui.lineEdit_w->text().toInt();
		this->h = ui.lineEdit_h->text().toInt();
		this->num = ui.lineEdit_num->text().toInt();  this->setSave(), this->close();
		});
	connect(ui.pushButton_cancel, &QPushButton::clicked, [=]() {this->close(); });
	QRegularExpression re("^([1-9][0-9][0-9]?)$");
	QRegularExpressionValidator* reval = new QRegularExpressionValidator(re);
	ui.lineEdit_h->setValidator(reval);
	ui.lineEdit_w->setValidator(reval);
	ui.lineEdit_num->setValidator(reval);
	ui.checkBox_beg->setChecked(true);
	ui.lineEdit_w->setText("8");
	ui.lineEdit_h->setText("8");
	ui.lineEdit_num->setText("10");
	float hundred = 10.0 / 8.0 / 8.0;
	ui.lineEdit_w->setReadOnly(true);
	ui.lineEdit_h->setReadOnly(true);
	ui.lineEdit_num->setReadOnly(true);
	ui.label_hundred->setText(QString::number(hundred * 100, 'f', 2) + "%");
}

LevelSet::~LevelSet()
{}

void LevelSet::caltextChange()
{
	int w = ui.lineEdit_w->text().toInt();
	int h = ui.lineEdit_h->text().toInt();
	int num = ui.lineEdit_num->text().toInt();
	float hundred = double(num) / w / h;
	ui.label_hundred->setText(QString::number(hundred * 100, 'f', 2) + "%");
}
