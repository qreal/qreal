#include <QtGui>

#include "uml_guiobjectfactory.h"

#include "uml_edgeelement.h"
#include "uml_glamour_class.h"

using namespace UML;

Element * UML::GUIObjectFactory(QString &type)
{
    if (type == "eP2N") {
            return new EdgeElement();
    } else {
            return new GlamourClass();
    }
											
}
