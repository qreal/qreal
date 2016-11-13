/* Copyright 2007-2015 QReal Research Group
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

#include "propertyEditorView.h"

#include <QtWidgets/QApplication>

#include <qrkernel/platformInfo.h>
#include <qrutils/qRealFileDialog.h>

#include "models/commands/changePropertyCommand.h"

using namespace qReal;
using namespace qReal::gui::editor;

PropertyEditorView::PropertyEditorView(QWidget *parent)
		: QWidget(parent)
		, mChangingPropertyValue(false)
		, mModel(nullptr)
		, mPropertyEditor(new QtTreePropertyBrowser(this))
		, mLogicalModelAssistApi(nullptr)
		, mVariantManager(nullptr)
		, mVariantFactory(nullptr)
		, mButtonManager(nullptr)
		, mButtonFactory(nullptr)
		, mController(nullptr)
{
	mPropertyEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

PropertyEditorView::~PropertyEditorView()
{
	delete mPropertyEditor;
	delete mVariantManager;
	delete mVariantFactory;
	delete mButtonManager;
	delete mButtonFactory;
}

void PropertyEditorView::init(qReal::models::LogicalModelAssistApi &logicalModelAssistApi
		, qReal::Controller &controller)
{
	mLogicalModelAssistApi = &logicalModelAssistApi; // unused
	mController = &controller;
}

/*
	QAbstractItemView's slots
*/
void PropertyEditorView::setModel(PropertyEditorModel *model)
{
	mModel = model;
	connect(mModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataChanged(QModelIndex,QModelIndex)));
	connect(mModel, SIGNAL(modelReset()), this, SLOT(reset()));
}

void PropertyEditorView::scrollTo(const QModelIndex &, QAbstractItemView::ScrollHint)
{
	// do nothing
}

void PropertyEditorView::reset()
{
	setRootIndex(QModelIndex());
}

void PropertyEditorView::update(const QModelIndex &)
{
	QWidget::update();
}

void PropertyEditorView::setPropertyToRoot(const QModelIndex &index, const QList<QPair<QString, QString>> &values
		, QtVariantProperty *vItem)
{
	QString value = mModel->getValueFromIndex(index);
	QVariant val(value);
	vItem->setValue(val);
	vItem->setToolTip(val.toString());
	if (!values.isEmpty()) {
		QStringList friendlyNames;
		for (QPair<QString, QString> const &pair : values) {
			friendlyNames << pair.second;
		}

		vItem->setAttribute("enumNames", friendlyNames);
		vItem->setAttribute("enumEditable", mModel->enumEditable(index));
		const int idx = enumPropertyIndexOf(index, val.toString());
		if (mModel->enumEditable(index)) {
			vItem->setValue(idx < 0 ? val : values[idx].second);
		} else {
			vItem->setValue(idx);
		}


	}

}

int PropertyEditorView::getType(const QString &typeName
					, bool &isButton, const QList<QPair<QString, QString>> &values)
{
	int type = QVariant::String;
	if (typeName == "int") {
		type = QVariant::Int;
	} else if (typeName == "bool") {
		type = QVariant::Bool;
	} else if (typeName == "string") {
		type = QVariant::String;
	} else if (typeName == "code" || typeName == "directorypath" || typeName == "filepath") {
		isButton = true;
	} else if (!values.isEmpty()) {
		type = QtVariantPropertyManager::enumTypeId();
	}

	return type;
}


void PropertyEditorView::setDescription(QtVariantProperty *vItem, int cellIndex)
{
	QString description = propertyDescription(cellIndex);
	if (!description.isEmpty()) {
		vItem->setToolTip(description);
	}
}

