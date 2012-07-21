#pragma once

#include <QDialog>
#include <QModelIndex>

#include "../../qrkernel/ids.h"
#include "mainWindow.h"
#include "../models/models.h"

namespace Ui {
class ReferenceList;
}

class ReferenceList : public QDialog
{
	Q_OBJECT

public:
	ReferenceList(qReal::MainWindow *mainWindow, QPersistentModelIndex const &index
			, QString const &refType, QString const &currentValue, int role, QWidget *parent = 0);
	~ReferenceList();

signals:
	void referenceSet(QString const &newValue, QPersistentModelIndex const &index, int role);

private slots:
	void changeValue(QListWidgetItem *);
	void valueChanged();

private:
	void loadList(QString const &refType);
	void addItem(qReal::Id const &element);

	void highlightCurrentValue();

	Ui::ReferenceList *mUi;

	QPersistentModelIndex mIndex;
	QString mValue;
	int mRole;

	qReal::MainWindow *mWindow;
};
