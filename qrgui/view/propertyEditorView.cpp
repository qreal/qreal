#include "propertyEditorView.h"
#include "../mainwindow/mainWindow.h"


PropertyEditorView::PropertyEditorView(QWidget *parent) :
	QWidget(parent), mChangingPropertyValue(false),
	mModel(NULL), mPropertyEditor(new QtTreePropertyBrowser(this)),
	mLogicalModelAssistApi(NULL)
{
	mPropertyEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

PropertyEditorView::~PropertyEditorView()
{
	delete mPropertyEditor;
}

void PropertyEditorView::init(qReal::MainWindow *mainWindow, qReal::models::LogicalModelAssistApi *const logicalModelAssistApi)
{
	mMainWindow = mainWindow;
	mLogicalModelAssistApi = logicalModelAssistApi; // unused
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
		if (typeName == "int") {
			type = QVariant::Int;
		} else if (typeName == "bool") {
			type = QVariant::Bool;
		} else if (typeName == "string") {
			type = QVariant::String;
		} else if (!values.isEmpty()) {
			type = QtVariantPropertyManager::enumTypeId();
		} else {
			if (name == "shape") { // hack
				isButton = true;
			}
		}

		QtProperty *item = NULL;
		if (isButton) {
			item = buttonManager->addProperty(name);
		} else {
			QtVariantProperty *vItem = variantManager->addProperty(type, name);

			vItem->setValue(value);
			vItem->setToolTip(value.toString());
			if (!values.isEmpty()) {
				vItem->setAttribute("enumNames", values);
				QVariant idx(enumPropertyIndexOf(valueCell, value.toString()));
				vItem->setValue(idx);
			}
			item = vItem;
		}
		mPropertyEditor->addProperty(item);
	}

	connect(buttonManager, SIGNAL(buttonClicked(QtProperty*)),
		this, SLOT(buttonClicked(QtProperty*)));
	connect(variantManager, SIGNAL(valueChanged(QtProperty*, QVariant)),
		this, SLOT(editorValueChanged(QtProperty *, QVariant)));
	mPropertyEditor->setPropertiesWithoutValueMarked(true);
	mPropertyEditor->setRootIsDecorated(false);
}

void PropertyEditorView::dataChanged(const QModelIndex &, const QModelIndex &)
{
	for (int i = 0, rows = mModel->rowCount(QModelIndex()); i < rows; ++i) {
		QModelIndex const &valueIndex = mModel->index(i, 1);
		QtVariantProperty *property = dynamic_cast<QtVariantProperty*>(mPropertyEditor->properties().at(i));
		QVariant value = valueIndex.data();
		if (property) {
			if (property->propertyType() == QtVariantPropertyManager::enumTypeId()) {
				value = enumPropertyIndexOf(valueIndex, value.toString());
			}

			setPropertyValue(property, value);
			property->setToolTip(value.toString());
		}
	}
}

void PropertyEditorView::buttonClicked(QtProperty *property)
{
	int row = mPropertyEditor->properties().indexOf(property);
	QModelIndex const &index = mModel->index(row, 1);

	QString propertyValue = index.data(Qt::DisplayRole).toString();
	QPersistentModelIndex const actualIndex = mModel->modelIndex(index.row());
	int role = mModel->roleByIndex(index.row());

	mMainWindow->openShapeEditor(actualIndex, role, propertyValue);
}

void PropertyEditorView::editorValueChanged(QtProperty *prop, QVariant value)
{
	if(mChangingPropertyValue) return;

	QtVariantProperty *property = dynamic_cast<QtVariantProperty*>(prop);
	int propertyType = property->propertyType(),
		row = mPropertyEditor->properties().indexOf(property);
	QModelIndex const &index = mModel->index(row, 1);
	if (propertyType == QtVariantPropertyManager::enumTypeId()) {
		QStringList const &values = mModel->enumValues(index);
		int intValue = value.toInt();
		if (intValue >= 0 && intValue < values.length()) {
			value = values.at(intValue);
		}
	}
	value = QVariant(value.toString());
	mModel->setData(index, value);
}

void PropertyEditorView::setPropertyValue(QtVariantProperty *property, const QVariant &value)
{
	bool old = mChangingPropertyValue;
	mChangingPropertyValue = true;
	property->setValue(value);
	mChangingPropertyValue = old;
}

int PropertyEditorView::enumPropertyIndexOf(QModelIndex const &index, QString const &value)
{
	QStringList const &values = mModel->enumValues(index);
	if (!values.empty()) {
		return values.indexOf(value);
	}
	return -1;
}
