#pragma once

#include <QtCore/QList>
#include <QtWidgets/QTreeWidgetItem>

#include "../../../qrkernel/ids.h"

namespace qReal {

namespace gui {
class PaletteTreeWidget;
}

namespace commands {
class AbstractCommand;
}

namespace models {
class LogicalModelAssistApi;
class GraphicalModelAssistApi;

namespace details {

/// This class contains methods for processing all system explosion-based operations
/// from the model aspect
class Exploser : public QObject
{
	Q_OBJECT

public:
	explicit Exploser(LogicalModelAssistApi * const api);

	/// Returns current system suffix that would be added to every explosion target name
	static QString insideSuffix();

	/// Returns current system name of each group with reusable explosions in user palette
	static QString userGroupTitle();

	/// Returns current system description of each group with reusable explosions in user palette
	static QString userGroupDescription();

	/// Passed to this method palette tree will be automatically updated
	/// with actual reusable explosions information
	void addUserPalette(gui::PaletteTreeWidget * const tree, Id const &diagram);

	/// Returns all elements that have specified id as explosion target with hard nessesarity
	IdList elementsWithHardDependencyFrom(Id const &id) const;

	/// This method checks if explosion must be created immediately for given type
	/// and if yes returns type with explosion target. Otherwise returns Id().
	Id immediateExplosionTarget(Id const &id);

	/// Produces and returns command that binds either logical or graphical elements
	/// instances with explosion link and adds explosion instance into user blocks
	/// palette if nessesary.
	commands::AbstractCommand *addExplosionCommand(Id const &source, Id const &target
			, GraphicalModelAssistApi *graphicalApi = NULL);

	/// Produces and returns command that unbinds elements with explosion link and
	/// removes explosion instance from user blocks palette if nessesary.
	commands::AbstractCommand *removeExplosionCommand(Id const &source, Id const &target);

	/// Produces and returns command that creates element of target type and
	/// binds given element with new element with explosion link
	commands::AbstractCommand *createElementWithIncommingExplosionCommand(Id const &source
			, Id const &targetType, GraphicalModelAssistApi *graphicalApi);

	/// Produces and returns a command that performs the renaming of the whole
	/// explosions  hierarchy and binded palette items
	/// @param oneOfIds The id of one of the elements belonging to the hierarchy
	/// @param newNames The name which will bew assigned to all the elements in heirarchy
	commands::AbstractCommand *renameCommands(Id const &oneOfIds, QString const &newNames);

public slots:
	void refreshAllPalettes();

signals:
	/// Emitted each time when automatically created explosion target was removed
	void explosionTargetRemoved();

private:
	/// This method takes one of the elements of the diagram and tries
	/// to move through its explosions hiererchy to the root.
	/// In case of cyclic explosions the behaviour is undifined
	/// (most probably you will get infinite cycle)
	Id explosionsRoot(Id const &id) const;

	/// Returns the hierarchy tree of binded with explosion link elements;
	/// if elements are binded with cyclic explosions the behaviour is undefined
	/// @param oneOfIds belonging to this hierarchy.
	IdList explosionsHierarchy(Id const &oneOfIds) const;

	/// Recursively travels through the explosions tree and puts all elements
	/// ids into @param targetIds
	void explosionsHierarchyPrivate(Id const &currentId, IdList &targetIds) const;

	void refreshPalette(gui::PaletteTreeWidget * const tree, Id const &diagram);

	LogicalModelAssistApi *mApi; // Does not take ownership
	QMap<Id, gui::PaletteTreeWidget *> mUserPalettes;
};

}
}
}
