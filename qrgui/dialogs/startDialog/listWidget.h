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
	void addItem(QString const &text, QString const &userData, const QString &toolTip = "");
	
signals:
	void userDataSelected(QString const &userData);

private slots:
	void okActivate();
	void okButtonHandler();
	void doubleClickHandler(QListWidgetItem *item);

private:
	QString userData(QListWidgetItem *item);

	QListWidget *mListWidget;
	QPushButton *mOkButton;
};

}
