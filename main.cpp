/* ��� �������

+ ���������� ����� �� �����

1. ������� ����� �� ���������� �������
2. ������� ������� (���� isMovable ?)

4. �������� ���� - ������ � ����

7. ���������� ��� � �������� ��� ��������
8. ������� ���� �������

*/

#include "pacman.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
	QApplication a(argc, argv);
	Pacman w;
	w.show();

return a.exec();

	/*while (1){
		
		if (w.hHitPoints < 1){
		   QMessageBox::information(0,"���������","�� ��������!");
	       w.close();
		}
		else if (w.allDead()==true){
			QMessageBox::information(0, "���������", "�� ��������!");
			w.close();
		}
	}*/
	

}

