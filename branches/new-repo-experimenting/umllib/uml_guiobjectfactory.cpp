#include <QtGui>

#include "realreporoles.h"

#include "uml_guiobjectfactory.h"

#include "uml_edgeelement.h"
#include "uml_glamour_class.h"

using namespace UML;

Element * UML::GUIObjectFactory(int type)
{
	switch ( type ) {
		case UML::Class:	return new GlamourClass();
		case UML::Link:		return new EdgeElement();
		default:			qDebug() << "not creating" << type;
	}
	return 0;
}
