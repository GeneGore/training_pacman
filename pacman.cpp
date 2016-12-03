#include "pacman.h"

#include <QtGui>
#include <ctime> //

Pacman::Pacman(QWidget *parent) ///////�����������//////////////////////
	: QMainWindow(parent)
{ 
	ui.setupUi(this);
	
	time_t qTime;
	time(&qTime);
	srand((unsigned int)qTime);

	RandomForAll();
	
	for (int i = 0; i < countMonstr - 1; i++){ // ��������� ���������� �������� ������ � ������ �������� ������ Creature
		a[i].X=massCoordMonstrX[i];
		a[i].Y=massCoordMonstrY[i];
	}

	hX = massCoordMonstrX[countMonstr - 1]; //��������� ���������� �����, ����������� � ����������� �� ������ �����
	hY = massCoordMonstrY[countMonstr - 1]; // ��� ��������� �������� �������� ���������, ������ ������ ��������

	okno(); //������ ����
	allStuff();
	
	QTimer* ptimer = new QTimer(this); // ������
	connect(ptimer, SIGNAL(timeout()), SLOT(okno())); // ������ ������� timeout ������ �� ������ okno()
	ptimer->start(25); // ������������� �������


	countStepsInGals = 0; //��������� �������� �������� ����� �� ������� �������������� ��������
	angle = 0;


	radEll = 0; //��������� ������ ���������
	deltaEll = 5;
	yesIter = 0;

	/*QColor col(Qt::green);
	QBrush brush1(col, Qt::NoBrush);
	QPen ell(Qt::green, 1);
	qgEll = scene->addEllipse(-50, -50, 50, 50, ell, brush1);*/

	/////////////////������� ���� ������� � ��� ����������//////
	for (int i = 0; i < countMonstr - 1; i++){
		//srand(time(NULL));
		a[i].mType = rand() % 3;


		switch (a[i].mType){
		case 0:
			a[i].searchRadius = a[i].searchRadius * 3;
			a[i].mAttack = a[i].mAttack * 1;
			a[i].mDefence = a[i].mDefence * 1;
			a[i].mHitPoints = a[i].mHitPoints * 1;
			a[i].step = a[i].step * 2;
			break;
		case 1:
			a[i].searchRadius = a[i].searchRadius * 2;
			a[i].mAttack = a[i].mAttack * 2;
			a[i].mDefence = a[i].mDefence * 2;
			a[i].mHitPoints = a[i].mHitPoints * 1;
			a[i].step = a[i].step * 5;
			break;

		case 2:
			a[i].searchRadius = a[i].searchRadius * 1;
			a[i].mAttack = a[i].mAttack * 3;
			a[i].mDefence = a[i].mDefence * 3;
			a[i].mHitPoints = a[i].mHitPoints * 1;
			a[i].step = a[i].step *10;
			break;
		}
	}
	//������� ���������� �����

	hDefence = 100;
	hAttack = 100;
	hHitPoints=500;
	hMana = 1000;
	/*a[countMonstr].mType = 3;
	a[countMonstr].mHitPoints = 100;
	a[countMonstr].mAttack = 100;
	//a[countMonstr].mDefence = 100; //������ ������??? �����-�� ���� � �������*/


	QObject::connect(ui.mahButton1, SIGNAL(clicked()), this, SLOT(yesFunc()));

	


} /////////////////// ����� ������������//////////////////////////

