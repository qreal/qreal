#pragma once

#include <QtGui/QPlainTextEdit>

#include "abstractScrollArea.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
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
