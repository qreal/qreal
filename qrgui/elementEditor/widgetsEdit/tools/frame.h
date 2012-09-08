#pragma once

#include <QtGui/QFrame>

#include "layoutTool.h"

namespace qReal
{
namespace widgetsEdit
{

int const FRAME_DEFAULT_WIDTH = 150;
int const FRAME_DEFAULT_HEIGHT = 150;
int const FRAME_MINIMAL_WIDTH = 50;
int const FRAME_MINIMAL_HEIGHT = 50;

class Frame : public LayoutTool
{
	Q_OBJECT

	Q_PROPERTY(QFrame::Shape shape READ frameShape WRITE setShape USER true)
	Q_PROPERTY(QFrame::Shadow shadow READ frameShadow WRITE setShadow USER true)
	Q_PROPERTY(int lineWidth READ lineWidth WRITE setLineWidth USER true)
	Q_PROPERTY(int midLineWidth READ midLineWidth WRITE setMidLineWidth USER true)

public:
	Frame(ToolController *controller);

protected:
	Frame(QFrame *frame, ToolController *controller);

private:
	void initialize();

	QFrame::Shape frameShape() const;
	QFrame::Shadow frameShadow() const;
	int lineWidth() const;
	int midLineWidth() const;

	void setShape(QFrame::Shape shape);
	void setShadow(QFrame::Shadow shadow);
	void setLineWidth(int width);
	void setMidLineWidth(int width);

	QFrame *mFrame;

};

}
}
