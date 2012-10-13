#pragma once

#include <QtGui/QAbstractScrollArea>

#include "frame.h"

namespace qReal
{
namespace widgetsEdit
{

class AbstractScrollArea : public Frame
{
	Q_OBJECT

	Q_PROPERTY(Qt::ScrollBarPolicy verticalScrollBarPolicy
		READ verticalScrollBarPolicy
		WRITE setVerticalScrollBarPolicy
		USER true DESIGNABLE true)
	Q_PROPERTY(Qt::ScrollBarPolicy horizontalScrollBarPolicy
		READ horizontalScrollBarPolicy
		WRITE setHorizontalScrollBarPolicy
		USER true DESIGNABLE true)

protected:
	AbstractScrollArea(QAbstractScrollArea *area, ToolController *controller);

private:
	Qt::ScrollBarPolicy verticalScrollBarPolicy() const;
	Qt::ScrollBarPolicy horizontalScrollBarPolicy() const;
	void setVerticalScrollBarPolicy(Qt::ScrollBarPolicy policy);
	void setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy policy);

	QAbstractScrollArea *mAbstractScrollArea;
};

}
}