void Pacman::allStuff(){

	ui.graphicsView->installEventFilter(new clMsEvent(ui.graphicsView)); /// ������ �������
	

	QMenu* pmnuFile = new QMenu("&File");
	pmnuFile->addAction("&New");
	pmnuFile->addAction("&Load");
	pmnuFile->addAction("&Save");
	pmnuFile->addAction("&Exit");
	menuBar()->addMenu(pmnuFile);

	QMenu* pmnuFile1 = new QMenu("&Edit");
	pmnuFile1->addAction("&Edit Map");
	pmnuFile1->addAction("&Open Map");
	pmnuFile1->addAction("&I don't know");
	menuBar()->addMenu(pmnuFile1);


	addToolBar(ui.mainToolBar);
	//ui.mainToolBar->addAction(QPixmap(image_xpm), "������ xpm", this, SLOT(slotNoImp()));
	ui.mainToolBar->addAction(QPixmap("Resources/knop.bmp"), "GOD MODE", this, SLOT()); //
	ui.mainToolBar->addSeparator();
	ui.mainToolBar->addAction(QPixmap("Resources/knop2.bmp"), "GOD MODE", this, SLOT()); //

	//������ ���������
	QLabel* l1 = new QLabel("���������: ");
	statusBar()->addWidget(l1);
	QLabel* l2 = new QLabel("��� �����");
	statusBar()->addWidget(l2);

	QPixmap pix1("Resources/ma1.bmp");
	ui.mahButton1->setIcon(pix1);

	QPixmap pix2("Resources/ma2.bmp");
	ui.mahButton2->setIcon(pix2);


}



void Pacman::okno(){ // ��� ���� � ����� � ��������, � ������� ������ ������ ������ timeout. ������ �����������
	

	scene = new QGraphicsScene(ui.graphicsView); // ������ ����� � ���������� �� � ������������� ui.graphicsView

	scene->setBackgroundBrush(Qt::black); // ������������
	scene->setSceneRect(0,0,445,445); // ������������ ������ �����

	//scene->installEventFilter(this); /////////////////////////

	buildWall(); // ������ �����
	setMonsters(); //������ �� ��������� ����� �������� � �����

	//������� ��������
	for (int i = 0; i < countMonstr; i++){
		detectionHero(a[i]);
		mMove(a[i]);
	}

	
	if (0 < yesIter && yesIter < 100 && hMana>0){
		radEll = 50;
		deltaEll = -9;
		drawAttack();
		yesIter++;
	}
	else { yesIter = 0; radEll = 1; deltaEll = 5; }


	if (hMana < 1000){ hMana=hMana+1;}
	//��� ������ ������: �����������, ������� ���� � �.�.
	//����� Magic: ������������ �����������...

	//QBrush brush1(Qt::NoBrush);
	//QBrush brush1(Qt::Dense5Pattern);

	/*QColor col(Qt::black);
	QBrush brush1(col, Qt::Dense3Pattern); //��� ������� ������� ��� �����������
	QPen ell(Qt::transparent, 1);
	scene->addEllipse(hX-7, hY-7, 45, 45, ell, brush1);
	QPen pen(Qt::red, 10);
	scene->addLine(aa, bb, aa + 30, bb + 30, pen);*/

	ui.graphicsView->setScene(scene);//���������� �����!

}


//////////////////////////////����� ������� ������
void Pacman::keyPressEvent(QKeyEvent* pe){

	int tempX=hX; // ��������� ���������� ��� �������� ��������� �����
	int tempY=hY; // �����, ����� ����� ���� ���������, �� ���������� �� ����� �� ������
	
	switch (pe->key()) //������� key ������ �������� ������� �������
	{
	case Qt::Key_W:
		tempY = hY - 10; // ����� - ��� �������� ���� �����
		//�������� ����� W
		if (-2<tempY && scene->itemAt(tempX, tempY) == 0 && scene->itemAt(tempX + heroWidth, tempY) == 0){ //
			//-2<tempY ����������� ���� ����� �� ������� �������� ����
			// scene->itemAt(tempX, tempY) == 0 �� ����������� ����������� ������ ��� (�.�.itemAt(tempX, tempY) �� ������ ��������� �� �� ���� ������ ������ QGraphicsPixmapItem)
			// �����: ���������� ������ �� ������� ������ ���� ��������, ������� ����� ��������� ������� ������ ���� � ������� ������� scene->itemAt(tempX + 48, tempY) == 0
			// ����� ����� ����� ������������ � ����� ����� (��� �������� ������ ���� �������� �� ������� �������� "������" ����� �� �������� ����) ��� �� ������ ������ �����
			hX = tempX;
			hY = tempY;
		}
		break;
	case Qt::Key_S:
		tempY = hY + 10;
		//�������� ���� S
		if (420>tempY && scene->itemAt(tempX, tempY + heroHeight) == 0 && scene->itemAt(tempX + heroWidth, tempY + heroHeight) == 0){
			hX = tempX;
			hY = tempY;
		}
		break;
	case Qt::Key_A:
		tempX = hX - 10;
		//�������� ����� A
		if (-2<tempX && scene->itemAt(tempX, tempY) == 0 && scene->itemAt(tempX, tempY + heroHeight) == 0){
			hX = tempX;
			hY = tempY;
		}
		break;
	case Qt::Key_D:
		tempX = hX + 10;
		//�������� ������ D
		if (402>tempX && scene->itemAt(tempX + heroWidth, tempY) == 0 && scene->itemAt(tempX + heroWidth, tempY + heroHeight) == 0){
			hX = tempX;
			hY = tempY;
		}
		break;
	default:
		QWidget::keyPressEvent(pe);
	}
}

