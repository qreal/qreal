#pragma once

#include <QList>
#include <QDebug>
#include <QString>
#include <QVariant>

#include "constants.h"
#include "../kernel/ids.h"

namespace qReal {
		class Message {
			public:
				Message(Id const target, action const performed, QString const details,
						QVariant const prevValue, QVariant const newValue);

				Id target() const;
				action performed() const;
				QString details() const;
				QVariant prevValue() const;
				QVariant newValue() const;

				static QList<Message> parseLog(QString path);
				static QString getDataString(QVariant const data);
			private:
				const Id mTarget;
				const action mPerformed;
				const QString mDetails;
				const QVariant mPrevValue;
				const QVariant mNewValue;
		};
}
