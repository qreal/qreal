#pragma once

#include <QtGui/QPushButton>

#include "abstractButton.h"
#include "../private/toolController.h"

namespace qReal
{
namespace widgetsEdit
{

class PushButton : public AbstractButton
{
	Q_PROPERTY(bool flat READ isFlat WRITE setFlat USER true)

public:
	PushButton(ToolController *controller);

private:
	bool isFlat() const;
	void setFlat(bool flat);

	QPushButton *mPushButton;
};

}
}
