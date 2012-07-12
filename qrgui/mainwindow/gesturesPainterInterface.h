#pragma once
#include <QString>
#include <QList>
#include <QPoint>
//#include <../../qrgui/view/gestures/GeometricForms.h>
#include <C:\Users\Maestro\qreal\qrgui\view\gestures\GeometricForms.h>

class GesturesPainterInterface
{
public:
	virtual QString currentElement() = 0;
	virtual void draw(PathVector const & paths) = 0;
	virtual void setElements(QList<QString> const & elements) = 0;
};
