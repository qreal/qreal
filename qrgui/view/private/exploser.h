#pragma once

#include <QtWidgets/QMenu>

#include "../../models/logicalModelAssistApi.h"

namespace qReal {

class MainWindow;

/// Incapsulates all operations related to explosion links
class Exploser : public QObject
{
	Q_OBJECT

public:
	Exploser(MainWindow * const mainWindow
			, LogicalModelAssistInterface * const logicalApi
			, QObject *parent = 0);

	void createConnectionSubmenus(QMenu &contextMenu, Element const * const element) const;

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
			, QMenu &contextMenu, QString const &menuName
			, qReal::Id const &outgoingConnection, qReal::IdList const &incomingConnections
			, char const *slot) const;

	MainWindow *mMainWindow; // Doesn`t take ownership
	LogicalModelAssistInterface *mLogicalApi; // Doesn`t take ownership
};

}
