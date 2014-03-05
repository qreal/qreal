#pragma once

#include <QtCore/QObject>

#include <qrrepo/repoApi.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "robotsGeneratorDeclSpec.h"
#include "generatorCustomizer.h"
#include "controlFlowGeneratorBase.h"
#include "templateParametrizedEntity.h"

namespace qReal {
namespace robots {
namespace generators {

/// The main class for every code generator. Produces nessesary components and
/// controls their work. Must be reimplemented in each concrete generator.
class ROBOTS_GENERATOR_EXPORT MasterGeneratorBase : public QObject, public TemplateParametrizedEntity
{
	Q_OBJECT

public:
	MasterGeneratorBase(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter
			, Id const &diagramId);

	void setProjectDir(QFileInfo const &fileInfo);

	/// @warning This method mustn`t be called from constructor. Otherwise
	/// there will be segfault due to pure virtual method call in constructor
	virtual void initialize();

	/// Starts code generation process. Returns path to file with generated code
	/// if it was successfull and an empty string otherwise.
	virtual QString generate();

protected:
	virtual GeneratorCustomizer *createCustomizer() = 0;

	/// Implementation must return a path to a file where all generated code
	/// will be written. Called on the last stage of the generation process
	/// so concrete generators have time to 'prepare' this path
	virtual QString targetPath() = 0;

	virtual bool supportsGotoGeneration() const = 0;

	virtual void beforeGeneration();
	virtual void processGeneratedCode(QString &generatedCode);
	virtual void afterGeneration();

	void outputCode(QString const &path, QString const &code);

	qrRepo::RepoApi const &mRepo;
	ErrorReporterInterface &mErrorReporter;
	Id const mDiagram;
	GeneratorCustomizer *mCustomizer;
	ControlFlowGeneratorBase *mReadableControlFlowGenerator;  // Takes ownership
	ControlFlowGeneratorBase *mGotoControlFlowGenerator;  // Takes ownership
	QString mProjectName;
	QString mProjectDir;
	int mCurInitialNodeNumber;
};

}
}
}

