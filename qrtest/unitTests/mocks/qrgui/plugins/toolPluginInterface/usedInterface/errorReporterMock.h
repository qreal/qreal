#pragma once

#include <QtCore/QString>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class ErrorReporterMock : public qReal::ErrorReporterInterface {
public:
	MOCK_METHOD2(addInformation, void(QString const &message, qReal::Id const &position));
	MOCK_METHOD2(addWarning, void(QString const &message, qReal::Id const &position));
	MOCK_METHOD2(addError, void(QString const &message, qReal::Id const &position));
	MOCK_METHOD2(addCritical, void(QString const &message, qReal::Id const &position));

	MOCK_METHOD3(addUniqueError, void(QString const &message, qReal::gui::Error::Severity const &severity
			, qReal::Id const &position));
	MOCK_METHOD3(delUniqueError, void(QString const &message, qReal::gui::Error::Severity const &severity
			, qReal::Id const &position));
	MOCK_METHOD1(delAllErrorOfElement, void(qReal::Id const &position));

	MOCK_METHOD0(clear, void());
	MOCK_METHOD0(clearErrors, void());
	MOCK_METHOD0(wereErrors, bool());
};

}
