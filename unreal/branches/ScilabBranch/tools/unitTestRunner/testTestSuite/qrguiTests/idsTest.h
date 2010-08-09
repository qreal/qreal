#pragma once

#include "../../../../qReal/qrgui/kernel/ids.h"
#include <QtTest/QTest>

namespace tests
{
	namespace qrgui
	{

		class IdsTest: public QObject
		{
			Q_OBJECT
		private slots:
			void init() {
				mId = qReal::Id("1", "2", "3", "4");
			}

			void testIdToString() {
				QCOMPARE(mId.toString(), QString("qrm:/1/2/3/4"));
			}

			void testElements() {
				QCOMPARE(mId.editor(), QString("1"));
				QCOMPARE(mId.diagram(), QString("2"));
				QCOMPARE(mId.element(), QString("3"));
				QCOMPARE(mId.id(), QString("4"));
			}

			void testToUrl() {
				QCOMPARE(mId.toUrl(), QUrl("qrm:/1/2/3/5"));
			}

			void testCompare() {
				QCOMPARE(mId, qReal::Id("1", "2", "3", "4"));
			}

			void testType() {
				QCOMPARE(mId.type(), qReal::Id("1", "2", "3"));
			}

		private:
			qReal::Id mId;
		};
	}
}
