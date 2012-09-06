#pragma once

#include <QtGui/QAbstractButton>

#include "tool.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class AbstractButton : public Tool
{
	Q_OBJECT

	Q_PROPERTY(bool autoRepeat READ hasAutoRepeat WRITE setAutoRepeat USER true)
	Q_PROPERTY(int autoRepeatDelay READ autoRepeatDelay WRITE setAutoRepeatDelay USER true)
	Q_PROPERTY(int autoRepeatInterval READ autoRepeatInterval WRITE setAutoRepeatInterval USER true)
	Q_PROPERTY(QString text READ text WRITE setText USER true)

protected:
	AbstractButton(QAbstractButton *button, ToolController *controller);

private:
	bool hasAutoRepeat() const;
	int autoRepeatDelay() const;
	int autoRepeatInterval() const;
	QString text() const;

	void setAutoRepeat(bool autoRepeat);
	void setAutoRepeatDelay(int autoRepeatDelay);
	void setAutoRepeatInterval(int autoRepeatInterval);
	void setText(QString const &text);

	QAbstractButton *mAbstractButton;
};

}
}
