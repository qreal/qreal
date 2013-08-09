#pragma once

#include <QtCore/QString>
#include "../../qrkernel/ids.h"
#include <QtCore/QPersistentModelIndex>

namespace qReal {

/// Interface for getting data from the repo by plugins' classes.
class ElementRepoInterface {
public:
	virtual QString logicalProperty(QString const &roleName) const = 0;
	virtual qReal::Id id() const = 0;
	virtual QString name() const = 0;
};

}
