#pragma once

#include <QtCore/QObject>

#include <qrrepo/repoApi.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "generatorCustomizer.h"
#include "controlFlowGeneratorBase.h"
#include "templateParametrizedEntity.h"

namespace qReal {
namespace robots {
namespace generators {

class MasterGeneratorBase : public QObject, public TemplateParametrizedEntity
{
public:
	MasterGeneratorBase(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter
			, Id const &diagramId);

	/// @warning This method mustn`t be called from constructor. Otherwise
	/// there will be segfault due to pure virtual method call in constructor
	virtual void initialize();

	/// Starts code generation process. Returns path to file with generated code
	/// if it was successfull and an empty string otherwise.
	virtual QString generate();

protected:
	virtual GeneratorCustomizer *createCustomizer() = 0;
	virtual QString pathToGenerate() = 0;

	virtual void beforeGeneration();
	virtual void processGeneratedCode(QString &generatedCode);
	virtual void afterGeneration();

	void outputCode(QString const &path, QString const &code);

	qrRepo::RepoApi const &mRepo;
	ErrorReporterInterface &mErrorReporter;
	Id const mDiagram;
	GeneratorCustomizer *mCustomizer;
	ControlFlowGeneratorBase *mReadableControlFlowGenerator;
};

}
}
}

