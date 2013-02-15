#include <QtCore/QMetaProperty>

#include "outerBindingsDialog.h"
#include "ui_outerBindongsDialog.h"

using namespace qReal::widgetsEdit;

OuterBindingsDialog::OuterBindingsDialog(qReal::PropertyProxyBase *proxy
		, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::OuterBindongsDialog)
	, mProxy(proxy)
{
	initialize();
}

OuterBindingsDialog::~OuterBindingsDialog()
{
	delete mUi;
}

void OuterBindingsDialog::initialize()
{
	mUi->setupUi(this);
	connect(this, SIGNAL(accepted()), this, SLOT(commit()));
	connect(mUi->addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
	connect(mUi->removeButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));

	initializeProperties();
	initializeView();
}

void OuterBindingsDialog::initializeProperties()
{
	QMetaObject const *metaObject = mProxy->metaObject();
	int const propertyCount = metaObject->propertyCount();
	for (int i = 0; i < propertyCount; ++i) {
		QMetaProperty const metaProperty = metaObject->property(i);
		if (metaProperty.isUser() && metaProperty.isDesignable()) {
			mProperties << metaProperty.name();
		}
	}
}

void OuterBindingsDialog::initializeView()
{
	mUi->tableWidget->horizontalHeader()->resizeSection(0, 130);
	mUi->tableWidget->horizontalHeader()->resizeSection(1, 130);

	QMap<QString, QString> const oldBindings = mProxy->outerBindings();
	foreach (QString const &source, oldBindings.keys()) {
		foreach(QString const &target, oldBindings.values(source)) {
			addItem(source, target);
		}
	}
}

void OuterBindingsDialog::addItem(QString const &source, QString const &target)
{
	QComboBox *sourceEditor = new QComboBox;
	sourceEditor->addItems(mProperties);
	sourceEditor->setCurrentIndex(mProperties.indexOf(source));
	mComboBoxes.append(sourceEditor);

	QTableWidgetItem *targetEditor = new QTableWidgetItem(target);

	int const lastRow = mUi->tableWidget->rowCount();
	mUi->tableWidget->insertRow(lastRow);
	mUi->tableWidget->setCellWidget(lastRow, 0, sourceEditor);
	mUi->tableWidget->setItem(lastRow, 1, targetEditor);
}

void OuterBindingsDialog::addButtonClicked()
{
	addItem(QString(), QString());
}

void OuterBindingsDialog::removeButtonClicked()
{
	int const rowToDelete = mUi->tableWidget->currentRow();
	if (rowToDelete > 0) {
		mComboBoxes.removeAt(rowToDelete);
		mUi->tableWidget->removeRow(rowToDelete);
	}
}

void OuterBindingsDialog::commit()
{
	QMap<QString, QString> result;
	int const rowCount = mUi->tableWidget->rowCount();
	for (int row = 0; row < rowCount; ++row) {
		QComboBox *sourceEditor = mComboBoxes[row];
		QTableWidgetItem *targetEditor = mUi->tableWidget->item(row, 1);
		QString const source = sourceEditor->currentText();
		QString const target = targetEditor->text();
		if (!source.isEmpty() && !target.isEmpty()) {
			result.insertMulti(source, target);
		}
	}

	mProxy->setOuterBindings(result);
}
