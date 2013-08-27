#pragma once

#include <QtWidgets/QTreeWidget>
#include <QtCore/QHash>
#include <QtCore/QSettings>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>

#include <qrkernel/ids.h>

#include "mainwindow/palette/paletteElement.h"
#include "mainwindow/mainWindow.h"
#include "pluginManager/proxyEditorManager.h"

namespace qReal {
namespace gui {

/// Class for representing editor element on palette.
class DraggableElement : public QWidget
{
	Q_OBJECT

public:
	DraggableElement(MainWindow &mainWindow
		, PaletteElement const &paletteElement
		, bool iconsOnly
		, EditorManagerInterface &editorManagerProxy
		, QWidget *parent = NULL
		);

	/// Icon of an element on palette.
	QIcon icon() const;

	/// Name of an element on palette.
	QString text() const;

	/// Id of an element.
	Id id() const;

	/// The id of the element which will be a target for an explosion of
	/// newly created element
	Id explosionTarget() const;

	void setIconSize(int size);

	QSize iconsPreferredSize() const;

private slots:
	void changePropertiesPaletteActionTriggered();
	void changeAppearancePaletteActionTriggered();
	void deleteElementPaletteActionTriggered();
	void deleteElement();
	void checkElementForRootDiagramNode();

protected:
	virtual bool event(QEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);

private:
	void checkElementForChildren();

	PaletteElement const mData;
	QLabel *mLabel;
	EditorManagerInterface &mEditorManagerProxy;  // Does not have ownership.
	MainWindow &mMainWindow;
	Id mDeletedElementId;
	bool mIsRootDiagramNode;
};

}
}
