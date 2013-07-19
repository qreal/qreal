#pragma once

#include <QtCore/QList>
#include <QtWidgets/QTreeWidgetItem>

#include "../../../qrkernel/ids.h"

namespace qReal {

namespace gui {
class PaletteTreeWidget;
}

namespace models {
class LogicalModelAssistApi;

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

	void addUserPalette(gui::PaletteTreeWidget * const tree, Id const &diagram);

	/// Returns all elements that have specified id as explosion target with hard nessesarity
	IdList elementsWithHardDependencyFrom(Id const &id) const;

	/// Puts into  explosion tree`s elements ids with
	/// @param oneOfIds belonging to this hierarchy.
	IdList explosionsHierarchy(Id const &oneOfIds) const;

	/// This method checks if explosion must be created immediately for given type
	/// and if yes returns type with explosion target. Otherwise returns Id().
	Id explosionTarget(Id const &id);

	/// Binds elements with explosion link; adds explosion instance into
	/// palette if nessesary.
	void addExplosion(Id const &source, Id const &target);

	/// Performs the renaming of the whole explosions hierarchy and
	/// binded palette items
	/// @param oneOfIds The id of one of the elements belonging to the hierarchy
	void rename(Id const &oneOfIds);

private:
	void refreshAllPalettes();
	void refreshPalette(gui::PaletteTreeWidget * const tree, Id const &diagram);

	/// This method takes one of the elements of the diagram and tries
	/// to move through its explosions hiererchy to the root.
	/// In case of cyclic explosions the behaviour is undifined
	/// (most probably you will get infinite cycle)
	Id explosionsRoot(Id const &id) const;

	/// Recursively travels through the explosions tree and puts all elements
	/// ids into @param targetIds
	void explosionsHierarchyPrivate(Id const &currentId, IdList &targetIds) const;

	void reuseExplosion(Id const &source, bool invokeEditTrigger);
	QTreeWidgetItem *userGroup(Id const &diagram) const;

	LogicalModelAssistApi *mApi; // Does not take ownership
	QMap<Id, gui::PaletteTreeWidget *> mUserPalettes;
};

}
}
}
