#pragma once

#include "embeddedControl.h"

#include <QWidget>
#include <QLayout>
#include <QBoxLayout>

class LayoutedControl : public QWidget
{
	Q_OBJECT
	public:
		LayoutedControl(EmbeddedControl *control);
		EmbeddedControl* getControl() const;
	private:
		EmbeddedControl* control;
		QLayout* layout;
};
