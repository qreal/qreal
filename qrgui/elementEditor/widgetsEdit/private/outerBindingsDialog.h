#pragma once

#include <QtGui/QDialog>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QComboBox>

#include "../propertyBinding/propertyProxyBase.h"

namespace Ui
{
class OuterBindongsDialog;
}

namespace qReal
{
namespace widgetsEdit
{

class OuterBindingsDialog : public QDialog
{
	Q_OBJECT

public:
	OuterBindingsDialog(PropertyProxyBase *proxy, QWidget *parent = 0);
	~OuterBindingsDialog();

private slots:
	void addButtonClicked();
	void removeButtonClicked();
	void commit();

private:
	void initialize();
	void initializeProperties();
	void initializeView();

	void addItem(QString const &source, QString const &target);

	Ui::OuterBindongsDialog *mUi;
	PropertyProxyBase *mProxy;
	QList<QString> mProperties;
	QList<QComboBox *> mComboBoxes;
};

}
}
