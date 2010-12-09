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
				Message(Id const scene, Id const target, action performed);
				Message(Id const scene, Id const target, action const performed,
				QString const details, QVariant const prevValue, QVariant const newValue);

				Id scene() const;
				Id target() const;
				bool valid() const;
				action performed() const;
				QString details() const;
				QVariant prevValue() const;
				QVariant newValue() const;
				QString toString() const;

				Message generatePatchMessage() const;
				static QString getDataString(QVariant const data);
				static QVariant parseQVariant(QString const data);
				static QLinkedList<Message> parseLog(QString const path);

			private:
				Message();

				bool mValid;
				const Id mScene;
				const Id mTarget;
				const action mPerformed;
				const QString mDetails;
				const QVariant mPrevValue;
				const QVariant mNewValue;
		};
}
