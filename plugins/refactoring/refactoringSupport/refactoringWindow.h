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
	void activateRestButtons();

signals:
	void findButtonClicked(QString const &refactoringName);

private slots:
	void openPicture(QListWidgetItem *item);
	void findButtonActivate();

private:
	Ui::refactoringForm *mUi;
};

}

