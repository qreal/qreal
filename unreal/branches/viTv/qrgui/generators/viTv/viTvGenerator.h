#pragma once

#include <QtCore/QString>
#include <QtCore/QPoint>

#include "../../kernel/ids.h"
#include "../../mainwindow/errorReporter.h"

namespace qrRepo {
	class RepoApi;
}

namespace utils {
	class OutFile;
}

namespace qReal {

	namespace generators {

		class ViTvGenerator
		{
		public:
			explicit ViTvGenerator(qrRepo::RepoApi const &api);

			gui::ErrorReporter generate();
		private:
			struct SupportState
			{
				QString stateId;
				QString direction;
				QString endEvent;
				QString sourceState;
				QString targetState;
				QString minDistance;
			};

			void generateDiagram(Id const &id);
			void generateHeader(QString const &outputDirectory, Id const &id) const;
			void generateCpp(QString const &outputDirectory, Id const &id);

			void generateStateTransitions(utils::OutFile &out, Id const &id);

			void collectStates(Id const &id);
			QString movementDirection(Id const &movement) const;
			QString forwardBackwardMovementDirection(Id const &movement) const;
			QPoint linkVector(Id const &link) const;

			QStringList mStates;
			int mCurrentSupportStateIndex;

			qrRepo::RepoApi const &mApi;
			gui::ErrorReporter mErrorReporter;
		};

	}
}
