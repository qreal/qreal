#pragma once

#include <QtWidgets/QTextEdit>

namespace deployment {

class ShellWidget : public QTextEdit
{
public:
	explicit ShellWidget(QWidget *parent = 0);
};

}
