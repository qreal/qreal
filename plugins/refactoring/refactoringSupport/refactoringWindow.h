#pragma once

#include <QtGui/QWidget>

namespace Ui {
	class refactoringForm;
}

namespace qReal {

class RefactoringWindow: public QWidget
{
	Q_OBJECT

public:
	explicit RefactoringWindow(QWidget *parent = 0);
	~RefactoringWindow();

private:
	Ui::refactoringForm *mUi;
};

}

