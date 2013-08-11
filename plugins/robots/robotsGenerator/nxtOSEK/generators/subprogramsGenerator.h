#pragma once

#include <QtCore/QMap>
#include <QtCore/QSet>

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

	/// Starts code generation process
	/// @warning this will modify main generator`s currentGenerator() return result
	bool generate();

	/// Returns generation process result. If it was unsuccessfull returns empty list
	QList<SmartLine> &generatedCode();

private:
	bool checkIdentifier(QString const &identificator, QString const &rawName);

	void mergeCode(QMap<qReal::Id, QList<SmartLine> > const &declarations
			, QMap<qReal::Id, QList<SmartLine> > const &implementations);

	// TODO: this must be obtained via models!
	qReal::Id graphicalId(qReal::Id const &logicalId) const;

	qReal::Id firstToGenerate() const;

	NxtOSEKRobotGenerator *mMainGenerator;

	/// Stores all found by generator diagrams with subprograms implementation.
	/// Bool value means if key diagram was already processed and generated into
	/// the code.
	QMap<qReal::Id, bool> mDiscoveredSubprograms;

	QList<SmartLine> mGeneratedCode;

	QSet<QString> mUsedNames;

};

}
}