/*void Pacman::mousePressEvent(QGraphicsSceneMouseEvent* pe){
	

QMessageBox::information(0, "Message","Be smart!");
	

	

	/*QPointF mCoord;
	mCoord = pe->screenPos();
	
	//return mCoord;
	//QBrush brush1(col, Qt::NoBrush);
QPen pen(Qt::red, 10);
//int aa,bb;
aa = mCoord.x();
bb = mCoord.y();
	scene->addLine(aa,bb,aa+30,bb+30, pen);
}*/

/*bool Pacman::eventFilter(QObject *target, QEvent *event){

	if (target == scene) {
		QMessageBox::information(0, "Message", "Be smart!");
		if (event->type() == QEvent::KeyPress) {
			//QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			QMessageBox::information(0, "Message", "Be smart!2");
		}
	}
	return eventFilter(target, event);


}*/


void Pacman::RandomForAll(){//��� ������� ������ ��� ��������� ��������� �������� � ������ ��������� �� ���� ������� ����, �������� �� �� ������ �����

	virtscene = new QGraphicsScene; //  ����� �����, �� ������� ��� �������
	virtscene->setSceneRect(0, 0, 445, 445);//

	QPixmap virtbrick("Resources/virtbrick.bmp");//
	//���������� ����
	for (int i = 0; i < countWall; i++){//������ ���������� ���� � �������� �� ���� ����� ��� ��������, �� ��������� ��,  � ���� ��� ����� - ��������� � ������
		//������ �������, ��� ����� ��������� ������ �� ��� ���� � ��������
		
		bool a = false;
		pvirtbrick[i] = virtscene->addPixmap(virtbrick);
		while (a != true){
			int X = 48 * (rand() % 9);
			int Y = 45 * (rand() % 9);
			if (thisPositionIsEmpty(virtscene, X, Y) == true){ //�������, ��� �� �� ���� ����� �������� 
				pvirtbrick[i]->moveBy(X, Y);
				massCoordWallX[i] = X;
				massCoordWallY[i] = Y;
				a = true;
			}
		}
	}

	QPixmap virtstar("Resources/virtstar.bmp");
	//��������� ���������� ��������
	for (int i = 0; i < countMonstr; i++){
		bool a = false;
		pvirtstar[i] = virtscene->addPixmap(virtstar);
		while (a != true){
			int X = 48 * (rand() % 9);
			int Y = 45 * (rand() % 9);
			if (thisPositionIsEmpty(virtscene, X, Y) == true){ //�������, ��� �� �� ���� ����� �������� 
				pvirtstar[i]->moveBy(X, Y);
				massCoordMonstrX[i]=X;
				massCoordMonstrY[i]=Y;
				a = true;
			}
		}
	}
}

void Pacman::buildWall(){ //������ �����

	QPixmap brick("Resources/brick.bmp");
	
	for (int i = 0; i < countWall; i++){
		pbrick[i] = scene->addPixmap(brick);
		
		//��������
		/*QFont font("Times", 8, QFont::Bold);
		QGraphicsSimpleTextItem* stext;
		stext = scene->addSimpleText(QString::number(i), font);
		stext->moveBy(massCoordWallX[i]+15, massCoordWallY[i]+15);*/ //����� �� ����� ��� �����


		pbrick[i]->moveBy(massCoordWallX[i], massCoordWallY[i]);
		
		//��������!
		//QPen penDot1(Qt::green, 3);
		//scene->addLine(massCoordWallX[i], massCoordWallY[i], massCoordWallX[i], massCoordWallY[i], penDot1); //������ ����� �� ������� ����� ����

	}

}

