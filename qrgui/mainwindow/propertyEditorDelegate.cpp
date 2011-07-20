#include "propertyEditorDelegate.h"
#include "propertyEditorProxyModel.h"
#include <QtCore/QDebug>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#include <QtGui/QPushButton>

#include "mainWindow.h"
#include "openShapeEditorButton.h"

#include "referenceTypeWindow.h"
#include "buttonRefWindow.h"

using namespace qReal;

PropertyEditorDelegate::PropertyEditorDelegate(QObject *parent)
		: QItemDelegate(parent)
		, mMainWindow(NULL)
		, mLogicalModelAssistApi(NULL)
{
}

QWidget *PropertyEditorDelegate::createEditor(QWidget *parent,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const
{
	Q_UNUSED(option);

	PropertyEditorModel *model = const_cast<PropertyEditorModel*>(dynamic_cast<const PropertyEditorModel*>(index.model()));
	QString propertyName = model->data(index.sibling(index.row(), 0), Qt::DisplayRole).toString();
	if (propertyName == "shape") {
		QString propertyValue = model->data(index.sibling(index.row(), index.column()), Qt::DisplayRole).toString();
		QPersistentModelIndex const actualIndex = model->modelIndex(index.row());
		int role = model->roleByIndex(index.row());
		OpenShapeEditorButton *button = new OpenShapeEditorButton(parent, actualIndex, role, propertyValue);
		button->setText("Open Shape Editor");
		QObject::connect(button, SIGNAL(clicked()), mMainWindow, SLOT(openShapeEditor()));
		return button;
	}

	QStringList const values = model->enumValues(index);
	if (!values.isEmpty()) {
		QComboBox * const editor = new QComboBox(parent);
		foreach (QString item, values)
			editor->addItem(item.replace("&lt;", "<"));
		return editor;
	}

	QString typeName = model->typeName(index).toLower();
	if (typeName != "int" && typeName != "string" && !typeName.isEmpty()) {
		int const role = model->roleByIndex(index.row());
		QModelIndex const &actualIndex = model->modelIndex(index.row());
		ButtonRefWindow * const button = new ButtonRefWindow(parent, typeName
				, *mLogicalModelAssistApi, role, actualIndex, mMainWindow);
		return button;
	}
	QLineEdit * const editor = new QLineEdit(parent);

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

void PropertyEditorDelegate::setModelData(QWidget *editor
		, QAbstractItemModel *model
		, const QModelIndex &index) const
{
	QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(editor);
	QComboBox *comboEdit = dynamic_cast<QComboBox*>(editor);

	QString value;
	if (lineEdit)
		value = lineEdit->text();
	else if (comboEdit)
		value = comboEdit->currentText().replace("<", "&lt;");
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

void PropertyEditorDelegate::init(MainWindow *mainWindow
		, qReal::models::LogicalModelAssistApi * const logicalModelAssistApi)
{
	mMainWindow = mainWindow;
	mLogicalModelAssistApi = logicalModelAssistApi;
}
