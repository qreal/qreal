#pragma once

#include <QtCore/QMap>
#include <QtCore/QSet>

#include <qrkernel/ids.h>
#include <qrrepo/repoApi.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "generatorBase/robotsGeneratorDeclSpec.h"
#include "generatorBase/templateParametrizedEntity.h"
#include "generatorBase/simpleGenerators/binding.h"

namespace qReal {
namespace robots {
namespace generators {
class ControlFlowGeneratorBase;

namespace parts {

// TODO: make this class more customizable for concrete generators

/// Incapsulates operations used for subprograms processing
class ROBOTS_GENERATOR_EXPORT Subprograms : public TemplateParametrizedEntity
{
public:
	Subprograms(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter
			, QString const &pathToTemplates
			, simple::Binding::ConverterInterface const *nameNormalizer);

	virtual ~Subprograms();

	/// Must be called each time when visitor has found subprogram call
	/// @param logicalId Logical id of the block which calls subprogram
	void usageFound(Id const &logicalId);

	/// Starts subprograms code generation process
	bool generate(ControlFlowGeneratorBase *mainGenerator);

	/// Returns the generation process result. If it was unsuccessfull returns an empty string.
	QString generatedCode() const;

	void appendManualSubprogram(QString const &name, QString const &body);

private:
	bool checkIdentifier(QString const &identifier, QString const &rawName);

	void mergeCode(QMap<Id, QString> const &declarations
			, QMap<Id, QString> const &implementations);

	QString generateManualDeclarations() const;

	// TODO: this must be obtained via models or smth
	Id graphicalId(Id const &logicalId) const;

	Id firstToGenerate() const;

	QString readSubprogramTemplate(Id const &id, QString const &pathToTemplate);

	qrRepo::RepoApi const &mRepo;
	ErrorReporterInterface &mErrorReporter;
	simple::Binding::ConverterInterface const *mNameNormalizer;  // Takes ownership

	/// Stores all found by generator diagrams with subprograms implementation.
	/// Bool value means if key diagram was already processed and generated into
	/// the code.
	QMap<Id, bool> mDiscoveredSubprograms;

	QStringList mGeneratedCode;

	QSet<QString> mUsedNames;

	QMap<QString, QString> mManualDeclarations;
};

}
}
}
}
