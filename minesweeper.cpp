#include "minesweeper.h"
#include <qpushbutton.h>
#include <qlabel.h>
#include <QDirIterator>
#include <QDebug>
#include <qtime>
#include <QMessageBox>

Minesweeper::Minesweeper(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->loadPixmap();
	this->levelSet = new LevelSet(this);
	this->uiSet = new UiSet(this);

	ui.frame->resize(this->geometry().width(), ui.frame->height());
	this->pMenuBar = new QMenuBar(this);
	pMenuBar->setFixedWidth(ui.frame->width());
	pMenuBar->setFixedHeight(25);
	QMenu* menu = new QMenu("设置");
	pMenuBar->addMenu(menu);
	QAction* levelAction = new QAction("等级");
	QAction* uiSetAction = new QAction("界面");
	menu->addAction(levelAction);
	menu->addAction(uiSetAction);
	connect(levelAction, &QAction::triggered, [=]()
		{
			levelSet->exec();
		});
	connect(uiSetAction, &QAction::triggered, [=]()
		{
			uiSet->exec();
		});
	connect(this->levelSet, &LevelSet::setSave, [=]()
		{
			int w = levelSet->w;
			int h = levelSet->h;
			int num = levelSet->num;
			this->is_begin = false;
			createLattice(w, h, num);
		});
	connect(this->uiSet, &UiSet::setSave, [=]()
		{
			this->latticeSize = uiSet->px;
			this->is_begin = false;
			createLattice(this->row, this->col, this->num);
		});
	//QPushButton* qpushbutton = new QPushButton(this);
	//connect(qpushbutton, &QPushButton::clicked, [=]()
	//    {
	//        this->createLattice(20, 20);
	//    });
	createLattice(8, 8, 10);

}
Minesweeper::~Minesweeper()
{
}

void Minesweeper::mousePressEvent(QMouseEvent* e)
{
	if (this->latticeId.length() == 2)
	{

		//按下就给几个标识赋值
		switch (e->button())
		{
		case Qt::LeftButton:
		{
			this->is_left = true;
			this->lastClickIsLeft = true;
			if (this->is_right)
			{
				this->showAroundLattice();
			}
			else
			{
				this->showLattice();
			}
			break;
		}
		case Qt::RightButton:
		{
			this->is_right = true;
			this->lastClickIsLeft = false;
			if (this->is_left)
			{
				this->showAroundLattice();
			}
			else
			{
				this->flagLattice();
			}
			break;
		}
		case Qt::MiddleButton:
			this->is_mid = true;
			this->lastClickIsLeft = false;
			this->showAroundLattice();
			break;
		default:
			break;
		}
	}
	latticeChange(this->latticeId);
}

void Minesweeper::mouseMoveEvent(QMouseEvent* e)
{
	if (qobject_cast <Lattice*>(childAt(e->pos())))
	{
		this->latticeId = qobject_cast <Lattice*>(childAt(e->pos()))->id;
	}
	if (this->is_left && !this->is_right)
	{
		this->showLattice();
	}
	else if (this->is_mid || (this->is_left && this->is_right))
	{
		this->showAroundLattice();
	}
	latticeChange(this->latticeId);
}

void Minesweeper::mouseReleaseEvent(QMouseEvent* e)
{
	if (this->latticeId.length() == 2)
	{
		//松开就给几个标识赋值
		switch (e->button())
		{
		case Qt::LeftButton:
		{
			this->is_left = false;
			if (this->is_right)
			{
				this->openAroundLattice();
			}
			else if (this->lastClickIsLeft)
			{
				this->openLattice();
			}
			break;
		}
		case Qt::RightButton:
		{
			this->is_right = false;
			if (this->is_left)
			{
				this->openAroundLattice();
			}
			break;
		}
		case Qt::MiddleButton:
			this->is_mid = false;
			this->openAroundLattice();
			break;
		default:
			break;
		}
		//只要抬起鼠标就清空所有展示的格子
		foreach(QList<int> var, this->showLatticeList)
		{
			this->latticeList[var[0]][var[1]]->is_show = false;
			this->changeLatticeList.insert(var);
		}
	}
	latticeChange(this->latticeId);
}

