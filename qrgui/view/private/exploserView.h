#pragma once

#include <QtWidgets/QMenu>

#include "models/logicalModelAssistApi.h"
#include "models/graphicalModelAssistApi.h"

namespace qReal {

class MainWindow;

namespace view {
namespace details {

/// This class contains methods for processing all system explosion-based operations
/// from the view aspect
class ExploserView : public QObject
{
	Q_OBJECT

public:
	ExploserView(MainWindow * const mainWindow
			, models::LogicalModelAssistApi * const logicalApi
			, models::GraphicalModelAssistApi * const graphicalApi
			, QObject *parent = 0);

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

	MainWindow *mMainWindow; // Doesn`t take ownership
	models::LogicalModelAssistApi *mLogicalApi; // Doesn`t take ownership
	models::GraphicalModelAssistApi *mGraphicalApi; // Doesn`t take ownership
};

}
}
}
