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


	int helper = 0;
	if (mModel->rowCount(index) > 0) {
		for (int i = 0; i < mModel->rowCount(index); ++i) {
			QString name1 = mModel->data(mModel->index(i + helper, 0)).toString();
			if (name1.isEmpty()) {
				return;
			}
			int type = QVariant::String;

			QtProperty *item = nullptr;
			QtGroupPropertyManager *groupManager = new QtGroupPropertyManager;
			QtVariantProperty *vItem = mVariantManager->addProperty(type, name1);

			item = vItem;
			if (name1.contains("Role")) {
				item = groupManager->addProperty(name1);

			}


			QList<QtProperty*> list;

			int count = mModel->countOfChilds(mModel->index(i + helper, 0));

			for (int j = 1; j < count + 1; ++j) {
				QString name2 = mModel->data(mModel->index(i + helper, j)).toString();
				const QModelIndex &valueIndex1 = mModel->index(i + helper, j);
				QString value1 = mModel->getValueFromIndex(valueIndex1);
				QVariant val(value1);
				QtProperty *item1 = nullptr;

				QtVariantProperty *vItem1 = mVariantManager->addProperty(type, name2);
				vItem1->setValue(val);

				item1 = vItem1;

				list.append(item1);

			}
			helper += count;

			while (!list.isEmpty()) {
				item->addSubProperty(list.takeFirst());
			}

			mPropertyEditor->addProperty(item);


		}
	}


//	for (int i = 0, rows = mModel->rowCount(index); i < rows; ++i) {
//		const QModelIndex &valueCell = mModel->index(i, 1);
//		QString name = mModel->data(mModel->index(i, 0)).toString();
//		const QVariant &value = mModel->data(valueCell);


//		int type = QVariant::String;
//		QString typeName = mModel->typeName(valueCell).toLower();
//		QList<QPair<QString, QString>> const values = mModel->enumValues(valueCell);
//		bool isButton = false;

//		if (typeName == "int") {
//			type = QVariant::Int;
//		} else if (typeName == "bool") {
//			type = QVariant::Bool;
//		} else if (typeName == "string") {
//			type = QVariant::String;
//		} else if (typeName == "code" || typeName == "directorypath" || typeName == "filepath") {
//			isButton = true;
//		} else if (!values.isEmpty()) {
//			type = QtVariantPropertyManager::enumTypeId();
//		} else {
//			if (name == "shape" || mModel->isReference(valueCell, name)) { // hack
//				isButton = true;
//			}
//		}

//		QtProperty *item = nullptr;
//		if (isButton) {
//			item = mButtonManager->addProperty(name);
//		} else {
//			QtVariantProperty *vItem = mVariantManager->addProperty(type, name);

//			vItem->setValue(value);
//			vItem->setToolTip(value.toString());

//			if (!values.isEmpty()) {
//				QStringList friendlyNames;
//				for (QPair<QString, QString> const &pair : values) {
//					friendlyNames << pair.second;
//				}

//				vItem->setAttribute("enumNames", friendlyNames);
//				vItem->setAttribute("enumEditable", mModel->enumEditable(valueCell));
//				const int idx = enumPropertyIndexOf(valueCell, value.toString());
//				if (mModel->enumEditable(valueCell)) {
//					vItem->setValue(idx < 0 ? value.toString() : values[idx].second);
//				} else {
//					vItem->setValue(idx);
//				}
//			}

//			item = vItem;
//		}

//		const QString description = propertyDescription(i);

//		if (!description.isEmpty()) {
//			item->setToolTip(description);
//		}

//		QtProperty *item1 = item;

//		QtGroupPropertyManager *groupManager = new QtGroupPropertyManager;
//		item = groupManager->addProperty(name);
//		item->addSubProperty(item1);

//		item->addSubProperty(item1);

//		mPropertyEditor->addProperty(item);
//	}

	connect(mButtonManager, SIGNAL(buttonClicked(QtProperty*))
			, this, SLOT(buttonClicked(QtProperty*)), Qt::UniqueConnection);
	connect(mVariantManager, SIGNAL(valueChanged(QtProperty*, QVariant))
			, this, SLOT(editorValueChanged(QtProperty *, QVariant)), Qt::UniqueConnection);
	mPropertyEditor->setPropertiesWithoutValueMarked(true);
	mPropertyEditor->setRootIsDecorated(false);
}