void Minesweeper::showLattice()
{
	foreach(QList<int> var, this->showLatticeList)
	{
		this->latticeList[var[0]][var[1]]->is_show = false;
		this->changeLatticeList.insert(var);
	}
	this->showLatticeList.clear();
	this->latticeList[this->latticeId[0]][this->latticeId[1]]->is_show = true;
	this->showLatticeList.append(this->latticeId);
	this->changeLatticeList.insert(this->latticeId);
	qDebug() << "showLattice" << this->latticeId << this->latticeList[this->latticeId[0]][this->latticeId[1]]->is_mine << this->latticeList[this->latticeId[0]][this->latticeId[1]]->num
		<< this->changeLatticeList;
}

void Minesweeper::showAroundLattice()
{
	foreach(QList<int> var, this->showLatticeList)
	{
		this->latticeList[var[0]][var[1]]->is_show = false;
		this->changeLatticeList.insert(var);
	}
	this->showLatticeList.clear();
	foreach(QList<int> var, this->latticeList[this->latticeId[0]][this->latticeId[1]]->AroundList)
	{
		this->latticeList[var[0]][var[1]]->is_show = true;
		this->showLatticeList.append(var);
		this->changeLatticeList.insert(var);
	}

	qDebug() << "showAroundLattice" << this->latticeId;
}

void Minesweeper::openLattice()
{
	if (!is_begin)
	{
		this->is_begin = true;
		calMineNum();
	}
	if (!this->latticeList[this->latticeId[0]][this->latticeId[1]]->is_flag)
	{
		this->latticeList[this->latticeId[0]][this->latticeId[1]]->is_open = true;
		this->changeLatticeList.insert(this->latticeId);
	}
	qDebug() << "openLattice" << this->latticeId;
}

void Minesweeper::openAroundLattice()
{
	qDebug() << "openAroundLattice" << this->latticeId;
}

void Minesweeper::flagLattice()
{
	//如果没有打开就处理标记的标志
	bool is_flag = this->latticeList[this->latticeId[0]][this->latticeId[1]]->is_flag;
	this->latticeList[this->latticeId[0]][this->latticeId[1]]->is_flag = is_flag ? false : true;
	this->changeLatticeList.insert(this->latticeId);
	qDebug() << "flagLattice" << this->latticeId;
}



void Minesweeper::calMineNum()
{
	QList<QList<int>> mineList;
	mineList.append(this->latticeId);
	for (int i = 0; i < this->num; i++)
	{
		srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
		int randrow = rand() % this->row;
		int randcol = rand() % this->col;
		while (mineList.contains({ randcol,randrow }))
		{
			randrow = rand() % this->row;
			randcol = rand() % this->col;
		}
		mineList.append({ randcol,randrow });
		this->latticeList[randcol][randrow]->is_mine = true;
		foreach(auto var, this->latticeList[randcol][randrow]->AroundList)
		{
			this->latticeList[var[0]][var[1]]->num++;
		}
	}
}

void Minesweeper::loadPixmap()
{
	QString path = QDir::currentPath() + "/res/lattice/";//获取当前目录
	QDir dir(path);
	QStringList filename;
	filename << "*.png" << "*.jpg";
	QStringList results;
	results = dir.entryList(filename, QDir::Files | QDir::Readable, QDir::Name);
	foreach(QString filename, results)
	{
		QPixmap pixmap;
		pixmap.load(path + filename);
		pixmap = pixmap.scaled(this->latticeSize, this->latticeSize);
		QString newFileName = filename.split(".")[0];
		qDebug() << newFileName;
		latticePix.insert(newFileName, pixmap);
	}
}

