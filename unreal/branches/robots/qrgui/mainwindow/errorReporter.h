#pragma once

#include <QtCore/QString>
#include <QtCore/QList>
#include"mainwindow.h"
#include "../kernel/ids.h"
#include "../kernel/definitions.h"
#include "error.h"
#include "errorlistwidget.h"

namespace qReal {

	namespace gui {

		class ErrorReporter : public QObject {
			Q_OBJECT

		public:
			void addInformation(QString const &message, Id const &position = ROOT_ID);
			void addWarning(QString const &message, Id const &position = ROOT_ID);
			void addError(QString const &message, Id const &position = ROOT_ID);
			void addCritical(QString const &message, Id const &position = ROOT_ID);
			bool showErrors(ErrorListWidget* const errorListWidget, QDockWidget* const errorList) const;
		private:
			static QString severityMessage(Error const &error);
			QList<Error> mErrors;
		};

	}

}
