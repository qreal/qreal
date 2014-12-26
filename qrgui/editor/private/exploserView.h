#pragma once

#include <QtWidgets/QMenu>

#include <qrkernel/ids.h>

namespace qReal {

class Element;
class Explosion;
class Controller;
class SceneCustomizer;
class EditorManagerInterface;

namespace commands {
class AbstractCommand;
}

namespace models {
class Models;
class LogicalModelAssistApi;
class GraphicalModelAssistApi;
class Exploser;
}

namespace view {
namespace details {

/// This class contains methods for processing all system explosion-based operations
/// from the view aspect
class ExploserView : public QObject
{
	Q_OBJECT

public:
	explicit ExploserView(models::Models const &models
			, Controller &controller
			, SceneCustomizer const &customizer
			, QObject *parent = 0);

	/// Adds to @see contextMenu actions and submenus related to explosions
	void createConnectionSubmenus(QMenu &contextMenu, Element const * const element) const;

	/// Activates binded with explosion element or creates default explosed element
	void handleDoubleClick(Id const &id);

	/// Adds commands of explosion creation to given element creation command
	void handleCreationWithExplosion(commands::AbstractCommand *createCommand
			, Id const &source, Id const &target);

signals:
	/// Activates first binded with explosion link graphical instance of the element
	/// with given @arg id.
	void goTo(Id const &id);

	/// Emitted when palette contents could change and thus must be reread.
	void refreshPalette();

	/// Emitted when user requested to change some element`s graphical representation.
	void openShapeEditor(Id const &id
		, QString const &propertyValue
		/// @todo: whan passing it by reference the build on travis fails
		, EditorManagerInterface const *editorManagerProxy
		, bool useTypedPorts);

	/// Emitted each time when scene must invoke ExpandCommand to the child instance with the given id.
	void expandElement(Id const &element);

private slots:
	void addExplosionActionTriggered();
	void goToActionTriggered();
	void removeExplosionActionTriggered();
	void expandExplosionActionTriggered();
	void changePropertiesActionTriggered();
	void changeAppearanceActionTriggered();
	void addElementToPaletteActionTriggered();

private:
	void createAddExplosionMenu(Element const * const element
			, QMenu &contextMenu, QList<Explosion> const &explosions
			, qReal::Id const &alreadyConnectedElement) const;

	void createRemoveExplosionMenu(Element const * const element
			, QMenu &contextMenu, qReal::Id const &outgoingConnection) const;

	void createExpandAction(Element const * const element
			, QMenu &contextMenu, qReal::Id const &alreadyConnectedElement) const;

	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	models::Exploser &mExploser;
	Controller &mController;
	SceneCustomizer const &mCustomizer;
};

}
}
}
