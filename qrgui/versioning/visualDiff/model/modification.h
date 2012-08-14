#pragma once

#include <QtCore/QVariant>

#include "diffState.h"

namespace versioning
{
namespace details
{

class Modification
{
public:
	Modification();

	DiffState state() const;
	QVariant oldValue() const;
	QVariant newValue() const;

	void setState(DiffState const state);
	void setOldValue(QVariant const &value);
	void setNewValue(QVariant const &value);

private:
	DiffState mState;
	QVariant mOldValue;
	QVariant mNewValue;
};

}
}
