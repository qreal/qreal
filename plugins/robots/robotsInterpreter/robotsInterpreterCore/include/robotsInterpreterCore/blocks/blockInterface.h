#pragma once

#include <QtCore/QObject>

namespace robotsInterpreterCore {
namespace blocks {

class BlockInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~BlockInterface() {}
};

}
}
