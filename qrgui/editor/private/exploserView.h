/* Copyright 2013-2016 Dmitry Mordvinov
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

#include <QtWidgets/QMenu>

#include <qrkernel/ids.h>

namespace qReal {

namespace models {
class Models;
class LogicalModelAssistApi;
class GraphicalModelAssistApi;
class Exploser;
}

class Explosion;
class Controller;
class EditorManagerInterface;

namespace commands {
class AbstractCommand;
}

namespace gui {
namespace editor {

class SceneCustomizer;
class Element;

namespace view {
namespace details {

/// This class contains methods for processing all system explosion-based operations
/// from the view aspect
class ExploserView : public QObject
{
	Q_OBJECT

public:
	explicit ExploserView(const models::Models &models
			, Controller &controller
			, const SceneCustomizer &customizer
			, QObject *parent = 0);

	/// Adds to @see contextMenu actions and submenus related to explosions
	void createConnectionSubmenus(QMenu &contextMenu, const Element * const element) const;

	/// Activates binded with explosion element or creates default explosed element
	void handleDoubleClick(const Id &id);

signals:
	/// Activates first binded with explosion link graphical instance of the element
	/// with given @arg id.
	void goTo(const Id &id);

	/// Emitted when palette contents could change and thus must be reread.
	void refreshPalette();

	/// Emitted when user requested to change some element`s graphical representation.
	void openShapeEditor(const Id &id
		, const QString &propertyValue
		/// @todo: whan passing it by reference the build on travis fails
		, const EditorManagerInterface *editorManagerProxy
		, bool useTypedPorts);

	/// Emitted each time when scene must invoke ExpandCommand to the child instance with the given id.
	void expandElement(const Id &element);

private slots:
	void addExplosionActionTriggered();
	void goToActionTriggered();
	void removeExplosionActionTriggered();
	void expandExplosionActionTriggered();
	void changePropertiesActionTriggered();
	void changeAppearanceActionTriggered();
	void addElementToPaletteActionTriggered();

private:
	void createAddExplosionMenu(const Element * const element
			, QMenu &contextMenu, QList<Explosion> const &explosions
			, const Id &alreadyConnectedElement) const;

	void createRemoveExplosionMenu(const Element * const element
			, QMenu &contextMenu, const Id &outgoingConnection) const;

	void createExpandAction(const Element * const element
			, QMenu &contextMenu, const Id &alreadyConnectedElement) const;

	const models::Models &mModels;
	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	models::Exploser &mExploser;
	Controller &mController;
	const SceneCustomizer &mCustomizer;
};

}
}
}
}
}
