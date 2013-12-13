#include <QtCore/QMetaProperty>
#include <QtWidgets/QFileDialog>

#include "outerBindingsDialog.h"
#include "ui_outerBindongsDialog.h"

#include <qrutils/outFile.h>
#include <qrutils/inFile.h>

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
	connect(mUi->saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
	connect(mUi->loadButton, SIGNAL(clicked()), this, SLOT(loadClicked()));
	connect(mUi->mergeButton, SIGNAL(clicked()), this, SLOT(mergeClicked()));

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

	load(mProxy->outerBindings());
}

void OuterBindingsDialog::load(QMap<QString, QString> const &bindings)
{
	mUi->tableWidget->clearContents();
	mUi->tableWidget->setRowCount(0);
	mComboBoxes.clear();
	foreach (QString const &source, bindings.keys()) {
		foreach(QString const &target, bindings.values(source)) {
			addItem(source, target);
		}
	}
}

void OuterBindingsDialog::merge(QMap<QString, QString> const &newBindings)
{
	QMap<QString, QString> const currentBindings = buildBindingMap();
	foreach (QString const &source, newBindings.keys()) {
		if (!mProperties.contains(source)) {
			// TODO: report about it
			continue;
		}
		foreach(QString const &target, newBindings.values(source)) {
			if (currentBindings.contains(source) && currentBindings[source] == target) {
				// Do not want to duplicate bindings
				continue;
			}
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
	if (rowToDelete >= 0) {
		mComboBoxes.removeAt(rowToDelete);
		mUi->tableWidget->removeRow(rowToDelete);
	}
}

void OuterBindingsDialog::commit()
{
	mProxy->setOuterBindings(buildBindingMap());
}

QMap<QString, QString> OuterBindingsDialog::buildBindingMap() const
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

	return result;
}

void OuterBindingsDialog::saveClicked()
{
	QString path = QFileDialog::getSaveFileName(this
			, tr("Select save destiation"), QString()
			, tr("QReal outer binding files") + "(*.obf);;" + tr("All Files") + "(*.*)");
	if (path.isEmpty()) {
		return;
	}
	if (!path.endsWith(".obf", Qt::CaseInsensitive)) {
		path.append(".obf");
	}

	QString const xml = serializeData(buildBindingMap()).toString(4);
	utils::OutFile file(path);
	file() << xml;
}

QDomDocument OuterBindingsDialog::askUserDocument()
{
	QString const path = QFileDialog::getOpenFileName(this
			, tr("Open"), QString()
			, tr("QReal outer binding files") + "(*.obf);;" + tr("All Files") + "(*.*)");
	if (path.isEmpty()) {
		return QDomDocument();
	}

	QDomDocument document;
	document.setContent(utils::InFile::readAll(path));
	return document;
}

void OuterBindingsDialog::loadClicked()
{
	QDomDocument const document = askUserDocument();
	if (!document.isNull()) {
		load(deserializeData(document));
	}
}

void OuterBindingsDialog::mergeClicked()
{
	QDomDocument const document = askUserDocument();
	if (!document.isNull()) {
		merge(deserializeData(document));
	}
}

QDomDocument OuterBindingsDialog::serializeData(QMap<QString, QString> const &bindings)
{
	QDomDocument result;
	QDomElement root = result.createElement("outerBindings");
	foreach (QString const &source, bindings.keys()) {
		QDomElement sourceElement = result.createElement("source");
		sourceElement.setAttribute("name", source);
		foreach (QString const &target, bindings.values(source)) {
			QDomElement targetElement = result.createElement("target");
			targetElement.setAttribute("name", target);
			sourceElement.appendChild(targetElement);
		}
		root.appendChild(sourceElement);
	}
	result.appendChild(root);
	return result;
}

QMap<QString, QString> OuterBindingsDialog::deserializeData(QDomDocument const &document)
{
	QMap<QString, QString> result;
	QDomElement const root =  document.documentElement();
	QDomElement sourceElement = root.firstChildElement("source");
	while (!sourceElement.isNull()) {
		QString const source = sourceElement.attribute("name");
		QDomElement targetElement = sourceElement.firstChildElement("target");
		while (!targetElement.isNull()) {
			QString const target = targetElement.attribute("name");
			result.insertMulti(source, target);
			targetElement = targetElement.nextSiblingElement("target");
		}
		sourceElement = sourceElement.nextSiblingElement("source");
	}
	return result;
}