void PropertyEditorView::dataChanged(const QModelIndex &left, const QModelIndex &right)
{
	int helper = 0;

	if (mModel->rowCount(QModelIndex()) <= 0) {
		return;
	}

	QList<QtProperty*> list = mPropertyEditor->properties();

	for (int i = 0; i < list.count(); ++i) {
		QtProperty *temp = list.at(i);
		if (!temp) {
			break;
		}

		QList<QtProperty*> childs = temp->subProperties();
		if (childs.isEmpty()) {
			break;
		}

		for (int j = 0; j < childs.count(); ++j) {
			QtVariantProperty *child = dynamic_cast<QtVariantProperty*>(childs.at(j));
			QString val = child->valueText();
			qDebug() << val << endl;


			QVariant value(val);
			const QModelIndex &valueIndex = mModel->index(i + helper, j + 1);
			if (!val.isEmpty()) {
				mModel->setValueForIndex(valueIndex, val);

				if (child) {
					if (child->propertyType() == QtVariantPropertyManager::enumTypeId()
							&& !mModel->enumEditable(valueIndex))
					{
						value = enumPropertyIndexOf(valueIndex, value.toString());
					}

					setPropertyValue(child, value);

					const QString description = propertyDescription(i);
					const QString tooltip = description.isEmpty() ? value.toString() : description;

					child->setToolTip(tooltip);
				}
			}
		}
	}

}

void PropertyEditorView::buttonClicked(QtProperty *property)
{
	qDebug() << "buttonClicked" << endl;
	int row = mPropertyEditor->properties().indexOf(property);
	const QModelIndex &index = mModel->index(row, 1);
	QString name = mModel->data(mModel->index(row, 0)).toString();
	QString propertyValue = index.data(Qt::DisplayRole).toString();
	int role = mModel->roleByIndex(index.row());

	const QPersistentModelIndex actualIndex = mModel->modelIndex(index.row());

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
	qDebug () << "editorValueChanged" << endl;
	if (mChangingPropertyValue) {
		return;
	}

	QtVariantProperty *property = dynamic_cast<QtVariantProperty*>(prop);
	int propertyType = property->propertyType();

	QList<QtProperty*> list = mPropertyEditor->properties();
	int row = 1;
	int column = 0;

	QString firstPart = "";

	for (QtProperty* temp : list) {
		QList<QtProperty*> childs = temp->subProperties();
		for (QtProperty* child : childs) {
			if (child == prop) {
				firstPart = temp->propertyName();
				break;
			} else {
				++row;
			}
		}

		if (!firstPart.isEmpty()) {
			break;
		}
		++row;
	}

	if (firstPart.isEmpty()) {
		row = 0;
		for (QtProperty* temp : list) {
			if (temp != prop) {
				++row;
			} else {
				break;
			}
		}

	}

	const QModelIndex &index = mModel->index(row + column, column); //row

	if (propertyType == QtVariantPropertyManager::enumTypeId()) {
		QList<QPair<QString, QString>> const values = mModel->enumValues(index);
		if (mModel->enumEditable(index)) {
			for (const auto &keyValue : values) {
				if (keyValue.second == value) {
					value = keyValue.first;
				}
			}
		} else {
			const int intValue = value.toInt();
			if (intValue >= 0 && intValue < values.length()) {
				value = values.at(intValue).first;
			}
		}
	}

	value = QVariant(value.toString());
	const Id id = mModel->idByIndex(index);

	const QString propertyName = mModel->propertyName(index);

	// TODO: edit included Qt Property Browser framework or inherit new browser
	// from it and create propertyCommited() and propertyCancelled() signal
	qReal::commands::ChangePropertyCommand *changeCommand =
			new qReal::commands::ChangePropertyCommand(mLogicalModelAssistApi, firstPart + "!" + propertyName, id, value);
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
