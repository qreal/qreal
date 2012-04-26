#pragma once

#include <QtGui/QWidget>
#include <QtGui/QListWidgetItem>

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
	void updateRefactorings(QString const &dirPath);

private slots:
	void openPicture(QListWidgetItem *item);

private:
	Ui::refactoringForm *mUi;
};

}

