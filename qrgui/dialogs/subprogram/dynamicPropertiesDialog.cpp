/* Copyright 2015-2016 Kogutich Denis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "dynamicPropertiesDialog.h"
#include "ui_dynamicPropertiesDialog.h"

#include <qrkernel/platformInfo.h>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QMessageBox>
#include <QtXml/QDomDocument>
#include <QtCore/QUuid>
#include <QtCore/QDir>

#include <models/commands/changePropertyCommand.h>

using namespace qReal;
using namespace gui;

const bool hideLabels = false;

DynamicPropertiesDialog::DynamicPropertiesDialog(const qReal::Id &id
		, models::LogicalModelAssistApi &logicalRepoApi
		, models::Exploser &exploser
		, Controller &controller, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::DynamicPropertiesDialog)
	, mShapeWidget(new ShapePropertyWidget(this))
	, mShapeBackgroundWidget(new ShapePropertyWidget(this))
	, mShapeScrollArea(new QScrollArea(this))
	, mShapeBackgroundScrollArea(new QScrollArea(this))
	, mLogicalRepoApi(logicalRepoApi)
	, mExploser(exploser)
	, mController(controller)
	, mId(id)
{
	mUi->setupUi(this);
	setWindowTitle(tr("Properties"));
	mUi->labels->setColumnCount(4);
	mUi->labels->setHorizontalHeaderLabels({tr("Name"), tr("Type"), tr("Value"), ""});
	mUi->labels->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	if (hideLabels) {
		mUi->labels->hide();
		mUi->addLabel->hide();
		mUi->label->hide();
		setFixedHeight(350);
	}

	mShapeScrollArea->setWidget(mShapeWidget);
	mShapeScrollArea->setMaximumHeight(93);
	mUi->verticalLayout->insertWidget(6, mShapeScrollArea);
	mShapeBackgroundScrollArea->setWidget(mShapeBackgroundWidget);
	mShapeBackgroundScrollArea->setMaximumHeight(93);
	mUi->verticalLayout->insertWidget(8, mShapeBackgroundScrollArea);

	init();

	connect(mUi->addLabel, &QPushButton::clicked, this, &DynamicPropertiesDialog::addLabelButtonClicked);
	connect(mUi->saveAll, &QPushButton::clicked, this, &DynamicPropertiesDialog::saveButtonClicked);
	mUi->addLabel->setFocusPolicy(Qt::NoFocus);
	mUi->saveAll->setFocus();
	if (mShapeWidget->selectedShape().isEmpty()) {
		mShapeWidget->selectShape(0);
	}

	if (mShapeBackgroundWidget->selectedShape().isEmpty()) {
		mShapeBackgroundWidget->selectShape(0);
	}
}

DynamicPropertiesDialog::~DynamicPropertiesDialog()
{
	delete mUi;
}

QString DynamicPropertiesDialog::generateShapeXml(const QString &foreground, const QString &background)
{
	QDomDocument shapeDoc;
	QDomElement picture = shapeDoc.createElement("picture");
	picture.setAttribute("sizey", 50);
	picture.setAttribute("sizex", 50);
	if (!background.isEmpty()) {
		QDomElement backgroundSdf = shapeDoc.createElement("image");
		backgroundSdf.setAttribute("name", background);
		backgroundSdf.setAttribute("x1", 0);
		backgroundSdf.setAttribute("y1", 0);
		backgroundSdf.setAttribute("x2", 50);
		backgroundSdf.setAttribute("y2", 50);
		picture.appendChild(backgroundSdf);
	}

	QDomElement foregroundSdf = shapeDoc.createElement("image");
	foregroundSdf.setAttribute("name", foreground);
	foregroundSdf.setAttribute("x1", 0);
	foregroundSdf.setAttribute("y1", 0);
	foregroundSdf.setAttribute("x2", 50);
	foregroundSdf.setAttribute("y2", 50);
	picture.appendChild(foregroundSdf);
	shapeDoc.appendChild(picture);

	return shapeDoc.toString(0);
}

void DynamicPropertiesDialog::addLabelButtonClicked()
{
	QPushButton *button = new QPushButton(tr("Delete"), this);
	const int rowCount = mUi->labels->rowCount();
	mUi->labels->setRowCount(rowCount + 1);
	mUi->labels->setCellWidget(rowCount, 3, button);
	connect(button, &QPushButton::clicked, this, &DynamicPropertiesDialog::deleteButtonClicked);

	QComboBox *types = new QComboBox(this);
	types->addItems({"int", "float", "bool", "string"});
	mUi->labels->setCellWidget(rowCount, 1, types);
	connect(types, &QComboBox::currentTextChanged, this, &DynamicPropertiesDialog::typeChanged);
}

QString DynamicPropertiesDialog::defaultLabelValue(const QString &type) const
{
	if (type == "int") {
		return "0";
	}

	if (type == "float") {
		return "0.0";
	}

	if (type == "string") {
		return "\"\"";
	}

	if (type == "bool") {
		return "true";
	}

	return QString();
}

void DynamicPropertiesDialog::saveButtonClicked()
{
	const QString error = tryToSave();
	if (!error.isEmpty()) {
		QMessageBox::critical(this, tr("Error"), error);
		return;
	}

	const QString localStringPropertyName = mLogicalRepoApi.mutableLogicalRepoApi().stringProperty(mId, "name");
	auto command = new commands::ChangePropertyCommand(
			&mLogicalRepoApi.mutableLogicalRepoApi()
			, "name"
			, mId
			, localStringPropertyName
			, mUi->subprogramName->text()
	);

	const QString localStringPropertyShape = mLogicalRepoApi.mutableLogicalRepoApi().stringProperty(mId, "shape");
	const QString selectedShape = mShapeWidget->selectedShape();
	const QString selectedBackground = mShapeBackgroundWidget->selectedShape();
	command->addPostAction(new commands::ChangePropertyCommand(
			&mLogicalRepoApi.mutableLogicalRepoApi()
			, "shape"
			, mId
			, localStringPropertyShape
			, generateShapeXml(selectedShape, selectedBackground)
	));

	// dirty hack to forward values. It's is not labels, it's just name for restoring values
	QDomDocument dynamicLabels;
	QDomElement labels = dynamicLabels.createElement("labels");

	QMap<QString, QString> previousLabels;
	const QString currentDynamicLabelsString = mLogicalRepoApi.mutableLogicalRepoApi().stringProperty(mId, "labels");
	QDomDocument currentDynamicLabels;
	currentDynamicLabels.setContent(currentDynamicLabelsString);
	for (QDomElement element = currentDynamicLabels.firstChildElement("labels").firstChildElement("label")
			; !element.isNull()
			; element = element.nextSiblingElement("label"))
	{
		const QString key = QString("%1 %2").arg(element.attribute("text")).arg(element.attribute("type"));
		previousLabels[key] = element.attribute("textBinded");
	}

	int x = 40;
	int y = 60;
	for (int i = 0; i < mUi->labels->rowCount(); ++i) {
		const QString name = mUi->labels->item(i, 0)->text();
		const QString type = qobject_cast<QComboBox *>(mUi->labels->cellWidget(i, 1))->currentText();
		const QString value = type == "bool"
				? qobject_cast<QComboBox*>(mUi->labels->cellWidget(i, 2))->currentText()
				: mUi->labels->item(i, 2) ? mUi->labels->item(i, 2)->text() : QString();

		QDomElement label = dynamicLabels.createElement("label");
		label.setAttribute("x", x);
		label.setAttribute("y", y);
		const QString key = QString("%1 %2").arg(name).arg(type);
		label.setAttribute("textBinded", previousLabels.contains(key)
				? previousLabels[key]
				: QUuid::createUuid().toString()
		);

		label.setAttribute("type", type);
		label.setAttribute("value", value.trimmed().isEmpty() ? defaultLabelValue(type) : value);
		label.setAttribute("text", name);
		labels.appendChild(label);

		y += 30;
	}

	const QString localStringPropertyLabels = mLogicalRepoApi.mutableLogicalRepoApi().stringProperty(mId, "labels");
	dynamicLabels.appendChild(labels);
	command->addPostAction(new commands::ChangePropertyCommand(
			&mLogicalRepoApi.mutableLogicalRepoApi()
			, "labels"
			, mId
			, localStringPropertyLabels
			, dynamicLabels.toString(4)
	));

	connect(command, &commands::ChangePropertyCommand::redoComplete, &mExploser, [=](){
		emit mExploser.explosionTargetCouldChangeProperties(mId);
		emit mExploser.explosionsSetCouldChange();
	});

	connect(command, &commands::ChangePropertyCommand::undoComplete, &mExploser, [=](){
		emit mExploser.explosionTargetCouldChangeProperties(mId);
		emit mExploser.explosionsSetCouldChange();
	});

	mController.execute(command);
	close();
}

void DynamicPropertiesDialog::deleteButtonClicked()
{
	for (int i = 0; i < mUi->labels->rowCount(); ++i) {
		if (mUi->labels->cellWidget(i, 3) == sender()) {
			mUi->labels->removeRow(i);
			break;
		}
	}
}

void DynamicPropertiesDialog::typeChanged(const QString &newType)
{
	int row = -1;
	for (int i = 0; i < mUi->labels->rowCount(); ++i) {
		if (mUi->labels->cellWidget(i, 1) == sender()) {
			row = i;
			break;
		}
	}

	if (row < 0) {
		return;
	}

	if (newType == "bool") {
		QComboBox *boolValues = new QComboBox();
		boolValues->addItems({"false", "true"});
		mUi->labels->setCellWidget(row, 2, boolValues);
	} else {
		mUi->labels->removeCellWidget(row, 2);
	}
}

void DynamicPropertiesDialog::init()
{
	const QString filePath = PlatformInfo::invariantSettingsPath("pathToImages") + "/subprogramImages";
	QDir dir(filePath);
	QStringList strList = dir.entryList();
	QStringList shapes;
	const QRegExp png(".png$");
	const QRegExp svg(".svg$");
	for (const QString &str : strList) {
		if (str.contains(png) || str.contains(svg)) {
			shapes << "subprogramImages/" + str;
		}
	}

	dir.setPath(filePath + "/subprogramBackgrounds");
	strList = dir.entryList();
	QStringList shapesBackgrounds;
	for (const QString &str : strList) {
		if (str.contains(png) || str.contains(svg)) {
			shapesBackgrounds << "subprogramImages/subprogramBackgrounds/" + str;
		}
	}

	QDomDocument shape;
	shape.setContent(mLogicalRepoApi.mutableLogicalRepoApi().stringProperty(mId, "shape"));
	QString currentShape;
	QString currentBackground;

	const QDomElement shapeElem = shape.firstChildElement("picture").firstChildElement("image");
	if (shapeElem.nextSiblingElement("image").isNull()) {
		currentShape = shapeElem.attribute("name");
		currentBackground = QString();
	} else {
		currentBackground = shapeElem.attribute("name");
		currentShape = shapeElem.nextSiblingElement("image").attribute("name");
	}

	mShapeWidget->initShapes(shapes, currentShape, false);
	mShapeBackgroundWidget->initShapes(shapesBackgrounds, currentBackground, true);
	mUi->subprogramName->setText(mLogicalRepoApi.mutableLogicalRepoApi().stringProperty(mId, "name"));
	const QString labels = mLogicalRepoApi.mutableLogicalRepoApi().stringProperty(mId, "labels");
	if (labels.isEmpty()) {
		return;
	}

	QDomDocument dynamicLabels;
	dynamicLabels.setContent(labels);
	for (QDomElement element = dynamicLabels.firstChildElement("labels").firstChildElement("label")
			; !element.isNull()
			; element = element.nextSiblingElement("label"))
	{
		const QString type = element.attribute("type");
		const QString value = element.attribute("value");
		const QString text = element.attribute("text");
		addLabel(text, type, value);
	}
}

QString DynamicPropertiesDialog::tryToSave() const
{
	QSet<QString> names;
	const int rowCount = mUi->labels->rowCount();
	for (int i = 0; i < rowCount; ++i) {
		// Return false if "Name" not filled
		if (!mUi->labels->item(i, 0) || mUi->labels->item(i, 0)->text().isEmpty()) {
			return tr("Name is not filled in row %1").arg(i);
		}

		const QString type = qobject_cast<QComboBox*>(mUi->labels->cellWidget(i, 1))->currentText();
		if (type == "int") {
			QString value = mUi->labels->item(i, 2) ? mUi->labels->item(i, 2)->text() : "";
			if (!value.isEmpty()) {
				bool ok;
				value.toInt(&ok);
				// Return false if "int" value isn't int
				if (!ok) {
					return tr("Value in row %1 is not integer").arg(i + 1);
				}
			}
		}

		names << mUi->labels->item(i, 0)->text();
	}

	// Return false if some names are the same
	if (names.count() < rowCount) {
		return tr("Duplicate names");
	}

	return QString();
}

void DynamicPropertiesDialog::addLabel(const QString &name, const QString &type, const QString &value)
{
	QPushButton *button = new QPushButton(tr("Delete"));
	int rowCount = mUi->labels->rowCount();
	mUi->labels->setRowCount(rowCount + 1);
	mUi->labels->setCellWidget(rowCount, 3, button);
	connect(button, &QPushButton::clicked, this, &DynamicPropertiesDialog::deleteButtonClicked);

	QComboBox *types = new QComboBox(this);
	types->addItems({"int", "float", "bool", "string"});
	types->setCurrentText(type);
	mUi->labels->setCellWidget(rowCount, 1, types);
	connect(types, &QComboBox::currentTextChanged, this, &DynamicPropertiesDialog::typeChanged);

	QTableWidgetItem *nameItem = new QTableWidgetItem(name);
	mUi->labels->setItem(rowCount, 0, nameItem);

	if (type == "bool") {
		QComboBox *boolValues = new QComboBox(this);
		boolValues->addItems({"false", "true"});
		boolValues->setCurrentText(value);
		mUi->labels->setCellWidget(rowCount, 2, boolValues);
	} else {
		QTableWidgetItem *valueItem = new QTableWidgetItem(value);
		mUi->labels->setItem(rowCount, 2, valueItem);
	}
}
