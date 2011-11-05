#pragma once

#include <QtCore/QObject>

namespace ubiq {
namespace generator {

/// Main class of UbiqMobile generator, directs generation process.
class Generator : public QObject
{
	Q_OBJECT

public:
	Generator();
	virtual ~Generator();

public slots:
	/// Starts generation process.
	void generate();
};

}
}
