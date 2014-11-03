#pragma once

#include <QtWidgets/QListWidget>

namespace qReal {

/// @brief List with button for confirm choice. The selected data is sent through a signal.
/// List in which you can add element together with associated with it the string data.
/// Selecting an item by either double clicking or by using the supporting buttons.
/// The selected string data sent through the signal userDataSelected(QString const &userData).
class ListWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ListWidget(QWidget *parent = 0);

	/// @param text Item label
	/// @param userData Associated with item the string data
	/// @param toolTip Item tooltip
	void addItem(QString const &text, QString const &userData, QString const &toolTip = "");
	void highlightFirstItem();

signals:
	void userDataSelected(QString const &userData);

private slots:
	void doubleClickHandler(QListWidgetItem *item);

private:
	QString userData(QListWidgetItem *item);

protected:
	QListWidget *mListWidget;
};

}
