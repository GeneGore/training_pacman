#ifndef PACMAN_H
#define PACMAN_H

#include "Creature.h"
#include "clMsEvent.h"
#include <QMainWindow>
#include "ui_pacman.h"


class Pacman : public QMainWindow
{
	Q_OBJECT

public slots:
	void okno();

public:
	Pacman(QWidget *parent = 0);
	~Pacman();

	QGraphicsPixmapItem* phero;
	QGraphicsPixmapItem* pbrick[20];
	QGraphicsScene *scene;

	

	int hX;
	int hY;

	int heroWidth = 30;
	int heroHeight = 30;

	int countWall=20; //количество стеновых блоков
	int massCoordWallX[20]; //рандомные координаты стенового блока Х
	int massCoordWallY[20]; // рандомные координаты стенового блока У
QGraphicsPixmapItem* pvirtbrick[20];
	int countStepsInGals; //счетчик шагов на участке прямолинейного движения

	int radEll;
	int deltaEll;
	int yesIter;

	int hDefence, hAttack, hHitPoints, hMana;
	

	Creature a[7]; //монстры и герой
	QGraphicsScene *virtscene;
	int countMonstr=7;
	int massCoordMonstrX[7];
	int massCoordMonstrY[7];
	QGraphicsPixmapItem* pvirtstar[7];

	

	QGraphicsEllipseItem* qgEll;

	////
	

	void keyPressEvent(QKeyEvent*);
	//virtual void mousePressEvent(QGraphicsSceneMouseEvent*);
	//virtual bool eventFilter(QObject*, QEvent*);

	void buildWall();
	bool thisPositionIsEmpty(QGraphicsScene*, int, int);
	void setMonsters();
	void RandomForAll();

	void mMove(Creature&);
	void detectionHero(Creature&); // ищем толстого 
	int angle; //угол, типа пеленг

	void allStuff();
	bool allDead();

	public slots:
	void drawAttack();
	void yesFunc();


private:
	Ui::PacmanClass ui;
};


#endif // PACMAN_H
