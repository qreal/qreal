#pragma once

#include "../../mainwindow/errorReporter.h"
#include "../../../qrrepo/repoApi.h"

namespace qReal {
namespace generators {
namespace nxtOSEK {

//! Class for generate a nxtOSEK code from Robot Language Diagram.

class NxtOSEKgenerator {
public:
	explicit NxtOSEKgenerator(qrRepo::RepoControlInterface &api, QString const &destinationPath = "")
			:mDestinationPath(destinationPath) {
		mIsNeedToDeleteMApi = false;
		mApi = dynamic_cast<qrRepo::RepoApi *>(&api);

	}

	explicit NxtOSEKgenerator(QString const &pathToRepo, QString const &destinationPath = "")
			:mDestinationPath(SettingsManager::value("temp", "").toString()) {
		Q_UNUSED(destinationPath)
		mIsNeedToDeleteMApi = true;
		mApi = new qrRepo::RepoApi(pathToRepo);
	}

	virtual ~NxtOSEKgenerator() {
		if (mApi && mIsNeedToDeleteMApi)
			delete mApi;
	}

	virtual gui::ErrorReporter &generate() = 0;

protected:
	qrRepo::RepoApi *mApi;
	bool mIsNeedToDeleteMApi;
	QString mDestinationPath;

	gui::ErrorReporter mErrorReporter;
};
}
}
}
