#include <QtGui>

#include "realreporoles.h"

#include "uml_guiobjectfactory.h"

#include "uml_edgeelement.h"
#include "uml_glamour_class.h"

#include "cppcode/reqnConceptAlternativeClass.h"
#include "cppcode/reqnFeaturedClass.h"
#include "cppcode/reqnLeafClass.h"
#include "cppcode/reqnParentAlternativeClass.h"
#include "cppcode/reqnParentClass.h"
#include "cppcode/reqnParentMandatoryClass.h"
#include "cppcode/reqnParentOpAlternativeClass.h"
#include "cppcode/reqnParentOptionalClass.h"
#include "cppcode/reqnParentOrClass.h"



using namespace UML;

Element * UML::GUIObjectFactory(int type){
	switch ( type ) {
		case UML::reqnFeatured:		return new reqnFeaturedClass();
		case UML::reqnConceptAlternative:		return new reqnConceptAlternativeClass();
		case UML::reqnLeaf:		return new reqnLeafClass();
		case UML::reqnParent:		return new reqnParentClass();
		case UML::reqnParentMandatory:		return new reqnParentMandatoryClass();
		case UML::reqnParentOr:		return new reqnParentOrClass();
		case UML::reqnParentOptional:		return new reqnParentOptionalClass();
		case UML::reqnParentAlternative:		return new reqnParentAlternativeClass();
		case UML::reqnParentOpAlternative:		return new reqnParentOpAlternativeClass();
		default: 	 qDebug() << "not creating" << type; 
	}
	return 0;
}
