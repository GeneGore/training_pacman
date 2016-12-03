/* ЧТО СДЕЛАТЬ

+ Распечатку доски из файла

1. Перенос героя на координаты курсора
2. Перенос виджета (флаг isMovable ?)

4. редактор карт - запись в файл

7. прозрачный фон и анимация для картинок
8. сделать игру сетевой

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
		   QMessageBox::information(0,"Сообщение","Ты проиграл!");
	       w.close();
		}
		else if (w.allDead()==true){
			QMessageBox::information(0, "Сообщение", "Ты проиграл!");
			w.close();
		}
	}*/
	

}

