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
	void createGoToSubmenu(QMenu * const goToMenu, QString const &name, qReal::IdList const &ids) const;
	void createAddExplosionMenu(Element const * const element
			, QMenu &contextMenu, QString const &menuName
			, QList<Explosion> const &explosions
			, qReal::IdList const &alreadyConnectedElements
			, char const *slot) const;

	void createRemoveExplosionMenu(Element const * const element
			, QMenu &contextMenu, QString const &menuName
			, qReal::IdList const &outgoingConnections, qReal::IdList const &incomingConnections
			, char const *slot) const;

	MainWindow *mMainWindow; // Doesn`t take ownership
	LogicalModelAssistInterface *mLogicalApi; // Doesn`t take ownership
};

}
