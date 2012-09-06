#pragma once

#include <QtGui/QAbstractScrollArea>

#include "frame.h"

namespace qReal
{
namespace widgetsEdit
{

class AbstractScrollArea : public Frame
{
protected:
	AbstractScrollArea(QAbstractScrollArea *area, ToolController *controller);

private:
	QAbstractScrollArea *mAbstractScrollArea;
};

}
}
