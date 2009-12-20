#include "suite.h"

#include "qrguiTests/idsTest.h"

Q_EXPORT_PLUGIN2(qreal_editors, IdTestSuite)

IdTestSuite::IdTestSuite()
{
}

QString IdTestSuite::suiteName()
{
	return "IdTestSuite";
}

QList<Test> IdTestSuite::tests()
{
	QList<Test> result;
	Test idsTest;
	idsTest.first = new tests::qrgui::IdsTest;
	idsTest.second = "IdsTest";

	result.append(idsTest);
	return result;
}
