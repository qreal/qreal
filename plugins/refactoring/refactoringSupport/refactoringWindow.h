#pragma once

#include <QtGui/QDialog>
#include <QtGui/QListWidgetItem>

namespace Ui {
	class refactoringForm;
}

namespace qReal {

/// Window to work with available refactorings. Allows you to choose refactoring,
/// find all its occurrences and apply it
class RefactoringWindow: public QDialog
{
	Q_OBJECT

public:
	explicit RefactoringWindow(QWidget *parent = 0);
	~RefactoringWindow();
	void updateRefactorings(QString const &dirPath);
	void activateRestButtons();
	void discard();

signals:
	void findButtonClicked(QString const &refactoringName);
	void findNextButtonClicked();
	void discardButtonClicked();
	void applyButtonClicked();

private slots:
	void openPicture(QListWidgetItem *item);
	void findButtonActivate();
	void findNextButtonActivate();
	void discardButtonActivate();
	void applyButtonActivate();

private:
	Ui::refactoringForm *mUi;
};

}

