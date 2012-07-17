#pragma once

#include <QDialog>
#include <QtGui>

class SuggestToCreateDiagramDialog : public QDialog
{
	Q_OBJECT
public:
	SuggestToCreateDiagramDialog(QWidget *parent);
	void addItemToDiagramsList(QString diagramName);
	void diagramsListSetCurrentRow(int num);
signals:
	void diagramsListCurrentRowChanged(int num);
	void diagramsListItemDoubleClicked(QListWidgetItem* item);
	void diagramsListDestroyed();
	void cancelButtonClicked();
	void okButtonClicked();
private:
	QLabel mLabel;
	QListWidget mDiagramsListWidget;
	QPushButton mCancelButton;
	QPushButton mOkButton;
	QVBoxLayout *mVLayout;
	QHBoxLayout *mHLayout;
};

