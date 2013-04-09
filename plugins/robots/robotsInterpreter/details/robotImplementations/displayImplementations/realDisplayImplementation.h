#pragma once

#include "abstractDisplayImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace displayImplementations {

class RealDisplayImplementation : public AbstractDisplayImplementation
{
	Q_OBJECT
public:
	RealDisplayImplementation();
	virtual ~RealDisplayImplementation() {}
	virtual void read();

signals:
	void response(bool leftIsPressed
				  , bool rightIsPressed
				  , bool centralIsPressed
				  , bool bottomIsPressed);
};

}
}
}
}
}
}