void PropertyEditorView::setRootIndex(const QModelIndex &index)
{
	mPropertyEditor->clear();

	mPropertyEditor->unsetFactoryForManager(mButtonManager);
	mPropertyEditor->unsetFactoryForManager(mVariantManager);

	delete mVariantManager;
	delete mVariantFactory;
	delete mButtonManager;
	delete mButtonFactory;

	mVariantManager = new QtVariantPropertyManager();
	mVariantFactory = new QtVariantEditorFactory();
	mButtonManager = new PushButtonPropertyManager();
	mButtonFactory = new PushButtonFactory();

	mPropertyEditor->setFactoryForManager(mButtonManager, mButtonFactory);
	mPropertyEditor->setFactoryForManager(mVariantManager, mVariantFactory);

	if (mModel->rowCount(index) > 0) {

		int i = 0;

		while (i < mModel->rowCount(index)) {
			QModelIndex valueIndex = mModel->index(i, 0);
			QString name = mModel->data(valueIndex).toString();
			if (name.isEmpty()) {
				return;
			}

			QString typeName = mModel->typeName(valueIndex).toLower();
			QList<QPair<QString, QString>> values = mModel->enumValues(valueIndex);
			bool isButton = false;
			int type = this->getType(typeName, isButton, values);

			QtProperty *item = nullptr;
			if (isButton) {
				item = mButtonManager->addProperty(name);
				mPropertyEditor->addProperty(item);
				++i;

			} else {
				QtGroupPropertyManager *groupManager = new QtGroupPropertyManager;
				QtVariantProperty *vItem = mVariantManager->addProperty(type, name);
				item = vItem;
				int count = mModel->countOfChilds(valueIndex);

				if (count != 0) {
					item = groupManager->addProperty(name);
				} else {
					this->setPropertyToRoot(valueIndex, values, vItem);
					this->setDescription(vItem, i);
				}

				QList<QtProperty*> list;
				for (int j = 0; j < count; ++j) {
					valueIndex = mModel->index(i, j + 1);
					name = mModel->data(valueIndex).toString();
					QtProperty *item1 = nullptr;
					typeName =  mModel->typeName(valueIndex).toLower();
					values = mModel->enumValues(valueIndex);
					isButton = false;

					type = this->getType(typeName, isButton, values);
					QtVariantProperty *vItem1 = mVariantManager->addProperty(type, name);
					this->setPropertyToRoot(valueIndex, values, vItem1);
					this->setDescription(vItem1, i + j + 1);

					item1 = vItem1;
					list.append(item1);
				}

				while (!list.isEmpty()) {
					item->addSubProperty(list.takeFirst());
				}

				mPropertyEditor->addProperty(item);
				if (count > 0) {
					i += count + 1;
				} else {
					++i;
				}
			}
		}

	}

	connect(mButtonManager, SIGNAL(buttonClicked(QtProperty*))
			, this, SLOT(buttonClicked(QtProperty*)), Qt::UniqueConnection);
	connect(mVariantManager, SIGNAL(valueChanged(QtProperty*, QVariant))
			, this, SLOT(editorValueChanged(QtProperty *, QVariant)), Qt::UniqueConnection);
	mPropertyEditor->setPropertiesWithoutValueMarked(true);
	mPropertyEditor->setRootIsDecorated(false);

}

void PropertyEditorView::setPropertyFromDataChanged(const QModelIndex &valueIndex, QtVariantProperty *prop
		, const int descriptionIndex)
{
	QString val = mModel->getValueFromIndex(valueIndex);
	QVariant value(val);

	if (prop) {

		if (prop->propertyType() == QtVariantPropertyManager::enumTypeId()
				&& !mModel->enumEditable(valueIndex))
		{
			value = enumPropertyIndexOf(valueIndex, value.toString());

		}

		setPropertyValue(prop, value);

		const QString description = propertyDescription(descriptionIndex);
		const QString tooltip = description.isEmpty() ? value.toString() : description;

		prop->setToolTip(tooltip);

	}
}

void PropertyEditorView::dataChanged(const QModelIndex &, const QModelIndex &)
{
	if (mModel->rowCount(QModelIndex()) <= 0) {
		return;
	}

	int i = 0;
	int row = 0;
	while (i < mPropertyEditor->properties().count()) {
		QtProperty *temp = mPropertyEditor->properties().at(i);
		if (!temp) {
			break;
		}

		QList<QtProperty*> childs = temp->subProperties();
		if (childs.isEmpty()) {
			if (dynamic_cast<QtVariantProperty*>(temp)) {
				QtVariantProperty *prop = dynamic_cast<QtVariantProperty*>(temp);
				const QModelIndex &valueIndex = mModel->index(row, 0);
				this->setPropertyFromDataChanged(valueIndex, prop, i);
			}
		}

		for (int j = 0; j < childs.count(); ++j) {
			QtVariantProperty *child = dynamic_cast<QtVariantProperty*>(childs.at(j));
			const QModelIndex &valueIndex = mModel->index(row, j + 1);
			this->setPropertyFromDataChanged(valueIndex, child, i);
		}

		if (childs.count() != 0) {
			row += childs.count() + 1;
		} else {
			++row;
		}

		++i;
	}
}

