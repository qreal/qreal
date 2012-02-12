#include "nxtOSEKgenerator.h"

#include "funcOrientedGenerator.h"
#include "sequentialGenerator.h"

using namespace qReal;
using namespace robots::generator;

NxtOSEKgenerator::GeneratorType NxtOSEKgenerator::mCurGeneratorType = NxtOSEKgenerator::sequentialType;
//NxtOSEKgenerator::GeneratorType NxtOSEKgenerator::mCurGeneratorType = NxtOSEKgenerator::funcOrientedType;

NxtOSEKgenerator::GeneratorType NxtOSEKgenerator::getCurGeneratorType() {
	return mCurGeneratorType;
}

void NxtOSEKgenerator::setCurGeneratorType(NxtOSEKgenerator::GeneratorType type)
{
	mCurGeneratorType = type;
}

NxtOSEKgenerator::~NxtOSEKgenerator()
{
	if (mApi && mIsNeedToDeleteMApi)
		delete mApi;
}

NxtOSEKgenerator::NxtOSEKgenerator(
		qrRepo::RepoControlInterface &api,
		qReal::ErrorReporterInterface &errorReporter,
		QString const &destinationPath
		)
	: mDestinationPath(destinationPath)
	, mErrorReporter(errorReporter)
{
	mIsNeedToDeleteMApi = false;
	mApi = dynamic_cast<qrRepo::RepoApi *>(&api);

}

NxtOSEKgenerator::NxtOSEKgenerator(
		QString const &pathToRepo,
		qReal::ErrorReporterInterface &errorReporter,
		QString const &destinationPath
		)
	: mDestinationPath(SettingsManager::value("temp", "").toString())
	, mErrorReporter(errorReporter)
{
	Q_UNUSED(destinationPath)
	mIsNeedToDeleteMApi = true;
	mApi = new qrRepo::RepoApi(pathToRepo);
}

NxtOSEKgenerator* NxtOSEKgenerator::createGenerator(
	qrRepo::RepoControlInterface &api,
	qReal::ErrorReporterInterface &errorReporter,
	QString const &destinationPath
	)
{
	switch (mCurGeneratorType) {
		case funcOrientedType:
			return new FuncOrientedGenerator(api, errorReporter, destinationPath);
		case sequentialType:
			return new SequentialGenerator(api, errorReporter, destinationPath);
	}

	return new SequentialGenerator(api, errorReporter, destinationPath);
}

NxtOSEKgenerator* NxtOSEKgenerator::createGenerator(
	QString const &pathToRepo,
	qReal::ErrorReporterInterface &errorReporter,
	QString const &destinationPath
	)
{
	switch (mCurGeneratorType) {
		case funcOrientedType:
			return new FuncOrientedGenerator(pathToRepo, errorReporter, destinationPath);
		case sequentialType:
			return new SequentialGenerator(pathToRepo, errorReporter, destinationPath);
	}

	return new SequentialGenerator(pathToRepo, errorReporter, destinationPath);
}

void NxtOSEKgenerator::toggleGeneratorTypes()
{
	switch (mCurGeneratorType) {
		case funcOrientedType:
		{
			mCurGeneratorType = sequentialType;
			break;
		}

		case sequentialType:
		{
			mCurGeneratorType = funcOrientedType;
			break;
		}
	}
}
