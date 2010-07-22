#include "propertyeditordelegate.h"
#include "propertyeditorproxymodel.h"
#include <QtCore/QDebug>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#include <QtGui/QPushButton>

#include "../model/model.h"
#include "mainwindow.h"
#include "openShapeEditorButton.h"

#include "referencetypewindow.h"
#include "buttonRefWindow.h"

using namespace qReal;

PropertyEditorDelegate::PropertyEditorDelegate(QObject *parent)
		: QItemDelegate(parent)
{
}

QWidget *PropertyEditorDelegate::createEditor(QWidget *parent,
											  const QStyleOptionViewItem &/*option*/,
											  const QModelIndex &index) const
{
	PropertyEditorModel *model = const_cast<PropertyEditorModel*>(dynamic_cast<const PropertyEditorModel*>(index.model()));
	QString propertyName = model->data(index.sibling(index.row(), 0), Qt::DisplayRole).toString();
	if (propertyName == "shape") {
		QString propertyValue = model->data(index.sibling(index.row(), index.column()), Qt::DisplayRole).toString();
		QPersistentModelIndex const myIndex = model->getModelIndex();
		int role = model->roleByIndex(index.row());
		OpenShapeEditorButton *button = new OpenShapeEditorButton(parent, myIndex, role, propertyValue);
		button->setText("open Shape Editor");
		QObject::connect(button, SIGNAL(clicked()), mMainWindow, SLOT(openNewEmptyTab()));
		return button;
	}
	QStringList values = model->getEnumValues(index);
	if (!values.isEmpty()) {
		QComboBox *editor = new QComboBox(parent);
		foreach (QString item, values)
			editor->addItem(item);
		return editor;
	}
	if (index.row() != 2)
	{
		QString typeName = model->getTypeName(index);
		if ((typeName != "int") && (typeName != "string") && (typeName != ""))
		{
			QAbstractItemModel* targModel = model->getTargetModel();
			int role = model->roleByIndex(index.row());
			const QModelIndex &ind = model->getModelIndex();
			qReal::MainWindow *mainWindow = mMainWindow;
			ButtonRefWindow *button = new ButtonRefWindow(parent, typeName, &(model->getApi()),
														  targModel, role, ind, mainWindow);
			QVariant data = targModel->data(ind, role);
			return button;
		}
	}
	QLineEdit *editor = new QLineEdit(parent);

	return editor;
}

void PropertyEditorDelegate::setEditorData(QWidget *editor,
										   const QModelIndex &index) const
{
	QString value = index.model()->data(index, Qt::DisplayRole).toString();

	QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(editor);
	if (lineEdit)
		lineEdit->setText(value);
	else {
		QComboBox *comboEdit = dynamic_cast<QComboBox*>(editor);
		if (comboEdit)
			comboEdit->setCurrentIndex(comboEdit->findText(value));
	}
}

void PropertyEditorDelegate::setModelData(QWidget *editor,
										  QAbstractItemModel *model,
										  const QModelIndex &index) const
{
	QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(editor);
	QComboBox *comboEdit = dynamic_cast<QComboBox*>(editor);

	QString value;
	if (lineEdit)
		value = lineEdit->text();
	else if (comboEdit)
		value = comboEdit->currentText();
	else
		return;

	model->setData(index, value, Qt::EditRole);
	mMainWindow->propertyEditorScrollTo(index);
}

void PropertyEditorDelegate::updateEditorGeometry(QWidget *editor,
												  const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
	editor->setGeometry(option.rect);
}

void PropertyEditorDelegate::setMainWindow(MainWindow *mainwindow)
{
	mMainWindow = mainwindow;
}
