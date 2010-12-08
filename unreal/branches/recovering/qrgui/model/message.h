#pragma once

#include <QList>
#include <QDebug>
#include <QString>
#include <QVariant>
#include <QLinkedList>

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
				QString toString() const;

				Message patchMessage() const;

				static QString getDataString(QVariant const data);
				static QVariant parseQVariant(QString const data);
				static QLinkedList<Message> parseLog(QString path);

			private:
				const Id mTarget;
				const action mPerformed;
				const QString mDetails;
				const QVariant mPrevValue;
				const QVariant mNewValue;
		};
}
