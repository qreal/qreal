#pragma once

#include <QtGui/QWidget>

#include "tool.h"

namespace qReal
{
namespace widgetsEdit
{

int const SPACER_DEFAULT_WIDTH_ACTIVE = 60;
int const SPACER_DEFAULT_HEIGHT_ACTIVE = 60;
int const SPACER_DEFAULT_WIDTH_INACTIVE = 20;
int const SPACER_DEFAULT_HEIGHT_INACTIVE = 20;

class Spacer : public Tool
{
public:
	Spacer(Qt::Orientation orientation, ToolController *controller);
	virtual ~Spacer() {}

protected:
	virtual void paint(QPainter *painter
		, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	static QString titleFromOrientation(Qt::Orientation orientation);
	static QString tagFromOrientation(Qt::Orientation orientation);
	static QString iconPathFromOrientation(Qt::Orientation orientation);
	static int widthFromOrientation(Qt::Orientation orientation);
	static int heightFromOrientation(Qt::Orientation orientation);
	static QSizePolicy::Policy hPolicyFromOrientation(Qt::Orientation orientation);
	static QSizePolicy::Policy vPolicyFromOrientation(Qt::Orientation orientation);

	Qt::Orientation mOrientation;
	QWidget *mSpacer;
};

}
}
