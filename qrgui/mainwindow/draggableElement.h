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

#include "mainWindow.h"
#include "../pluginManager/proxyEditorManager.h"
#include "../../qrkernel/ids.h"

namespace qReal{
namespace gui{

/// Class for representing editor element on palette.
class DraggableElement : public QWidget
{
	Q_OBJECT

public:
	DraggableElement(MainWindow &mainWindow
		, Id const &id
		, QString const &name
		, QString const &description
		, QIcon const &icon
		, QSize const &preferredSize
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

	void setIconSize(int size);

	QSize iconsPreferredSize() const;

private slots:
	void changePropertiesPaletteActionTriggered();
	void changeAppearancePaletteActionTriggered();
	void deleteElementPaletteActionTriggered();
	void deleteElement();
	void checkElementForRootDiagramNode();

private:
	Id mId;
	QIcon mIcon;
	QSize mPreferredSize;
	QString mText;
	QLabel *mLabel;
	EditorManagerInterface &mEditorManagerProxy;  // Does not have ownership.
	MainWindow &mMainWindow;
	Id mDeletedElementId;
	bool mIsRootDiagramNode;

	virtual void mousePressEvent(QMouseEvent *event);

	void checkElementForChildren();
};
}
}
