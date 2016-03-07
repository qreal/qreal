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

#include <qrkernel/settingsManager.h>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QMessageBox>
#include <QtXml/QDomDocument>
#include <QtCore/QUuid>
#include <QtCore/QDir>

using namespace qReal;
using namespace gui;

DynamicPropertiesDialog::DynamicPropertiesDialog(const qReal::Id &id
		, qrRepo::LogicalRepoApi &logicalRepoApi
		, models::Exploser &exploser
		, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::DynamicPropertiesDialog)
	, mShapeWidget(new ShapePropertyWidget(this))
	, mScrollArea(new QScrollArea(this))
	, mLogicalRepoApi(logicalRepoApi)
	, mExploser(exploser)
	, mId(id)
{
	mUi->setupUi(this);
	setWindowTitle(tr("Properties"));
	mUi->labels->setColumnCount(4);
	mUi->labels->setHorizontalHeaderLabels(QStringList() << "Name" << "Type" << "Value" << "");
	mUi->labels->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	mScrollArea->setWidget(mShapeWidget);
	mScrollArea->setMaximumHeight(93);
	mUi->verticalLayout->insertWidget(6, mScrollArea);

	init();

	connect(mUi->addLabel, &QPushButton::clicked, this, &DynamicPropertiesDialog::addLabelButtonClicked);
	connect(mUi->saveAll, &QPushButton::clicked, this, &DynamicPropertiesDialog::saveButtonClicked);
}

DynamicPropertiesDialog::~DynamicPropertiesDialog()
{
	delete mUi;
}

QString DynamicPropertiesDialog::generateShapeXml(const QString &shape1, const QString &shape2)
{
	QDomDocument shape;
	QDomElement picture = shape.createElement("picture");
	picture.setAttribute("sizey", 50);
	picture.setAttribute("sizex", 50);
	QDomElement image1 = shape.createElement("image");
	image1.setAttribute("name", shape1);
	image1.setAttribute("x1", 0);
	image1.setAttribute("y1", 0);
	image1.setAttribute("x2", 50);
	image1.setAttribute("y2", 50);
	picture.appendChild(image1);
	if (!shape2.isEmpty()) {
		QDomElement image2 = shape.createElement("image");
		image2.setAttribute("name", shape2);
		image2.setAttribute("x1", 0);
		image2.setAttribute("y1", 0);
		image2.setAttribute("x2", 50);
		image2.setAttribute("y2", 50);
		picture.appendChild(image2);
	}
	shape.appendChild(picture);

	return shape.toString(0);
}

void DynamicPropertiesDialog::addLabelButtonClicked()
{
	QPushButton *button = new QPushButton("Delete");
	int rowCount = mUi->labels->rowCount();
	mUi->labels->setRowCount(rowCount + 1);
	mUi->labels->setCellWidget(rowCount, 3, button);
	connect(button, &QPushButton::clicked, this, &DynamicPropertiesDialog::deleteButtonClicked);

	QComboBox *types = new QComboBox();
	types->addItems(QStringList() << "int" << "bool" << "string");
	mUi->labels->setCellWidget(rowCount, 1, types);
	connect(types, &QComboBox::currentTextChanged, this, &DynamicPropertiesDialog::typeChanged);
}

void DynamicPropertiesDialog::saveButtonClicked()
{
	if (!canSave()) {
		QMessageBox::critical(this, tr("Error"), tr("Save error, maybe you forgot fill Name"
		", Or int value isn't int, Or some names are the same"));
		return;
	}

	mLogicalRepoApi.setProperty(mId, "name", mUi->subprogramName->text());
	const QString selectedShape = generateShapeXml(mShapeWidget->getSelectedShape());
	mLogicalRepoApi.setProperty(mId, "shape", selectedShape);
	QDomDocument dynamicLabels;
	QDomElement labels = dynamicLabels.createElement("labels");

	int x = 40;
	int y = 60;
	for (int i = 0; i < mUi->labels->rowCount(); ++i) {
		QString name = mUi->labels->item(i, 0)->text();
		QString type = qobject_cast<QComboBox*>(mUi->labels->cellWidget(i, 1))->currentText();
		QString value;
		if (type == "bool") {
			value = qobject_cast<QComboBox*>(mUi->labels->cellWidget(i, 2))->currentText();
		} else {
			value = mUi->labels->item(i, 2) ? mUi->labels->item(i, 2)->text() : "";
		}

		QDomElement label1 = dynamicLabels.createElement("label");
		label1.setAttribute("x", x);
		label1.setAttribute("y", y);
		label1.setAttribute("textBinded", QUuid::createUuid().toString());
		label1.setAttribute("type", type);
		label1.setAttribute("value", value);
		labels.appendChild(label1);

		QDomElement label2 = dynamicLabels.createElement("label");
		label2.setAttribute("x", x - 70);
		label2.setAttribute("y", y);
		label2.setAttribute("text", name);
		labels.appendChild(label2);

		y += 30;
	}

	dynamicLabels.appendChild(labels);
	mLogicalRepoApi.setProperty(mId, "labels", dynamicLabels.toString(4));
	mExploser.explosionTargetCouldChangeProperties(mId);
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
	int row;
	for (int i = 0; i < mUi->labels->rowCount(); ++i) {
		if (mUi->labels->cellWidget(i, 1) == sender()) {
			row = i;
			break;
		}
	}

	if (newType == "bool") {
		QComboBox *boolValues = new QComboBox();
		boolValues->addItems(QStringList() << "false" << "true");
		mUi->labels->setCellWidget(row, 2, boolValues);
	} else {
		mUi->labels->removeCellWidget(row, 2);
	}
}

