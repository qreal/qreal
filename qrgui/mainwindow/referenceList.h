#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidgetItem>
#include <QtCore/QModelIndex>

#include <qrkernel/ids.h>

#include "models/models.h"

namespace Ui
{
class ReferenceList;
}

namespace qReal
{
class MainWindow;
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
			, QString const &refType, QStringList const &currentValue, int role, QWidget *parent = 0);
	~ReferenceList();

public slots:
	void activateElement(QListWidgetItem *);

signals:
	/// emitted when OK pressed
	void referenceSet(QStringList const &newValue, QPersistentModelIndex const &index, int role);

private slots:
	void valueChanged();
	void restoreSelected();

private:
	void loadList(QString const &refType);
	void addItem(qReal::Id const &element);

	void highlightCurrentValue(QStringList const &currentValue);
	QStringList getNewValue() const;

	void initConnections();

	Ui::ReferenceList *mUi;

	QPersistentModelIndex mIndex;
	int mRole;

	qReal::MainWindow *mWindow;
};