void Minesweeper::createLattice(int row, int col, int num)
{
	this->loadPixmap();//初始化pixmap
	//删除所有的Lattice类
	this->latticeList.clear();
	QList<Lattice*> labels = this->findChildren<Lattice*>();
	foreach(Lattice * label, labels)
	{
		label->deleteLater();
	}
	this->row = row;
	this->col = col;
	this->num = num;
	for (int i = 0; i < col; i++)
	{
		QList<Lattice* > latrowList;
		for (int j = 0; j < row; j++)
		{
			int x = 50 + j * this->latticeSize;
			int y = 100 + i * this->latticeSize;
			Lattice* lattice = new Lattice(this);
			lattice->id = { i,j };
			lattice->setGeometry(x, y, this->latticeSize, this->latticeSize);
			lattice->setPixmap(this->latticePix["on"]);
			//lattice->setFrameShape(QFrame::Box);
			for (int g = i - 1; g <= i + 1; g++)
			{
				for (int k = j - 1; k <= j + 1; k++)
				{
					if (g >= 0 && g < col && k >= 0 && k < row)
					{
						lattice->AroundList.append({ g,k });
					}
				}
			}
			lattice->show();
			connect(lattice, SIGNAL(enter(QList<int>)), this, SLOT(latticeChange(QList<int>)));
			connect(lattice, SIGNAL(leave(QList<int>)), this, SLOT(latticeChange(QList<int>)));
			latrowList.append(lattice);
		}
		this->latticeList.append(latrowList);
	}
	this->setFixedWidth(100 + this->row * this->latticeSize);
	this->setFixedHeight(150 + this->col * this->latticeSize);
	ui.frame->resize(this->geometry().width(), ui.frame->height());
	this->pMenuBar->setFixedWidth(ui.frame->width());
}
void Minesweeper::latticeChange(QList<int> id)
{
	this->latticeId = id;
	if (this->changeLatticeList.count() > 0)
	{
		foreach(QList<int> var, this->changeLatticeList)
		{
			bool is_open = this->latticeList[var[0]][var[1]]->is_open;//是不是打开的状态
			bool is_flag = this->latticeList[var[0]][var[1]]->is_flag;;//是不是标记的状态
			bool is_show = this->latticeList[var[0]][var[1]]->is_show;;//是不是展示的状态
			bool is_mine = this->latticeList[var[0]][var[1]]->is_mine;//是不是雷的状态
			int num = this->latticeList[var[0]][var[1]]->num;//周围雷数
			//qDebug() << is_open << is_flag << is_show;
			if (!is_open && !is_flag)
			{
				if (is_show)
				{
					this->latticeList[var[0]][var[1]]->setPixmap(this->latticePix["back"]);
				}
				else
				{
					this->latticeList[var[0]][var[1]]->setPixmap(this->latticePix["on"]);
				}
			}
			else if (is_open && !is_flag)
			{
				if (is_mine)
				{
					this->latticeList[var[0]][var[1]]->setPixmap(this->latticePix["mine"]);
				}
				else
				{
					QString pixmapName = "";
					switch (num)
					{
					case 0:
						pixmapName = "back";
						break;
					case 1:
						pixmapName = "one";
						break;
					case 2:
						pixmapName = "two";
						break;
					case 3:
						pixmapName = "three";
						break;
					case 4:
						pixmapName = "four";
						break;
					case 5:
						pixmapName = "five";
						break;
					case 6:
						pixmapName = "six";
						break;
					case 7:
						pixmapName = "seven";
						break;
					case 8:
						pixmapName = "eight";
						break;
					default:
						break;
					}
					if (pixmapName != "") this->latticeList[var[0]][var[1]]->setPixmap(this->latticePix[pixmapName]);
				}
			}
			else if (!is_open && is_flag)
			{
				this->latticeList[var[0]][var[1]]->setPixmap(this->latticePix["right"]);
			}
		}
		this->changeLatticeList.clear();
	}
}