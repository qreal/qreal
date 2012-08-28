#pragma once

#include <QtGui/QPlainTextEdit>

#include "abstractScrollArea.h"
#include "../private/toolController.h"

namespace Ui
{
namespace WidgetsEdit
{

class PlainTextEdit : public AbstractScrollArea
{
	Q_OBJECT

public:
	PlainTextEdit(ToolController *controller);

private slots:
	void textChanged();

private:
	QPlainTextEdit *mPlainTextEdit;

};

}
}
