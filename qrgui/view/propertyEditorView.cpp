#include "propertyEditorView.h"

#include <QTreeWidget>

PropertyEditorView::PropertyEditorView(QWidget *parent) :
	QWidget(parent)
{
	mModel = NULL;
	mPropertyEditor = new QtTreePropertyBrowser(this);
	mPropertyEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

/*
	QAbstractItemView's slots
*/
void PropertyEditorView::setModel(PropertyEditorModel *model)
{
	mModel = model;
	connect(mModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
		this, SLOT(dataChanged(QModelIndex,QModelIndex)));
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

void PropertyEditorView::editorValueChanged(QtProperty *prop, QVariant value)
{
	QtVariantProperty *property = dynamic_cast<QtVariantProperty*>(prop);
	int propertyType = property->propertyType();
	QModelIndex const &index = mPropertiesRows.key(property);
	if(propertyType == QtVariantPropertyManager::enumTypeId()){
		QStringList const &values = mModel->enumValues(index);
		int intValue = value.toInt();
		if(intValue >= 0 && intValue < values.length()){
			value = values.at(intValue);
		}
	}
	mModel->setData(index, value, Qt::EditRole);
}


void PropertyEditorView::update(const QModelIndex &)
{
	QWidget::update();
}

void PropertyEditorView::setRootIndex(const QModelIndex &index)
{
	mPropertyEditor->clear();
	mPropertiesRows.clear();
	QtVariantPropertyManager *variantManager = new QtVariantPropertyManager();
	QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();
	mPropertyEditor->setFactoryForManager(variantManager, variantFactory);

	for(int i = 0, rows = mModel->rowCount(index); i < rows; ++i){
		QModelIndex const &valueCell = mModel->index(i, 1);
		QString name = mModel->data(mModel->index(i, 0)).toString();
		QVariant const &value = mModel->data(valueCell);

		int type = QVariant::String;
		QString typeName = mModel->typeName(valueCell).toLower();
		QStringList values = mModel->enumValues(valueCell);
		if(typeName == "int"){
			type = QVariant::Int;
		} else if(typeName == "bool"){
			type = QVariant::Bool;
		} else if(!values.isEmpty()){
			type = QtVariantPropertyManager::enumTypeId();
		}

		QtVariantProperty *item = variantManager->addProperty(type, name);
		item->setValue(value);
		item->setToolTip(value.toString());
		if(!values.isEmpty()){
			item->setAttribute("enumNames", values);
			if(values.contains(value.toString())){
				item->setValue(values.indexOf(value.toString()));
			}
		}
		mPropertiesRows[valueCell] = item;
		mPropertyEditor->addProperty(item);
	}

	connect(variantManager, SIGNAL(valueChanged(QtProperty*,QVariant)),
		this, SLOT(editorValueChanged(QtProperty *, QVariant)));
	mPropertyEditor->setPropertiesWithoutValueMarked(true);
	mPropertyEditor->setRootIsDecorated(false);
}

void PropertyEditorView::dataChanged(const QModelIndex &, const QModelIndex &)
{
	for(int i = 0, rows = mModel->rowCount(QModelIndex()); i < rows; ++i){
		QModelIndex const &valueIndex = mModel->index(i, 1);
		QtVariantProperty *property = dynamic_cast<QtVariantProperty*>(mPropertiesRows[valueIndex]);
		QVariant const &value = valueIndex.data();
		property->setValue(value);
		property->setToolTip(value.toString());
	}
}
