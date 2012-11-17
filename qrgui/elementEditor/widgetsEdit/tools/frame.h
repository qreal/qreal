#pragma once

#include <QtGui/QFrame>

#include "layoutTool.h"

namespace qReal
{
namespace widgetsEdit
{

int const FRAME_DEFAULT_WIDTH = 150;
int const FRAME_DEFAULT_HEIGHT = 150;
int const FRAME_MINIMAL_WIDTH = 10;
int const FRAME_MINIMAL_HEIGHT = 10;

class Frame : public LayoutTool
{
	Q_OBJECT

	Q_PROPERTY(QFrame::Shape frameShape READ frameShape WRITE setFrameShape USER true DESIGNABLE true)
	Q_PROPERTY(QFrame::Shadow frameShadow READ frameShadow WRITE setFrameShadow USER true DESIGNABLE true)
	Q_PROPERTY(int lineWidth READ lineWidth WRITE setLineWidth USER true DESIGNABLE true)
	Q_PROPERTY(int midLineWidth READ midLineWidth WRITE setMidLineWidth USER true DESIGNABLE true)

public:
	explicit Frame(ToolController *controller);

protected:
	Frame(QFrame *frame, ToolController *controller);

private:
	void initialize();

	QFrame::Shape frameShape() const;
	QFrame::Shadow frameShadow() const;
	int lineWidth() const;
	int midLineWidth() const;

	void setFrameShape(QFrame::Shape shape);
	void setFrameShadow(QFrame::Shadow shadow);
	void setLineWidth(int width);
	void setMidLineWidth(int width);

	QFrame *mFrame;

};

}
}
