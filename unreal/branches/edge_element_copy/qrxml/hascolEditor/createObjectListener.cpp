#include "createObjectListener.h"

#include <QtCore/QDebug>

using namespace hascolEditor;

void CreateObjectListener::objectCreated(qReal::Id const &id)
{
	qDebug() << "CreateObjectListener called";
}
