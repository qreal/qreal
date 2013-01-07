#pragma once

#include <QtCore/QObject>
#include <QtGui/QBoxLayout>
#include <QtGui/QPushButton>

#include "../../../qrutils/navigationUtils/navigationMenu.h"

namespace qReal
{
namespace elementEdit
{

class ControlButtons : public QObject, public navigation::NavigationMenuContent
{
	Q_OBJECT

public:
	ControlButtons(bool forShapeEditor, bool isIconEditor);

signals:
	void iconClicked();
	void iconAccepted();
	void iconRejected();
	void saveClicked();
	void saveAsImageClicked();
	void saveToDiskClicked();
	void loadFromDiskClicked();
	void widgetClicked();
	void previewClicked();
	void shapeClicked();

private slots:
	void onIconClicked();
	void onIconAccepted();
	void onIconRejected();
	void onSaveClicked();
	void onSaveAsImageClicked();
	void onSaveToDiskClicked();
	void onLoadFromDiskClicked();
	void onWidgetClicked();
	void onPreviewClicked();
	void onShapeClicked();

private:
	QPushButton *createButton(QString const &toolTip
		, QString const &icon);
	QPushButton *createOnlyFor(bool shape, QString const &toolTip
		, QString const &icon);

	bool mForShapeEditor;
	QBoxLayout *mLayout;
};

}
}
