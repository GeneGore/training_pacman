#include "Creature.h"
#include <QtGui>
#include <ctime>

Creature::Creature() : mAttack(50), mDefence(50), mHitPoints(50), searchRadius(60), step(1), mType(0)
{

}

//Монстр рядом с героем - Атака
/*void Attack(){

	if

}*/



//Функция монстр мертв
bool Creature::isDead(){
	return (mHitPoints == 0); //возвращает TRUE, если условие в скобках истинно
}


Creature::~Creature()
{
}
