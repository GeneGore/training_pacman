#include "Creature.h"
#include <QtGui>
#include <ctime>

Creature::Creature() : mAttack(50), mDefence(50), mHitPoints(50), searchRadius(60), step(1), mType(0)
{

}

//������ ����� � ������ - �����
/*void Attack(){

	if

}*/



//������� ������ �����
bool Creature::isDead(){
	return (mHitPoints == 0); //���������� TRUE, ���� ������� � ������� �������
}


Creature::~Creature()
{
}
