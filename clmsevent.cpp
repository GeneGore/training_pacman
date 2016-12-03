#include "clmsevent.h"

clMsEvent::clMsEvent(QObject *pobj)
: QObject(pobj)
{


}


/*virtual*/bool clMsEvent::eventFilter(QObject* pobj, QEvent* pe)
	{

	if (pe->type() == QEvent::MouseButtonPress){
 
		QString strClassName = pobj->metaObject()->className();
		QMessageBox::information(0, "Class Name", strClassName);
		
		//pe->GraphicsSceneMousePress
		return true;
	}
	
	/*if (pe->type() == QEvent::MouseButtonPress) {
			if (static_cast<QMouseEvent*>(pe)->button() == Qt::LeftButton) {
				QString strClassName = pobj->metaObject()->className();
				QMessageBox::information(0, "Class Name", strClassName);
				return true;
			}
		}*/
		return false;
	}




clMsEvent::~clMsEvent()
{

}
