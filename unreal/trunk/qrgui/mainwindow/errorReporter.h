#pragma once

#include <QtCore/QString>
#include <QtCore/QList>

#include "../kernel/ids.h"
#include "../kernel/definitions.h"

namespace qReal {

	namespace gui {

		class Error {
		public:
			enum Severity {
				information,
				warning,
				error,
				critical
			};

			Error(QString const &message, Severity const &severity, Id const &position);
			Severity severity() const;
			QString message() const;
			Id position() const;
		private:
			QString mMessage;
			Severity mSeverity;
			Id mPosition;
		};

		class ErrorReporter {
		public:
			void addInformation(QString const &message, Id const &position = ROOT_ID);
			void addWarning(QString const &message, Id const &position = ROOT_ID);
			void addError(QString const &message, Id const &position = ROOT_ID);
			void addCritical(QString const &message, Id const &position = ROOT_ID);

			// Этот метод тут только временно, потом будет красивое окошко со списком ошибок.
			void showErrors() const;
		private:
			static QString severityMessage(Error const &error);

			Error::Severity maxSeverity() const;

			QList<Error> mErrors;
		};

	}

}
