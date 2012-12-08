#pragma once

#include <QTreeWidget>
#include <QtCore/QHash>
#include <QtCore/QSettings>
#include <QtGui/QWidget>
#include <QtGui/QIcon>
#include <QtGui/QToolButton>
#include <QtGui/QComboBox>
#include <QtGui/QVBoxLayout>
#include "../pluginManager/proxyEditorManager.h"
#include "../../qrkernel/ids.h"
#include <QtGui/QLabel>

namespace  qReal{
namespace gui{
/// Class for representing editor elements.
class DraggableElement : public QWidget
{
	Q_OBJECT
public:
	DraggableElement(Id const &id, QString const &name
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
private:
	Id mId;
	QIcon mIcon;
	QString mText;
	QLabel *mLabel;
	EditorManagerInterface *mEditorManagerProxy;
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
};
}
}
