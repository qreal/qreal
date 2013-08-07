#pragma once

#include <QtCore/QMap>

#include "../smartLine.h"

namespace robots {
namespace generator {

class NxtOSEKRobotGenerator;

/// Incapsulates operations used for subprograms processing
class SubprogramsGenerator
{
public:
	explicit SubprogramsGenerator(NxtOSEKRobotGenerator * const nxtGenerator);

	/// Must be called each time when visitor has found subprogram call
	/// @param logicalId Logical id of the block which calls subprogram
	void usageFound(qReal::Id const &logicalId);

	QList<SmartLine> generateCode();

private:
	qReal::Id firstToGenerate() const;

	NxtOSEKRobotGenerator *mNxtGen;

	/// Stores all found by generator diagrams with subprograms implementation.
	/// Bool value means if key diagram was already processed and generated into
	/// the code.
	QMap<qReal::Id, bool> mDiscoveredSubprograms;

	QMap<qReal::Id, QList<SmartLine> > mDeclarations;
	QMap<qReal::Id, QList<SmartLine> > mImplementations;
};

}
}
