/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtWidgets/QTreeWidget>
#include <QtCore/QHash>
#include <QtCore/QThread>
#include <QtCore/QSettings>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>

#include <qrkernel/ids.h>

#include "mainWindow/palette/paletteElement.h"
#include "mainWindow/mainWindow.h"
#include "plugins/pluginManager/proxyEditorManager.h"

namespace qReal {
namespace gui {

/// Class for representing editor element on palette.
class DraggableElement : public QWidget
{
	Q_OBJECT

public:
	DraggableElement(MainWindow &mainWindow
		, const PaletteElement &paletteElement
		, bool iconsOnly
		, const EditorManagerInterface &editorManagerProxy
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

	/// Returns a mime data instance binded with object during drag-and-drop.
	QMimeData *mimeData(const Id &elementId) const;

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
#ifdef Q_OS_WIN
	/// This class performs win8 drag manager hack for workaround of
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

	const PaletteElement mData;
	QLabel *mLabel;
	const EditorManagerInterface &mEditorManagerProxy;  // Does not have ownership.
	MainWindow &mMainWindow;
	Id mDeletedElementId;
	bool mIsRootDiagramNode;
};

}
}
