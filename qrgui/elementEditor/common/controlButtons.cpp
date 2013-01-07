#include "controlButtons.h"

using namespace qReal::elementEdit;

ControlButtons::ControlButtons(bool forShapeEditor, bool isIconEditor)
	: NavigationMenuContent(), mForShapeEditor(forShapeEditor)
{
	addStretch();

	if (isIconEditor) {
		QPushButton *acceptIcon = createButton(tr("Ready")
			, ":/icons/widgetsEditor/ok.png");
		QPushButton *rejectIcon = createButton(tr("Cancel")
			, ":/icons/widgetsEditor/cancel.png");
		connect(acceptIcon, SIGNAL(clicked()), this, SLOT(onIconAccepted()));
		connect(rejectIcon, SIGNAL(clicked()), this, SLOT(onIconRejected()));
	} else {
		QPushButton *icon = createButton(tr("Icon...")
			, ":/icons/widgetsEditor/icon.png");
		connect(icon, SIGNAL(clicked()), this, SLOT(onIconClicked()));
	}
	QPushButton *save = createButton(tr("Save")
		, ":/icons/widgetsEditor/save.png");
	QPushButton *saveAsImage= createOnlyFor(true, tr("Save as image...")
		, ":/icons/widgetsEditor/saveAsImage.png");
	QPushButton *saveToDisk= createButton(tr("Save to disk as XML...")
		, ":/icons/widgetsEditor/saveToDisk.png");
	QPushButton *loadFromDisk= createButton(tr("Load from disk...")
		, ":/icons/widgetsEditor/loadFromDisk.png");
	QPushButton *switchToWidgets = createOnlyFor(true, tr("Switch to widgets")
		, ":/icons/widgetsEditor/widgetIcon.png");
	QPushButton *preview = createOnlyFor(false, tr("Preview...")
		, ":/icons/preview.png");
	QPushButton *switchToShape = createOnlyFor(false, tr("Switch to shape")
		, ":/icons/widgetsEditor/shapeIcon.png");

	connect(save, SIGNAL(clicked()), this, SLOT(onSaveClicked()));
	if (saveAsImage) {
		connect(saveAsImage, SIGNAL(clicked()), this, SLOT(onSaveAsImageClicked()));
	}
	connect(saveToDisk, SIGNAL(clicked()), this, SLOT(onSaveToDiskClicked()));
	connect(loadFromDisk, SIGNAL(clicked()), this, SLOT(onLoadFromDiskClicked()));
	if (switchToWidgets) {
		connect(switchToWidgets, SIGNAL(clicked()), this, SLOT(onWidgetClicked()));
	}
	if (preview) {
		connect(preview, SIGNAL(clicked()), this, SLOT(onPreviewClicked()));
	}
	if (switchToShape) {
		connect(switchToShape, SIGNAL(clicked()), this, SLOT(onShapeClicked()));
	}
}

QPushButton *ControlButtons::createButton(QString const &toolTip
		, QString const &icon)
{
	QPushButton *button = new QPushButton(QIcon(icon), QString());
	button->setToolTip(toolTip);
	NavigationMenuContent::appendButton(button);
	button->setContentsMargins(0,0,0,0);
	return button;
}

QPushButton *ControlButtons::createOnlyFor(bool shape, const QString &toolTip
		, const QString &icon)
{
	return shape == mForShapeEditor ? createButton(toolTip, icon) : NULL;
}

void ControlButtons::onIconClicked()
{
	emit iconClicked();
}

void ControlButtons::onIconAccepted()
{
	emit iconAccepted();
}

void ControlButtons::onIconRejected()
{
	emit iconRejected();
}

void ControlButtons::onSaveClicked()
{
	emit saveClicked();
}

void ControlButtons::onSaveAsImageClicked()
{
	emit saveAsImageClicked();
}

void ControlButtons::onSaveToDiskClicked()
{
	emit saveToDiskClicked();
}

void ControlButtons::onLoadFromDiskClicked()
{
	emit loadFromDiskClicked();
}

void ControlButtons::onWidgetClicked()
{
	emit widgetClicked();
}

void ControlButtons::onPreviewClicked()
{
	emit previewClicked();
}

void ControlButtons::onShapeClicked()
{
	emit shapeClicked();
}
