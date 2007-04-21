#include <QtGui>

#include "realreporoles.h"

#include "uml_guiobjectfactory.h"

#include "uml_edgeelement.h"
#include "uml_glamour_class.h"
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
		case UML::krneRelationship:		return new krneRelationshipClass();
		case UML::krneDirRelationship:		return new krneDirRelationshipClass();
		case UML::krneComLink:		return new krneComLinkClass();
		case UML::krneElementImport:		return new krneElementImportClass();
		case UML::krnePackageImport:		return new krnePackageImportClass();
		case UML::krneGeneralization:		return new krneGeneralizationClass();
		case UML::reqeP2N:		return new reqeP2NClass();
		default: 	 qDebug() << "not creating" << type; 
	}
	return 0;
}
