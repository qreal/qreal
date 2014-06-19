#pragma once

#include <QtWidgets/QAbstractButton>
#include <qrgui/elementEditor/widgetsEdit/private/toolController.h>

#include "tool.h"

namespace qReal
{
namespace widgetsEdit
{

class AbstractButtonProxy : public ToolProxy
{
	Q_OBJECT

	Q_PROPERTY(bool autoRepeat READ hasAutoRepeat WRITE setAutoRepeat USER true DESIGNABLE true)
	Q_PROPERTY(int autoRepeatDelay READ autoRepeatDelay WRITE setAutoRepeatDelay USER true DESIGNABLE true)
	Q_PROPERTY(int autoRepeatInterval READ autoRepeatInterval WRITE setAutoRepeatInterval USER true DESIGNABLE true)
	Q_PROPERTY(QString text READ text WRITE setText USER true DESIGNABLE true)

protected:
	explicit AbstractButtonProxy(QAbstractButton *button);

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

class AbstractButton : public Tool
{
	Q_OBJECT

protected:
	AbstractButton(QAbstractButton *button, ToolController *controller);

private:
	QAbstractButton *mAbstractButton;
};

}
}