void DynamicPropertiesDialog::init()
{
	const QString filePath = SettingsManager::value("pathToImages").toString() + "/subprogramImages";
	QDir dir(filePath);
	const QStringList strList = dir.entryList();
	QStringList shapes;
	QRegExp png(".png$");
	QRegExp svg(".svg$");
	for (const QString &str : strList) {
		if (str.contains(png) || str.contains(svg)) {
			shapes << "subprogramImages/" + str;
		}
	}

	QDomDocument shape;
	shape.setContent(mLogicalRepoApi.stringProperty(mId, "shape"));
	const QString currentShape = shape.firstChildElement("picture").firstChildElement("image").attribute("name");
	mShapeWidget->initShapes(shapes, currentShape);
	mUi->subprogramName->setText(mLogicalRepoApi.stringProperty(mId, "name"));
	const QString labels = mLogicalRepoApi.stringProperty(mId, "labels");
	if (labels.isEmpty()) {
		return;
	}

	QDomDocument dynamicLabels;
	dynamicLabels.setContent(labels);
	for (QDomElement element
			= dynamicLabels.firstChildElement("labels").firstChildElement("label");
			!element.isNull();
			element = element.nextSiblingElement("label"))
	{
		const QString type = element.attribute("type");
		const QString value = element.attribute("value");
		element = element.nextSiblingElement("label");
		const QString text = element.attribute("text");
		addLabel(text, type, value);
	}
}

bool DynamicPropertiesDialog::canSave()
{
	QSet<QString> names;
	const int rowCount = mUi->labels->rowCount();
	for (int i = 0; i < rowCount; ++i) {
		// return false if "Name" not filled
		if (!mUi->labels->item(i, 0) || mUi->labels->item(i, 0)->text().isEmpty()) {
			return false;
		}

		const QString type = qobject_cast<QComboBox*>(mUi->labels->cellWidget(i, 1))->currentText();
		if (type == "int") {
			QString value = mUi->labels->item(i, 2) ? mUi->labels->item(i, 2)->text() : "";
			if (!value.isEmpty()) {
				bool ok;
				value.toInt(&ok);
				// return false if "int" value isn't int
				if (!ok) {
					return false;
				}
			}
		}

		names << mUi->labels->item(i, 0)->text();
	}

	// return false if some names are the same
	return names.count() == rowCount;
}

void DynamicPropertiesDialog::addLabel(const QString &name, const QString &type, const QString &value)
{
	QPushButton *button = new QPushButton("Delete");
	int rowCount = mUi->labels->rowCount();
	mUi->labels->setRowCount(rowCount + 1);
	mUi->labels->setCellWidget(rowCount, 3, button);
	connect(button, &QPushButton::clicked, this, &DynamicPropertiesDialog::deleteButtonClicked);

	QComboBox *types = new QComboBox();
	types->addItems(QStringList() << "int" << "bool" << "string");
	types->setCurrentText(type);
	mUi->labels->setCellWidget(rowCount, 1, types);
	connect(types, &QComboBox::currentTextChanged, this, &DynamicPropertiesDialog::typeChanged);

	QTableWidgetItem *nameItem = new QTableWidgetItem(name);
	mUi->labels->setItem(rowCount, 0, nameItem);

	if (type == "bool") {
		QComboBox *boolValues = new QComboBox();
		boolValues->addItems(QStringList() << "false" << "true");
		boolValues->setCurrentText(value);
		mUi->labels->setCellWidget(rowCount, 2, boolValues);
	} else {
		QTableWidgetItem *valueItem = new QTableWidgetItem(value);
		mUi->labels->setItem(rowCount, 2, valueItem);
	}
}
