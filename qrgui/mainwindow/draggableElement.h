#pragma once

#include <QTreeWidget>
#include <QtCore/QHash>
#include <QtCore/QSettings>
#include <QtGui/QWidget>
#include <QtGui/QIcon>
#include <QtGui/QToolButton>
#include <QtGui/QComboBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include "mainWindow.h"
#include "../pluginManager/proxyEditorManager.h"
#include "../../qrkernel/ids.h"

namespace  qReal{
namespace gui{
/// Class for representing editor elements.
class DraggableElement : public QWidget
{
	Q_OBJECT
public:
	DraggableElement(MainWindow *mainWindow, Id const &id, QString const &name
					 , QString const &description
					 , QIcon const &icon, bool iconsOnly, QWidget *parent = NULL);

	QIcon icon() const
	{
		return mIcon;
	}

	QString text() const
	{
		return mText;
	}

	Id id() const
	{
		return mId;
	}

	void setIconSize(int size);
	void setEditorManagerProxy(EditorManagerInterface *editorManagerProxy);
private slots:
	void changePropertiesPaletteActionTriggered();
	void changeAppearancePaletteActionTriggered();
	void deleteElementPaletteActionTriggered();
	void deleteElement();
	void checkElementForRootDiagramNode();
	void checkElementForChildren();
private:
	Id mId;
	QIcon mIcon;
	QString mText;
	QLabel *mLabel;
	EditorManagerInterface *mEditorManagerProxy;
	MainWindow *mMainWindow;
	Id mDeletedElementId;
	bool mIsRootDiagramNode;
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
};
}
}
