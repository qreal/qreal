#include <QtGui/QGridLayout>

#include "elementEditor.h"
#include "../../qrutils/outFile.h"

using namespace qReal;

ElementEditor::ElementEditor(const QPersistentModelIndex &index
		, const int &role, QWidget *parent)
	: QWidget(parent), mIndex(index), mRole(role)
	, mWidgetBased(false), mLayout(new QStackedLayout)
	, mStartWidget(new QWidget), mWidgetsEditor(NULL)
	, mShapeEditor(NULL), mOpenedFromMetaEditor(true)
{
	initComponents();
}

ElementEditor::ElementEditor(QWidget *parent)
	: QWidget(parent), mRole(0), mWidgetBased(false)
	, mLayout(new QStackedLayout), mStartWidget(new QWidget)
	, mWidgetsEditor(NULL), mShapeEditor(NULL)
	, mOpenedFromMetaEditor(false)
{
	initComponents();
}

ShapeEdit *ElementEditor::shapeEditor() const
{
	return mShapeEditor;
}

widgetsEdit::WidgetsEditor *ElementEditor::widgetEditor() const
{
	return mWidgetsEditor;
}

bool ElementEditor::isWidgetBased() const
{
	return mWidgetBased;
}

void ElementEditor::load(const QString &data)
{
	QDomDocument document;
	document.setContent(data);
	if (document.isNull()) {
		return;
	}
	if (isWidgetBasedDocument(document)) {
		onWidgetBasedButtonClicked();
		mWidgetsEditor->load(document);
	} else {
		onShapeBasedButtonClicked();
		mShapeEditor->load(document);
	}
}

bool ElementEditor::isWidgetBasedDocument(QDomDocument const &document)
{
	QDomElement const graphicsElement = document.firstChild().toElement();
	if (graphicsElement.isNull()) {
		return false;
	}
	return !graphicsElement.firstChildElement("widget-template").isNull();
}

void ElementEditor::initComponents()
{
	setLayout(mLayout);
	initStartWidget();
}

void ElementEditor::initStartWidget()
{
	QIcon const shapeIcon(":/icons/widgetsEditor/shapeEditor.png");
	QIcon const widgetsIcon(":/icons/widgetsEditor/widgetEditor.png");
	QString const shapeToolTip = tr("Create shape-based element");
	QString const widgetsToolTip = tr("Create widget-based element");

	QPushButton *shapeEditorButton = new QPushButton;
	QPushButton *widgetsEditorButton = new QPushButton;
	connect(shapeEditorButton, SIGNAL(clicked()),
			this, SLOT(onShapeBasedButtonClicked()));
	connect(widgetsEditorButton, SIGNAL(clicked())
		, this, SLOT(onWidgetBasedButtonClicked()));
	shapeEditorButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	widgetsEditorButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	shapeEditorButton->setToolTip(shapeToolTip);
	widgetsEditorButton->setToolTip(widgetsToolTip);
	shapeEditorButton->setIcon(shapeIcon);
	widgetsEditorButton->setIcon(widgetsIcon);
	// TODO: Draw with non-constant size
	QSize const iconSize(300, 500);
	shapeEditorButton->setIconSize(iconSize);
	widgetsEditorButton->setIconSize(iconSize);

	QGridLayout *startLayout = new QGridLayout;
	startLayout->setMargin(3);
	startLayout->addWidget(shapeEditorButton, 0, 0);
	startLayout->addWidget(widgetsEditorButton, 0, 1);
	mStartWidget->setLayout(startLayout);

	mLayout->addWidget(mStartWidget);
}

void ElementEditor::showWidget(QWidget *widget)
{
	if (mLayout->count() > 1) {
		mLayout->removeWidget(mLayout->widget(mLayout->count()-1));
	}
	mLayout->addWidget(widget);
	mLayout->setCurrentIndex(mLayout->count()-1);
}

void ElementEditor::showShapeEditor()
{
	if (!mShapeEditor) {
		initShapeEditor();
	}
	mShapeEditor->setWidgetBased(mWidgetBased);
	showWidget(mShapeEditor);
}

void ElementEditor::showWidgetsEditor()
{
	if (!mWidgetsEditor) {
		initWidgetEditor();
	}
	showWidget(mWidgetsEditor);
}

void ElementEditor::onWidgetEditorRequestedShape(QDomDocument const &shape)
{
	showShapeEditor();
	mShapeEditor->load(shape);
}

void ElementEditor::initWidgetEditor()
{
	mWidgetBased = true;
	if (mOpenedFromMetaEditor) {
		mWidgetsEditor = new widgetsEdit::WidgetsEditor(mIndex, mRole);
	} else {
		mWidgetsEditor = new widgetsEdit::WidgetsEditor;
	}
	connect(mWidgetsEditor, SIGNAL(shapeRequested(QDomDocument))
			, this, SLOT(onWidgetEditorRequestedShape(QDomDocument)));
	connect(mWidgetsEditor, SIGNAL(widgetSaved(QString,QPersistentModelIndex,int))
			, this, SLOT(onWidgetEditorSavedShape(QString,QPersistentModelIndex,int)));
	connect(mWidgetsEditor, SIGNAL(changeToShapeType(QDomDocument))
			, this, SLOT(onSwitchedToShape(QDomDocument)));
}

void ElementEditor::initShapeEditor()
{
	mWidgetBased = false;
	if (mOpenedFromMetaEditor) {
		mShapeEditor = new ShapeEdit(mIndex, mRole);
	} else {
		mShapeEditor = new ShapeEdit;
	}
	connect(mShapeEditor, SIGNAL(shapeSaved(QString,QPersistentModelIndex,int))
			, this, SLOT(onShapeEditorSavedShape(QString,QPersistentModelIndex,int)));
	connect(mShapeEditor, SIGNAL(switchToWidgetsEditor(QDomDocument))
			, this, SLOT(onSwitchedToWidget(QDomDocument)));
}

void ElementEditor::onWidgetBasedButtonClicked()
{
	showWidgetsEditor();
}

void ElementEditor::onShapeBasedButtonClicked()
{
	showShapeEditor();
}

void ElementEditor::onWidgetEditorSavedShape(const QString &widget
		, const QPersistentModelIndex &index, const int &role)
{
	emit widgetSaved(widget, index, role);
}

void ElementEditor::onShapeEditorSavedShape(const QString &shape
		, const QPersistentModelIndex &index, const int &role)
{
	emit shapeSaved(shape, index, role);
}

void ElementEditor::onSwitchedToWidget(const QDomDocument &document)
{
	mWidgetBased = true;
	showWidgetsEditor();
	mWidgetsEditor->setShape(document);
}

void ElementEditor::onSwitchedToShape(const QDomDocument &document)
{
	mWidgetBased = false;
	showShapeEditor();
	mShapeEditor->load(document);
}
