#pragma once

#include <QString>

namespace qrRepo
{
namespace details
{

class SerializerVersioningInterface
{
public:
	virtual ~SerializerVersioningInterface() {};

	virtual bool doAdd(QString const &what, bool force = true) = 0;
	virtual bool doRemove(QString const &what, bool force = true) = 0;
	virtual QStringList newErrors() = 0;

};

}
}
