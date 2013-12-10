#pragma once

#include <QtCore/QString>
#include <QtCore/QVariant>

namespace qReal {
class UXInfoInterface {

public:
	virtual ~UXInfoInterface() {}

	virtual void reportSettingsChanges(QString const &name, QVariant const &oldValue, QVariant const &newValue) = 0;
};
}
