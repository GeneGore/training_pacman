#include <QGraphicsItem>

class Creature
{
public:
	
	bool isDead();
	
	int mAttack; //���� �����
	int mDefence; // ���� ������
	int mHitPoints; // ���������
	int X, Y; // ����������
	int mType; // ��� �������
	int step; //����� ���� � ��������
	int curKurs;// ������� ����
	int searchRadius;

	QGraphicsPixmapItem* monstr; //
	

	Creature();
	~Creature();
};

