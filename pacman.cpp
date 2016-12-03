#include "pacman.h"

#include <QtGui>
#include <ctime> //

Pacman::Pacman(QWidget *parent) ///////КОНСТРУКТОР//////////////////////
	: QMainWindow(parent)
{ 
	ui.setupUi(this);
	
	time_t qTime;
	time(&qTime);
	srand((unsigned int)qTime);

	RandomForAll();
	
	for (int i = 0; i < countMonstr - 1; i++){ // начальные координаты монстров заношу в данные объектов класса Creature
		a[i].X=massCoordMonstrX[i];
		a[i].Y=massCoordMonstrY[i];
	}

	hX = massCoordMonstrX[countMonstr - 1]; //начальные координаты героя, сгенеренные и проверенные на другой сцене
	hY = massCoordMonstrY[countMonstr - 1]; // это последние элементы массивов координат, первые отданы монстрам

	okno(); //рисуем окно
	allStuff();
	
	QTimer* ptimer = new QTimer(this); // Таймер
	connect(ptimer, SIGNAL(timeout()), SLOT(okno())); // Сигнал таймера timeout связан со слотом okno()
	ptimer->start(25); // инициализация таймера


	countStepsInGals = 0; //начальное значение счетчика шагов на участке прямолинейного движения
	angle = 0;


	radEll = 0; //начальный радиус поражения
	deltaEll = 5;
	yesIter = 0;

	/*QColor col(Qt::green);
	QBrush brush1(col, Qt::NoBrush);
	QPen ell(Qt::green, 1);
	qgEll = scene->addEllipse(-50, -50, 50, 50, ell, brush1);*/

	/////////////////Задание типа монстра и его параметров//////
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
	//Задание параметров героя

	hDefence = 100;
	hAttack = 100;
	hHitPoints=500;
	hMana = 1000;
	/*a[countMonstr].mType = 3;
	a[countMonstr].mHitPoints = 100;
	a[countMonstr].mAttack = 100;
	//a[countMonstr].mDefence = 100; //ПОЧЕМУ ОШИБКА??? какая-то беда с памятью*/


	QObject::connect(ui.mahButton1, SIGNAL(clicked()), this, SLOT(yesFunc()));

	


} /////////////////// КОНЕЦ КОНСТРУКТОРА//////////////////////////

void Pacman::allStuff(){

	ui.graphicsView->installEventFilter(new clMsEvent(ui.graphicsView)); /// ФИЛЬТР СОБЫТИЙ
	

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
	//ui.mainToolBar->addAction(QPixmap(image_xpm), "Иконка xpm", this, SLOT(slotNoImp()));
	ui.mainToolBar->addAction(QPixmap("Resources/knop.bmp"), "GOD MODE", this, SLOT()); //
	ui.mainToolBar->addSeparator();
	ui.mainToolBar->addAction(QPixmap("Resources/knop2.bmp"), "GOD MODE", this, SLOT()); //

	//строка состояния
	QLabel* l1 = new QLabel("Состояние: ");
	statusBar()->addWidget(l1);
	QLabel* l2 = new QLabel("все путем");
	statusBar()->addWidget(l2);

	QPixmap pix1("Resources/ma1.bmp");
	ui.mahButton1->setIcon(pix1);

	QPixmap pix2("Resources/ma2.bmp");
	ui.mahButton2->setIcon(pix2);


}



void Pacman::okno(){ // это окно я рисую с частотой, с которой таймер выдает сигнал timeout. смотри конструктор
	

	scene = new QGraphicsScene(ui.graphicsView); // создаю сцену и привязываю ее к представлению ui.graphicsView

	scene->setBackgroundBrush(Qt::black); // устанавливаю
	scene->setSceneRect(0,0,445,445); // устанавливаю размер сцены

	//scene->installEventFilter(this); /////////////////////////

	buildWall(); // Строим стены
	setMonsters(); //Рисуем на свободном месте монстров и героя

	//двигаем монстров
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
	//это скорее защита: невидимость, силовое поле и т.д.
	//класс Magic: длительность воздействия...

	//QBrush brush1(Qt::NoBrush);
	//QBrush brush1(Qt::Dense5Pattern);

	/*QColor col(Qt::black);
	QBrush brush1(col, Qt::Dense3Pattern); //вот удачное решение для невидимости
	QPen ell(Qt::transparent, 1);
	scene->addEllipse(hX-7, hY-7, 45, 45, ell, brush1);
	QPen pen(Qt::red, 10);
	scene->addLine(aa, bb, aa + 30, bb + 30, pen);*/

	ui.graphicsView->setScene(scene);//установить сцену!

}


