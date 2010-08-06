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
			void addInformation(QString const &message, Id const &position = Id::rootId());
			void addWarning(QString const &message, Id const &position = Id::rootId());
			void addError(QString const &message, Id const &position = Id::rootId());
			void addCritical(QString const &message, Id const &position = Id::rootId());

			// TODO: remove it and add nice window with errors list
			bool showErrors(QString const &successMessage) const;
		private:
			static QString severityMessage(Error const &error);

			Error::Severity maxSeverity() const;

			QList<Error> mErrors;
		};

	}

}
