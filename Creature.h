#include <QGraphicsItem>

class Creature
{
public:
	
	bool isDead();
	
	int mAttack; //очки атаки
	int mDefence; // очки защиты
	int mHitPoints; // хитпойнты
	int X, Y; // координаты
	int mType; // тип монстра
	int step; //длина шага в пикселях
	int curKurs;// текущий курс
	int searchRadius;

	QGraphicsPixmapItem* monstr; //
	

	Creature();
	~Creature();
};

