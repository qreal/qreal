#pragma once

#include "control.h"

#include <QWidget>
#include <QLayout>
#include <QBoxLayout>

class LayoutedControl : public QWidget
{
	Q_OBJECT
	public:
		LayoutedControl(Control *control);
		Control* getControl() const;
	private:
		Control* control;
		QLayout* layout;
};
