#pragma once

#include <QDialog>
#include <QModelIndex>

#include "../../qrkernel/ids.h"
#include "mainWindow.h"
#include "../models/models.h"

namespace Ui {
class ReferenceList;
}

/** @brief ReferenceList is a widget which allows to edit reference properties
  *
  * Shows all the logical elements of needed type and sets reference property to chosen value
  */
class ReferenceList : public QDialog
{
	Q_OBJECT

public:
	ReferenceList(qReal::MainWindow *mainWindow, QPersistentModelIndex const &index
			, QString const &refType, QString const &currentValue, int role, QWidget *parent = 0);
	~ReferenceList();

public slots:
	void activateElement(QListWidgetItem *);

signals:
	/// emitted when OK pressed
	void referenceSet(QString const &newValue, QPersistentModelIndex const &index, int role);

private slots:
	void valueChanged();
	void restoreSelected();

private:
	void loadList(QString const &refType);
	void addItem(qReal::Id const &element);

	void highlightCurrentValue(QString const &currentValue);
	QString getNewValue();

	void initConnections();

	Ui::ReferenceList *mUi;

	QPersistentModelIndex mIndex;
	int mRole;

	qReal::MainWindow *mWindow;
};
