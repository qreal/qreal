#pragma once
#include <QtGui/QWidget>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>

namespace qReal {

class ListWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ListWidget(QWidget *parent = 0);
	void addItem(QString const &text, QString const &userData);
	
signals:
	void userDataSelected(QString const &userData);

private slots:
	void okActivate();
	void okButtonHandler();
	void doubleClickHandler(QListWidgetItem *item);

private:
	QListWidget *mListWidget;
	QPushButton *mOkButton;
};

}
