#pragma once

#include <QtWidgets/QMenu>

#include "../../models/logicalModelAssistApi.h"
#include "../../models/graphicalModelAssistApi.h"

namespace qReal {

class MainWindow;

/// Incapsulates all operations related to explosion links
class Exploser : public QObject
{
	Q_OBJECT

public:
	Exploser(MainWindow * const mainWindow
			, LogicalModelAssistInterface * const logicalApi
			, GraphicalModelAssistInterface * const graphicalApi
			, QObject *parent = 0);

	/// Adds to @see contextMenu actions and submenus related to explosions
	void createConnectionSubmenus(QMenu &contextMenu, Element const * const element) const;

	/// Activates binded with explosion element or creates default explosed element
	void handleDoubleClick(Id const &id);

private slots:
	void addExplosionActionTriggered();
	void goToActionTriggered();
	void removeExplosionActionTriggered();

private:
	void createAddExplosionMenu(Element const * const element
			, QMenu &contextMenu, QList<Explosion> const &explosions
			, qReal::Id const &alreadyConnectedElement
			, char const *slot) const;

	void createRemoveExplosionMenu(Element const * const element
			, QMenu &contextMenu, qReal::Id const &outgoingConnection
			, char const *slot) const;

	/// Activates first binded with explosion link graphical instance of the element
	/// with given @see id
	void goTo(Id const &id);

	Id createElementWithIncommingExplosion(Id const &source, Id const &targetType);

	MainWindow *mMainWindow; // Doesn`t take ownership
	LogicalModelAssistInterface *mLogicalApi; // Doesn`t take ownership
	GraphicalModelAssistInterface *mGraphicalApi; // Doesn`t take ownership
};

}
