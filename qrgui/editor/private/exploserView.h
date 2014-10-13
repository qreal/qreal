#pragma once

#include <QtWidgets/QMenu>

#include <qrkernel/ids.h>

namespace qReal {

class Element;
class Explosion;
class Controller;

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
	explicit ExploserView(models::Models const &models, Controller &controller, QObject *parent = 0);

	/// Adds to @see contextMenu actions and submenus related to explosions
	void createConnectionSubmenus(QMenu &contextMenu, Element const * const element) const;

	/// Activates binded with explosion element or creates default explosed element
	void handleDoubleClick(Id const &id);

	/// Adds commands of explosion creation to given element creation command
	void handleCreationWithExplosion(commands::AbstractCommand *createCommand
			, Id const &source, Id const &target);

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

	/// Activates first binded with explosion link graphical instance of the element
	/// with given @see id
	void goTo(Id const &id);

	models::LogicalModelAssistApi &mLogicalApi;
	models::GraphicalModelAssistApi &mGraphicalApi;
	models::Exploser &mExploser;
	Controller &mController;
};

}
}
}