void Pacman::setMonsters(){ // ���������� �������� � ����� �� ����������� �����������

	int deadmans=0;
	QFont font("Times", 6, QFont::Bold);
	QGraphicsTextItem* text;
	QColor clr(Qt::green);
	
	QColor col(Qt::green);
	QBrush brush1(col, Qt::NoBrush);
	QPen ell(Qt::yellow, 1);
	
	qgEll = scene->addEllipse(hX + deltaEll, hY + deltaEll, radEll, radEll, ell, brush1);
	//qgEll->hide();
	if (hHitPoints < 1) { scene->removeItem(qgEll); }

	QPixmap star1("Resources/star1.bmp");
	QPixmap star2("Resources/star2.bmp");
	QPixmap star3("Resources/star3.bmp");
	QPixmap hero("Resources/hero.bmp"); //������ ������ �������, ���������� ������� �����
	
	phero = scene->addPixmap(hero);

	for (int i = 0; i < countMonstr - 1; i++){
	    switch (a[i].mType){
		      case 0: a[i].monstr = scene->addPixmap(star1); break;
		      case 1: a[i].monstr = scene->addPixmap(star2); break;
		      case 2: a[i].monstr = scene->addPixmap(star3); break;
	    }

		text = scene->addText(QString::number(a[i].mHitPoints), font); //������������� � ����� ����� ����������
		text->setDefaultTextColor(clr); // ��������� ����
		text->moveBy(a[i].X, a[i].Y); // �������� �� ����������� �������

        if (a[i].mHitPoints>0){
		   a[i].monstr->moveBy(a[i].X, a[i].Y); // ���� ������ ������, ������� ��� �� �����������
		}
		else {
			scene->removeItem(a[i].monstr);//���� �����, ������� ��� �� �����
			deadmans++;
			scene->removeItem(text);        // � ��� ������� ���������� ����
		}
		
		if (a[i].monstr->collidesWithItem(qgEll)&&yesIter!=0){ //��� ����������� ������� ������ � ������� �������� ��� ������
		 a[i].mHitPoints = a[i].mHitPoints - 1; 
		}
	}
	
	if (hHitPoints < 1) { scene->removeItem(phero); }
	
	ui.lhHP->setText(QString::number(hHitPoints));
	ui.lhAtt->setText(QString::number(hAttack));
	ui.lhDef->setText(QString::number(hDefence));
	ui.manaBar->setValue(hMana);

	ui.score->setText(QString::number(deadmans));
	phero->moveBy(hX, hY);
}

bool Pacman::thisPositionIsEmpty(QGraphicsScene* sc, int posX, int posY){

	if (sc->itemAt(posX, posY) == 0  &&
		sc->itemAt(posX + 48, posY) == 0 &&
		sc->itemAt(posX, posY + 45) == 0 &&
		sc->itemAt(posX + 48, posY + 45) == 0){
		return true;
	}
	else false;
}

void Pacman::detectionHero(Creature& m){

    int pi = 3.14;
	QPen penRay(Qt::yellow, 1);//���

		QLine ray(m.X + 15, m.Y + 15, m.X + 15 + m.searchRadius*qCos(angle*pi / 180), m.Y + 15 + m.searchRadius*qSin(angle*pi / 180));
		QGraphicsLineItem* pline = scene->addLine(ray, penRay);
		pline->hide();

	
	
	if (hHitPoints > 0){
		
		if (angle > 360){
			angle = 0;
		}

		int futX = m.X + m.step *qCos(angle*pi / 180);
		int futY = m.Y + m.step *qSin(angle*pi / 180);

		if (pline->collidesWithItem(phero)){
			//�������� ���� � ������� �������� ��� ���:
			if (scene->itemAt(futX, futY) != *pbrick){ //������ ����� ������� ������������

				m.X = m.X + m.step * qCos(angle*pi / 180);
				m.Y = m.Y + m.step *qSin(angle*pi / 180);
			}
			//if (scene->itemAt(futX, futY) == phero){ //������ ����� ������� ������������ + ������ "���������" �� �����
			if (m.monstr->collidesWithItem(phero)&&m.mHitPoints>0){
				hHitPoints = hHitPoints - 1;
			}
		}
		angle = angle + 10;
	}
	else {
		 scene->removeItem(pline);
	}
}

