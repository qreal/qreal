#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QComboBox>

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

	void saveClicked();
	void loadClicked();
	void mergeClicked();

	QMap<QString, QString> buildBindingMap() const;

	QDomDocument serializeData(QMap<QString, QString> const &bindings);
	QMap<QString, QString> deserializeData(QDomDocument const &document);

private:
	void initialize();
	void initializeProperties();
	void initializeView();

	QDomDocument askUserDocument();

	void load(QMap<QString, QString> const &bindings);
	void merge(QMap<QString, QString> const &newBindings);

	void addItem(QString const &source, QString const &target);

	Ui::OuterBindongsDialog *mUi;
	PropertyProxyBase *mProxy;
	QList<QString> mProperties;
	QList<QComboBox *> mComboBoxes;
};

}
}
