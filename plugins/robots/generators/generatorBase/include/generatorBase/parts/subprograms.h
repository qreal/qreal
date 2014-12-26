#pragma once

#include <QtCore/QMap>
#include <QtCore/QSet>

#include <qrkernel/ids.h>
#include <qrrepo/repoApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "generatorBase/robotsGeneratorDeclSpec.h"
#include "generatorBase/templateParametrizedEntity.h"
#include "generatorBase/simpleGenerators/binding.h"

namespace generatorBase {

class ControlFlowGeneratorBase;

namespace parts {

// TODO: make this class more customizable for concrete generators

/// Incapsulates operations used for subprograms processing
class ROBOTS_GENERATOR_EXPORT Subprograms : public TemplateParametrizedEntity
{
public:
	Subprograms(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, QString const &pathToTemplates
			, simple::Binding::ConverterInterface const *nameNormalizer);

	~Subprograms() override;

	/// Must be called each time when visitor has found subprogram call
	/// @param logicalId Logical id of the block which calls subprogram
	void usageFound(qReal::Id const &logicalId);

	/// Starts subprograms code generation process
	bool generate(ControlFlowGeneratorBase *mainGenerator, QString const &indentString);

	/// Returns the subprograms forward declarations code. If generation was unsuccessfull returns an empty string.
	QString forwardDeclarations() const;

	/// Returns the subprograms code. If generation was unsuccessfull returns an empty string.
	QString implementations() const;

	void appendManualSubprogram(QString const &name, QString const &body);

private:
	bool checkIdentifier(QString const &identifier, QString const &rawName);

	void obtainCode(QMap<qReal::Id, QString> const &declarations
			, QMap<qReal::Id, QString> const &implementations);

	QString generateManualDeclarations() const;

	// TODO: this must be obtained via models or smth
	qReal::Id graphicalId(qReal::Id const &logicalId) const;

	qReal::Id firstToGenerate() const;

	QString readSubprogramTemplate(qReal::Id const &id, QString const &pathToTemplate);

	qrRepo::RepoApi const &mRepo;
	qReal::ErrorReporterInterface &mErrorReporter;
	simple::Binding::ConverterInterface const *mNameNormalizer;  // Takes ownership

	/// Stores all found by generator diagrams with subprograms implementation.
	/// Bool value means if key diagram was already processed and generated into
	/// the code.
	QMap<qReal::Id, bool> mDiscoveredSubprograms;

	QStringList mImplementationsCode;
	QStringList mForwardDeclarationsCode;

	QSet<QString> mUsedNames;

	QMap<QString, QString> mManualDeclarations;
};

}
}