//////////////////////////////Ловля нажатия клавиш
void Pacman::keyPressEvent(QKeyEvent* pe){

	int tempX=hX; // временные переменные для хранения координат героя
	int tempY=hY; // нужны, чтобы можно было проверить, не столкнется ли герой со стеной
	
	switch (pe->key()) //функция key выдаст значение нажатой клавиши
	{
	case Qt::Key_W:
		tempY = hY - 10; // цифра - это величина шага героя
		//Движение вверх W
		if (-2<tempY && scene->itemAt(tempX, tempY) == 0 && scene->itemAt(tempX + heroWidth, tempY) == 0){ //
			//-2<tempY ограничение хода героя по верхней границей окна
			// scene->itemAt(tempX, tempY) == 0 по посчитанным координатам никого нет (т.е.itemAt(tempX, tempY) не вернет указатель ни на один объект класса QGraphicsPixmapItem)
			// важно: координаты даются по вернему левому углу картинки, поэтому нужно проверить верхний правый угол с помощью условия scene->itemAt(tempX + 48, tempY) == 0
			// иначе герой будет пересекаться с самим собой (при движении вправо идет смещение от правого верхнего "внутрь" героя на величину шага) или со стеной правым краем
			hX = tempX;
			hY = tempY;
		}
		break;
	case Qt::Key_S:
		tempY = hY + 10;
		//Движение вниз S
		if (420>tempY && scene->itemAt(tempX, tempY + heroHeight) == 0 && scene->itemAt(tempX + heroWidth, tempY + heroHeight) == 0){
			hX = tempX;
			hY = tempY;
		}
		break;
	case Qt::Key_A:
		tempX = hX - 10;
		//Движение влево A
		if (-2<tempX && scene->itemAt(tempX, tempY) == 0 && scene->itemAt(tempX, tempY + heroHeight) == 0){
			hX = tempX;
			hY = tempY;
		}
		break;
	case Qt::Key_D:
		tempX = hX + 10;
		//Движение вправо D
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


void Pacman::RandomForAll(){//Эта функция задаст все возможные рандомные значения в начале программы на весь игровой цикл, проверив их на другой сцене

	virtscene = new QGraphicsScene; //  Делаю сцену, на которой все прикину
	virtscene->setSceneRect(0, 0, 445, 445);//

	QPixmap virtbrick("Resources/virtbrick.bmp");//
	//Координаты стен
	for (int i = 0; i < countWall; i++){//генерю координаты стен и добавляю на вирт сцену для проверки, не наложится ли,  и если все путем - записываю в массив
		//НАПИШИ ФУНКЦИЮ, два куска одинакого текста же для стен и монстров
		
		bool a = false;
		pvirtbrick[i] = virtscene->addPixmap(virtbrick);
		while (a != true){
			int X = 48 * (rand() % 9);
			int Y = 45 * (rand() % 9);
			if (thisPositionIsEmpty(virtscene, X, Y) == true){ //Смотрим, нет ли на этом месте элемента 
				pvirtbrick[i]->moveBy(X, Y);
				massCoordWallX[i] = X;
				massCoordWallY[i] = Y;
				a = true;
			}
		}
	}

	QPixmap virtstar("Resources/virtstar.bmp");
	//Начальные координаты монстров
	for (int i = 0; i < countMonstr; i++){
		bool a = false;
		pvirtstar[i] = virtscene->addPixmap(virtstar);
		while (a != true){
			int X = 48 * (rand() % 9);
			int Y = 45 * (rand() % 9);
			if (thisPositionIsEmpty(virtscene, X, Y) == true){ //Смотрим, нет ли на этом месте элемента 
				pvirtstar[i]->moveBy(X, Y);
				massCoordMonstrX[i]=X;
				massCoordMonstrY[i]=Y;
				a = true;
			}
		}
	}
}

void Pacman::buildWall(){ //СТРОИМ СТЕНЫ

	QPixmap brick("Resources/brick.bmp");
	
	for (int i = 0; i < countWall; i++){
		pbrick[i] = scene->addPixmap(brick);
		
		//Работает
		/*QFont font("Times", 8, QFont::Bold);
		QGraphicsSimpleTextItem* stext;
		stext = scene->addSimpleText(QString::number(i), font);
		stext->moveBy(massCoordWallX[i]+15, massCoordWallY[i]+15);*/ //Пишет на блоке его номер


		pbrick[i]->moveBy(massCoordWallX[i], massCoordWallY[i]);
		
		//Работает!
		//QPen penDot1(Qt::green, 3);
		//scene->addLine(massCoordWallX[i], massCoordWallY[i], massCoordWallX[i], massCoordWallY[i], penDot1); //рисует точку на верхнем левом углу

	}

}

void Pacman::setMonsters(){ // Установить монстров и героя по проверенным координатам

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
	QPixmap hero("Resources/hero.bmp"); //создаю объект пиксмап, содержащий рисунок героя
	
	phero = scene->addPixmap(hero);

	for (int i = 0; i < countMonstr - 1; i++){
	    switch (a[i].mType){
		      case 0: a[i].monstr = scene->addPixmap(star1); break;
		      case 1: a[i].monstr = scene->addPixmap(star2); break;
		      case 2: a[i].monstr = scene->addPixmap(star3); break;
	    }

		text = scene->addText(QString::number(a[i].mHitPoints), font); //Устанавливаем в сцену метки хитпойнтов
		text->setDefaultTextColor(clr); // добавляем цвет
		text->moveBy(a[i].X, a[i].Y); // помещаем по координатам монстра

        if (a[i].mHitPoints>0){
		   a[i].monstr->moveBy(a[i].X, a[i].Y); // если монстр здоров, двигаем его по координатам
		}
		else {
			scene->removeItem(a[i].monstr);//если помер, удаляем его со сцены
			deadmans++;
			scene->removeItem(text);        // и его меточку хитпойнтов тоже
		}
		
		if (a[i].monstr->collidesWithItem(qgEll)&&yesIter!=0){ //при пересечении эллипса смерти и монстра минусуем ему пойнты
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
	QPen penRay(Qt::yellow, 1);//луч

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
			//передать угол в функцию движения или так:
			if (scene->itemAt(futX, futY) != *pbrick){ //только левый верхний отрабытывает

				m.X = m.X + m.step * qCos(angle*pi / 180);
				m.Y = m.Y + m.step *qSin(angle*pi / 180);
			}
			//if (scene->itemAt(futX, futY) == phero){ //только левый верхний отрабытывает + плодит "призраков" на карте
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

	int tempX = a.X; // временные переменные для хранения координат
	int tempY = a.Y; // нужны, чтобы можно было проверить, не столкнется ли со стеной

	if (countStepsInGals>15){ // число шагов до поворота
	a.curKurs = rand() % 4;
	countStepsInGals = 0;
	}
	

	switch (a.curKurs) //
	{
	case 0:
		tempY = a.Y - a.step; //
		//Движение вверх
		if (-2<tempY && scene->itemAt(tempX, tempY) == 0 && scene->itemAt(tempX + 30, tempY) == 0){ //
			//-2<tempY ограничение хода по верхней границей окна
			// sc->itemAt(tempX, tempY) == 0 по посчитанным координатам никого нет (т.е.itemAt(tempX, tempY) не вернет указатель ни на один объект класса QGraphicsPixmapItem)
			// важно: координаты даются по вернему левому углу картинки, поэтому нужно проверить верхний правый угол с помощью условия sc->itemAt(tempX + 48, tempY) == 0
			// иначе будет пересекаться с самим собой (при движении вправо идет смещение от правого верхнего "внутрь" на величину шага) или со стеной правым краем
			a.X = tempX;
			a.Y = tempY;
		}
		break;
	case 1:
		tempY = a.Y + a.step;
		//Движение вниз
		if (420>tempY && scene->itemAt(tempX, tempY + 30) == 0 && scene->itemAt(tempX + 30, tempY + 30) == 0){
			a.X = tempX;
			a.Y = tempY;
		}
		break;
	case 2:
		tempX = a.X - a.step;
		//Движение влево
		if (-2<tempX && scene->itemAt(tempX, tempY) == 0 && scene->itemAt(tempX, tempY + 30) == 0){
			a.X = tempX;
			a.Y = tempY;
		}
		break;
	case 3:
		tempX = a.X + a.step;
		//Движение вправо
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

	//ПРИ НАЖАТИИ УВЕЛИЧИВАЕТ СКОРОСТЬ запускаю таймер "внутри" таймера
	/*QTimer* ptimer = new QTimer(this); // Таймер
	connect(ptimer, SIGNAL(timeout()), SLOT(okno())); // Сигнал таймера timeout связан со слотом okno()
	ptimer->start(25); // инициализация таймера*/
	/////////////////////////////////////////////////////////////////////////

}

void Pacman::yesFunc(){
	yesIter = 1;
}

bool Pacman::allDead(){ //не работает
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



//QPen penDot1(Qt::green, 5);//точка
//scene->addLine(tempX, tempY, tempX, tempY, penDot1);