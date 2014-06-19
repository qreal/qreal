#pragma once

#include <QtCore/QObject>

#include "../utilsDeclSpec.h"
#include "navigationPageInterface.h"

namespace navigation
{

class QRUTILS_EXPORT NavigationController : public QObject
{
	Q_OBJECT

public:
	NavigationController(NavigationState *state = NULL
			, NavigationPageInterface *defaultPage = NULL);

	NavigationPageInterface *defaultPage() const;
	NavigationState *state() const;
	void setState(NavigationState *state);

public slots:
	void switchTo(NavigationPageInterface *page);

signals:
	void pageChanged(NavigationPageInterface *page);

private:
	NavigationPageInterface *mDefaultPage;
	NavigationState *mState;
};

}
