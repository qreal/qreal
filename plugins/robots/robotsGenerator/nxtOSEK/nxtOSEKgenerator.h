#pragma once

#include "../../../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include "../../../qrrepo/repoApi.h"
#include "../../../../qrkernel/settingsManager.h"

namespace robots {
namespace generator {

/// Class for generate a nxtOSEK code from Robot Language Diagram.
class NxtOSEKgenerator {
public:
	enum GeneratorType {
		funcOrientedType,
		sequentialType
	};

	static GeneratorType getCurGeneratorType();
	static void setCurGeneratorType(GeneratorType type);

	virtual ~NxtOSEKgenerator();

	/// main method that starts a code generation.
	virtual qReal::ErrorReporterInterface &generate() = 0;

	static NxtOSEKgenerator* createGenerator(
				qrRepo::RepoControlInterface &api,
				qReal::ErrorReporterInterface &errorReporter,
				QString const &destinationPath = ""
				);

	static NxtOSEKgenerator* createGenerator(
				QString const &pathToRepo,
				qReal::ErrorReporterInterface &errorReporter,
				QString const &destinationPath = ""
				);
	
	/// toggles generator types that will be produced by 'createGenerator' method
	/// funcOrientedType -> sequentialType
	/// sequentialType -> funcOrientedType
	static void toggleGeneratorTypes();


protected:
	static GeneratorType mCurGeneratorType;

	NxtOSEKgenerator(
			qrRepo::RepoControlInterface &api,
			qReal::ErrorReporterInterface &errorReporter,
			QString const &destinationPath = ""
			);


	NxtOSEKgenerator(
			QString const &pathToRepo,
			qReal::ErrorReporterInterface &errorReporter,
			QString const &destinationPath = ""
			);


	qrRepo::RepoApi *mApi;
	bool mIsNeedToDeleteMApi;
	QString mDestinationPath;

	qReal::ErrorReporterInterface &mErrorReporter;
};
}
}
