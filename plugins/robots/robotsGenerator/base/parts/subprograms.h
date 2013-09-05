#pragma once

#include <QtCore/QMap>
#include <QtCore/QSet>

#include <qrkernel/ids.h>

namespace qReal {
namespace robots {
namespace generators {
namespace parts {

/// Incapsulates operations used for subprograms processing
class SubprogramsGenerator
{
public:
	explicit SubprogramsGenerator(NxtOSEKRobotGenerator * const nxtGenerator);

	/// Must be called each time when visitor has found subprogram call
	/// @param logicalId Logical id of the block which calls subprogram
	void usageFound(Id const &logicalId);

	/// Starts code generation process
	/// @warning this will modify main generator`s currentGenerator() return result
	bool generate();

	/// Returns generation process result. If it was unsuccessfull returns empty list
	QList<SmartLine_old> &generatedCode();

private:
	bool checkIdentifier(QString const &identificator, QString const &rawName);

	void mergeCode(QMap<Id, QList<SmartLine_old> > const &declarations
			, QMap<Id, QList<SmartLine_old> > const &implementations);

	// TODO: this must be obtained via models!
	Id graphicalId(Id const &logicalId) const;

	Id firstToGenerate() const;

	NxtOSEKRobotGenerator *mMainGenerator;

	/// Stores all found by generator diagrams with subprograms implementation.
	/// Bool value means if key diagram was already processed and generated into
	/// the code.
	QMap<Id, bool> mDiscoveredSubprograms;

	QList<SmartLine_old> mGeneratedCode;

	QSet<QString> mUsedNames;

};

}
}
}
}
