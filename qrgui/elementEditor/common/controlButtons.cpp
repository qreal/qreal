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
		connect(acceptIcon, SIGNAL(clicked()), this, SIGNAL(iconAccepted()));
		connect(rejectIcon, SIGNAL(clicked()), this, SIGNAL(iconRejected()));
	} else {
		QPushButton *icon = createButton(tr("Icon...")
			, ":/icons/widgetsEditor/icon.png");
		connect(icon, SIGNAL(clicked()), this, SIGNAL(iconClicked()));
	}
	QPushButton *save = createButton(tr("Save")
		, ":/icons/widgetsEditor/save.png");
	QPushButton *saveAsImage = createOnlyFor(true, tr("Save as image...")
		, ":/icons/widgetsEditor/saveAsImage.png");
	QPushButton *saveToDisk = createButton(tr("Save to disk as XML...")
		, ":/icons/widgetsEditor/saveToDisk.png");
	QPushButton *loadFromDisk = createButton(tr("Load from disk...")
		, ":/icons/widgetsEditor/loadFromDisk.png");
	QPushButton *switchToWidgets = createOnlyFor(true, tr("Switch to widgets")
		, ":/icons/widgetsEditor/widgetIcon.png");
	QPushButton *preview = createOnlyFor(false, tr("Preview...")
		, ":/icons/preview.png");
	if (!isIconEditor) {
		QPushButton *outerBindings = createOnlyFor(false, tr("Outer bindings...")
			, ":/icons/widgetsEditor/outerBindings.png");
		if (outerBindings) {
			connect(outerBindings, SIGNAL(clicked()), this, SIGNAL(outerBindingsClicked()));
		}
	}
	QPushButton *switchToShape = createOnlyFor(false, tr("Switch to shape")
		, ":/icons/widgetsEditor/shapeIcon.png");

	connect(save, SIGNAL(clicked()), this, SIGNAL(saveClicked()));
	if (saveAsImage) {
		connect(saveAsImage, SIGNAL(clicked()), this, SIGNAL(saveAsImageClicked()));
	}
	connect(saveToDisk, SIGNAL(clicked()), this, SIGNAL(saveToDiskClicked()));
	connect(loadFromDisk, SIGNAL(clicked()), this, SIGNAL(loadFromDiskClicked()));
	if (switchToWidgets) {
		connect(switchToWidgets, SIGNAL(clicked()), this, SIGNAL(widgetClicked()));
	}
	if (preview) {
		connect(preview, SIGNAL(clicked()), this, SIGNAL(previewClicked()));
	}
	if (switchToShape) {
		connect(switchToShape, SIGNAL(clicked()), this, SIGNAL(shapeClicked()));
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
