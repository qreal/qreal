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
		, QWidget *parent = nullptr
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
	void openGenerationToolsWindowActionTriggered();
	void deleteElement();
	void checkElementForRootDiagramNode();

protected:
	virtual bool event(QEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);

private:
#ifdef Q_OS_WIN
	/// This class performs win8 drag manager hack for workarround of
	/// https://github.com/qreal/qreal/issues/1014
	class HackTouchDragThread : public QThread
	{
	public:
		explicit HackTouchDragThread(QObject *parent = 0);

		static void simulateSystemPress();
		static void simulateSystemMove();
		static void simulateSystemRelease();

	protected:
		virtual void run();
	};
#endif

	void checkElementForChildren();
	void hackTouchDrag();

	PaletteElement const mData;
	QLabel *mLabel;
	EditorManagerInterface &mEditorManagerProxy;  // Does not have ownership.
	MainWindow &mMainWindow;
	Id mDeletedElementId;
	bool mIsRootDiagramNode;
};

}
}
