#ifndef CLMSEVENT_H
#define CLMSEVENT_H

#include <QObject>
#include <QEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>

class clMsEvent : public QObject
{
	Q_OBJECT

protected:
	virtual bool eventFilter(QObject*, QEvent*);

public:
	clMsEvent(QObject *pobj=0);
	~clMsEvent();

	

private:
	
};

#endif // CLMSEVENT_H
