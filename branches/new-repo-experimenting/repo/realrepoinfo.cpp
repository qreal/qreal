#include "realrepoinfo.h"
RealRepoInfo::RealRepoInfo(){

	Category cat;

	cat.objects.clear();
	cat.name = "Kernel";
	cat.objects  << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18 << 19 << 20 << 21 << 22 << 23	;
	categories << cat;

	cat.objects.clear();
	cat.name = "BASIC BEHAVIOURS";
	cat.objects  << 24 << 25 << 26 << 27	;
	categories << cat;

	cat.objects.clear();
	cat.name = "REQUIREMENTS DIAGRAM";
	cat.objects  << 28 << 29 << 30 << 31 << 32 << 33 << 34 << 35 << 36 << 37	;
	categories << cat;

	cat.objects.clear();
	cat.name = "CLASS DIAGRAM";
	cat.objects  << 38	;
	categories << cat;

	cat.objects.clear();
	cat.name = "USE CASE DIAGRAM";
	cat.objects  << 39 << 40 << 41 << 42 << 43 << 44	;
	categories << cat;

objects  << "krnnElement" << "krnnDiagram" << "krnnNamedElement" << "krnnComment" << "krnnNamespace" << "krnnPackage" << "krnnPackageableElem" << "krnnType" << "krnnTypedElem" << "krnnRedefElement" << "krnnClassifier" << "krnnProperty" << "krnnOperation" << "krnnParameter" << "krnnFeature" << "krnnBehavioralFeature" << "krnnStructuralFeature" << "krneRelationship" << "krneDirRelationship" << "krneComLink" << "krneElementImport" << "krnePackageImport" << "krneGeneralization" << "bbnBehavior" << "bbnBehavioredClassifier" << "bbnOpaqueBehavior" << "bbnFunctionBehavior" << "reqnFeatured" << "reqnConceptAlternative" << "reqnLeaf" << "reqnParent" << "reqnParentMandatory" << "reqnParentOr" << "reqnParentOptional" << "reqnParentAlternative" << "reqnParentOpAlternative" << "reqeP2N" << "cnClass" << "uscnActor" << "uscClassifier" << "uscExtensionPoint" << "untitledEdge_0" << "uscUseCase" << "uscExtend" << "uscInclude";

descriptions  << "Element" << "Diagram" << "NamedElement" << "Comment" << "Namespace" << "Package" << "PackageableElement" << "Type" << "TypedElement" << "RedefinableElement" << "Classifier" << "Property" << "Operation" << "Parameter" << "Feature" << "BehavioralFeature" << "StructuralFeature" << "Relationship" << "DirectedRelationship" << "CommentLink" << "ElementImport" << "PackageImport" << "Generalization" << "Behavour" << "Behavour" << "Behavour" << "Behavour" << "Featured Element" << "Concept Alternative" << "Leaf" << "Parent Node" << "Parent of Mandatory Features" << "Parent of Or-Features" << "Parent of Optional Features" << "Parent of Alternative Features" << "Parent of Optional-Alternative Features" << "Feature P2N Relationship" << "Class" << "Actor" << "Classifier" << "ExtensionPoint" << "embedded edge #0" << "UseCase" << "Extend" << "Include";

}

RealRepoInfo::~RealRepoInfo(){}

	QStringList RealRepoInfo::getObjectCategories() const {

	QStringList l;
	for( int i=0; i<categories.size(); i++)
		l << categories.at(i).name;
	return l;
}

QList<int> RealRepoInfo::getObjects(int category) const{
	return categories.at(category).objects;
}

QString RealRepoInfo::objectName( int id ) const {
	return objects.at(id-1);
}

QString RealRepoInfo::objectDesc( int id ) const{
	return descriptions.at(id-1);
}

