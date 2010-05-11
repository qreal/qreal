#include "createObjectListener.h"

#include <QtCore/QDebug>

using namespace hascolEditor;

void CreateObjectListener::objectCreated(qReal::Id const &)
{
	qDebug() << "CreateObjectListener called";
}
