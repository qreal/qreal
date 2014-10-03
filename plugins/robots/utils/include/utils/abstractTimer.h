#pragma once

#include <QtCore/QObject>

#include "utilsDeclSpec.h"

namespace utils {

class ROBOTS_UTILS_EXPORT AbstractTimer : public QObject
{
	Q_OBJECT

public:
	virtual void start(int ms) = 0;
	virtual void stop() = 0;

signals:
	void timeout();

protected slots:
	void onTimeout();
};

}