//

void Pacman::mMove(Creature& a){

	int tempX = a.X; // ��������� ���������� ��� �������� ���������
	int tempY = a.Y; // �����, ����� ����� ���� ���������, �� ���������� �� �� ������

	if (countStepsInGals>15){ // ����� ����� �� ��������
	a.curKurs = rand() % 4;
	countStepsInGals = 0;
	}
	

	switch (a.curKurs) //
	{
	case 0:
		tempY = a.Y - a.step; //
		//�������� �����
		if (-2<tempY && scene->itemAt(tempX, tempY) == 0 && scene->itemAt(tempX + 30, tempY) == 0){ //
			//-2<tempY ����������� ���� �� ������� �������� ����
			// sc->itemAt(tempX, tempY) == 0 �� ����������� ����������� ������ ��� (�.�.itemAt(tempX, tempY) �� ������ ��������� �� �� ���� ������ ������ QGraphicsPixmapItem)
			// �����: ���������� ������ �� ������� ������ ���� ��������, ������� ����� ��������� ������� ������ ���� � ������� ������� sc->itemAt(tempX + 48, tempY) == 0
			// ����� ����� ������������ � ����� ����� (��� �������� ������ ���� �������� �� ������� �������� "������" �� �������� ����) ��� �� ������ ������ �����
			a.X = tempX;
			a.Y = tempY;
		}
		break;
	case 1:
		tempY = a.Y + a.step;
		//�������� ����
		if (420>tempY && scene->itemAt(tempX, tempY + 30) == 0 && scene->itemAt(tempX + 30, tempY + 30) == 0){
			a.X = tempX;
			a.Y = tempY;
		}
		break;
	case 2:
		tempX = a.X - a.step;
		//�������� �����
		if (-2<tempX && scene->itemAt(tempX, tempY) == 0 && scene->itemAt(tempX, tempY + 30) == 0){
			a.X = tempX;
			a.Y = tempY;
		}
		break;
	case 3:
		tempX = a.X + a.step;
		//�������� ������
		if (402>tempX && scene->itemAt(tempX + 30, tempY) == 0 && scene->itemAt(tempX + 30, tempY + 30) == 0){
			a.X = tempX;
			a.Y = tempY;
		}
		break;

	}

	countStepsInGals++;

}

void Pacman::drawAttack(){
	
	//QBrush brush1(Qt::NoBrush);
	//QPen ell(Qt::yellow, 1);
	
	//qgEll = scene->addEllipse(hX-9, hY-9, 50, 50, ell, brush1);

	radEll = 50;
	hMana = hMana - 10;
	//qgEll->moveBy(hX-9, hY-9);

	//scene->removeItem(phero);

	//��� ������� ����������� �������� �������� ������ "������" �������
	/*QTimer* ptimer = new QTimer(this); // ������
	connect(ptimer, SIGNAL(timeout()), SLOT(okno())); // ������ ������� timeout ������ �� ������ okno()
	ptimer->start(25); // ������������� �������*/
	/////////////////////////////////////////////////////////////////////////

}

void Pacman::yesFunc(){
	yesIter = 1;
}

bool Pacman::allDead(){ //�� ��������
	bool d=true;
	for (int i = 0; i < countMonstr - 1; i++){
		if (a[i].mHitPoints>0) {
			d=false; 
		}
	}
	return d;

}

Pacman::~Pacman()
{

}



//QPen penDot1(Qt::green, 5);//�����
//scene->addLine(tempX, tempY, tempX, tempY, penDot1);