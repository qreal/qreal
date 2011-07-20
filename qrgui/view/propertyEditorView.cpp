#include "propertyEditorView.h"
#include "mainWindow.h"



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

	PushButtonPropertyManager *buttonManager = new PushButtonPropertyManager();
	PushButtonFactory *buttonFactory = new PushButtonFactory();

	mPropertyEditor->setFactoryForManager(buttonManager, buttonFactory);
	mPropertyEditor->setFactoryForManager(variantManager, variantFactory);

	for(int i = 0, rows = mModel->rowCount(index); i < rows; ++i){
		QModelIndex const &valueCell = mModel->index(i, 1);
		QString name = mModel->data(mModel->index(i, 0)).toString();
		QVariant const &value = mModel->data(valueCell);

		int type = QVariant::String;
		QString typeName = mModel->typeName(valueCell).toLower();
		QStringList values = mModel->enumValues(valueCell);
		bool isButton = false;
		if(typeName == "int"){
			type = QVariant::Int;
		} else if(typeName == "bool"){
			type = QVariant::Bool;
		} else if(typeName == "string") {
			type = QVariant::String;
		} else if(!values.isEmpty()){
			type = QtVariantPropertyManager::enumTypeId();
		} else {
			if(name == "shape") // hack
				isButton = true;
		}
//		qDebug() << name << ":" << typeName;

		QtProperty *item = NULL;
		if(isButton){
			item = buttonManager->addProperty(name);
		} else {
			QtVariantProperty *vItem = variantManager->addProperty(type, name);

			vItem->setValue(value);
			vItem->setToolTip(value.toString());
			if(!values.isEmpty()){
				vItem->setAttribute("enumNames", values);
				if(values.contains(value.toString())){
					vItem->setValue(values.indexOf(value.toString()));
				}
			}
			item = vItem;
		}
		mPropertiesRows[valueCell] = item;
		mPropertyEditor->addProperty(item);
	}

	connect(buttonManager, SIGNAL(buttonClicked(QtProperty*)),
		this, SLOT(buttonClicked(QtProperty*))); // <---==
	connect(variantManager, SIGNAL(valueChanged(QtProperty*, QVariant)),
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
		if(property){
			property->setValue(value);
			property->setToolTip(value.toString());
		}
	}
}

void PropertyEditorView::buttonClicked(QtProperty *property)
{
	QModelIndex const &index = mPropertiesRows.key(property);

	QString propertyValue = index.data(Qt::DisplayRole).toString();
	QPersistentModelIndex const actualIndex = mModel->modelIndex(index.row());
	int role = mModel->roleByIndex(index.row());

	mMainWindow->openShapeEditor(actualIndex, role, propertyValue);
}

void PropertyEditorView::init(qReal::MainWindow *mainWindow, qReal::models::LogicalModelAssistApi *const logicalModelAssistApi)
{
	mMainWindow = mainWindow;
	mLogicalModelAssistApi = logicalModelAssistApi;
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