void PropertyEditorView::buttonClicked(QtProperty *property)
{
	qDebug() << "buttonClicked" << endl;
	int row = mPropertyEditor->properties().indexOf(property);
	const QModelIndex &index = mModel->index(row, 0);
	QString name = mModel->data(mModel->index(row, 0)).toString();
	QString propertyValue = mModel->getValueFromIndex(index);
	int role = mModel->roleByIndex(index.column() + index.row());

	const QPersistentModelIndex actualIndex = mModel->modelIndex(index.row() + index.column());

	// there are only four types of buttons: shape, reference, text and directory path
	if (name == "shape") {
		emit shapeEditorRequested(actualIndex, role, propertyValue, false);
	} else {
		const QString typeName = mModel->typeName(index).toLower();
		if (typeName == "code") {
			emit textEditorRequested(actualIndex, role, propertyValue);
		} else if (typeName == "directorypath") {
			const QString startPath = propertyValue.isEmpty()
					? QDir::homePath()
					: propertyValue;
			const QString location = utils::QRealFileDialog::getExistingDirectory("OpenDirectoryForPropertyEditor"
					, this, tr("Specify directory:"), startPath);
			mModel->setData(index, location);
		} else if (typeName == "filepath") {
			const QString startPath = propertyValue.isEmpty()
					? QDir::homePath()
					: propertyValue;
			const QString location = utils::QRealFileDialog::getOpenFileName("OpenFileForPropertyEditor"
					, this, tr("Select file:"), startPath);
			mModel->setData(index, location);
		} else {
			emit referenceListRequested(actualIndex, typeName, propertyValue, role);
		}
	}
}

void PropertyEditorView::editorValueChanged(QtProperty *prop, QVariant value)
{
	if (mChangingPropertyValue) {
		return;
	}

	QtVariantProperty *property = dynamic_cast<QtVariantProperty*>(prop);
	int propertyType = property->propertyType();

	QList<QtProperty*> list = mPropertyEditor->properties();
	int row = 0;

	QString firstPart = "";
	int i = 0;

	while (i < list.length()) {
		QtProperty* temp = list.at(i);
		QList<QtProperty*> childs = temp->subProperties();
		if (!childs.isEmpty()) {
			++row;

			for (QtProperty* property : childs) {
				if (property == prop) {
					firstPart = temp->propertyName();
					break;
				}

				++row;
			}

			if (!firstPart.isEmpty()) {
				break;
			}

		} else {
			if (temp == prop) {
				break;
			}

			++row;
		}

		++i;
	}


	const QModelIndex &index = mModel->index(row, 0);

	if (propertyType == QtVariantPropertyManager::enumTypeId()) {
		QList<QPair<QString, QString>> const values = mModel->enumValues(index);
		if (mModel->enumEditable(index)) {
			for (const auto &keyValue : values) {
				if (keyValue.second == value) {
					value = keyValue.first;
				}
			}
		} else {
			QString tempValue = "";
			for (const auto &keyValue : values) {
				if (keyValue.second == value) {
					value = keyValue.first;
					tempValue = value.toString();
				}
			}

			if (tempValue.isEmpty()) {
				const int intValue = value.toInt();
				if (intValue >= 0 && intValue < values.length()) {
					value = values.at(intValue).first;
				}
			}
		}
	}

	value = QVariant(value.toString());
	const Id id = mModel->idByIndex(index);

	const QString propertyName = mModel->propertyName(index);
	if (!firstPart.isEmpty()) {
		firstPart += "!";
	}

	// TODO: edit included Qt Property Browser framework or inherit new browser
	// from it and create propertyCommited() and propertyCancelled() signal
	qReal::commands::ChangePropertyCommand *changeCommand =
			new qReal::commands::ChangePropertyCommand(mLogicalModelAssistApi, firstPart + propertyName, id, value);
	mController->execute(changeCommand);
}

void PropertyEditorView::setPropertyValue(QtVariantProperty *property, const QVariant &value)
{
	bool old = mChangingPropertyValue;
	mChangingPropertyValue = true;
	property->setValue(value);
	mChangingPropertyValue = old;
}

QString PropertyEditorView::propertyDescription(const int cellIndex) const
{
	const QModelIndex keyIndex = mModel->index(cellIndex, 0);
	return mModel->data(keyIndex, Qt::ToolTipRole).toString();
}

int PropertyEditorView::enumPropertyIndexOf(const QModelIndex &index, const QString &value)
{
	QList<QPair<QString, QString>> const values = mModel->enumValues(index);
	for (int index = 0; index < values.count(); ++index) {
		if (values[index].first == value) {
			return index;
		}
	}

	return -1;
}

void PropertyEditorView::resizeEvent(QResizeEvent *event ) {
	mPropertyEditor->resize(event->size());
}

void PropertyEditorView::installEventFilter(QObject *obj)
{
	QWidget::installEventFilter(obj);
	mPropertyEditor->window()->installEventFilter(obj);
}
