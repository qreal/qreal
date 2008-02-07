#include <QtGui>

#include "realreporoles.h"
#include "objects.h"
#include "uml_guiobjectfactory.h"

using namespace UML;

Element * UML::GUIObjectFactory(int type){
	switch ( type ) {
		case UML::krnnDiagram:
			return new krnnDiagramClass();
		case UML::cnClass:
			return new cnClassClass();
		case UML::krneRelationship:
			return new krneRelationshipClass();
		case UML::krneDirRelationship:
			return new krneDirRelationshipClass();
		case UML::krneComLink:
			return new krneComLinkClass();
		case UML::krneElementImport:
			return new krneElementImportClass();
		case UML::krnePackageImport:
			return new krnePackageImportClass();
		case UML::krneGeneralization:
			return new krneGeneralizationClass();
		case UML::ceAggregation:
			return new ceAggregationClass();
		case UML::ceComposition:
			return new ceCompositionClass();
		case UML::ceDependency:
			return new ceDependencyClass();
		default: 	 qDebug() << "not creating" << type; 
	}
	return 0;
}
