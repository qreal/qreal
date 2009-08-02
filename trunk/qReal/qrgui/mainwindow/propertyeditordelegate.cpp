#include "propertyeditordelegate.h"

PropertyEditorDelegate::PropertyEditorDelegate(QObject *parent)
		: QItemDelegate(parent)
{
}

QWidget *PropertyEditorDelegate::createEditor(QWidget *parent,
											  const QStyleOptionViewItem &/*option*/,
											  const QModelIndex &/*index*/) const
{
	QLineEdit *editor = new QLineEdit(parent);

	return editor;
}

void PropertyEditorDelegate::setEditorData(QWidget *editor,
										   const QModelIndex &index) const
{
	QString value = index.model()->data(index, Qt::DisplayRole).toString();

	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
	lineEdit->setText(value);
}

void PropertyEditorDelegate::setModelData(QWidget *editor,
										  QAbstractItemModel *model,
										  const QModelIndex &index) const
{
	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
	QString value = lineEdit->text();

	model->setData(index, value, Qt::EditRole);
}

void PropertyEditorDelegate::updateEditorGeometry(QWidget *editor,
												  const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
	editor->setGeometry(option.rect);
}
