#pragma once

#include <QtGui/QLineEdit>

#include "tool.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class LineEdit : public Tool
{
	Q_OBJECT

public:
	LineEdit(ToolController *controller);

private slots:
	void textChanged(QString const &text);

private:
	QLineEdit *mLineEdit;

};

}
}
