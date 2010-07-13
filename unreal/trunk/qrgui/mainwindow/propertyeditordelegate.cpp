#include "propertyeditordelegate.h"
#include "propertyeditorproxymodel.h"
#include <QtCore/QDebug>
#include <QtGui/QComboBox>

#include "../model/model.h"
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
	QStringList values = model->getEnumValues(index);
	if (!values.isEmpty()){
		QComboBox *editor = new QComboBox(parent);
		foreach (QString item, values)
			editor->addItem(item);
		return editor;
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
	QString value = lineEdit ? lineEdit->text() : (comboEdit ? comboEdit->currentText() : "");

	model->setData(index, value, Qt::EditRole);
}

void PropertyEditorDelegate::updateEditorGeometry(QWidget *editor,
												  const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
	editor->setGeometry(option.rect);
}
