#include "realrepoinfo.h"

static bool initCompleted = false;

class Category
{
public:
	QString name;
	QList<int> objects;
};

static QList< Category > categories;
static QStringList objects;
static QStringList descriptions;
static QMap<int, QStringList> map;

static void initStaticData()
{
	if( initCompleted )
		return;

objects  << "krnnElement" << "krnnDiagram" << "krnnNamedElement" << "krnnComment" << "krnnNamespace" << "krnnPackage" << "krnnPackageableElem" << "krnnType" << "krnnTypedElem" << "krnnRedefElement" << "krnnClassifier" << "krnnProperty" << "krnnOperation" << "krnnParameter" << "krnnFeature" << "krnnBehavioralFeature" << "krnnStructuralFeature" << "krneRelationship" << "krneDirRelationship" << "krneComLink" << "krneElementImport" << "krnePackageImport" << "krneGeneralization" << "bbnBehavior" << "bbnBehavioredClassifier" << "bbnOpaqueBehavior" << "bbnFunctionBehavior" << "reqnFeatured" << "reqnConceptAlternative" << "reqnLeaf" << "reqnParent" << "reqnParentMandatory" << "reqnParentOr" << "reqnParentOptional" << "reqnParentAlternative" << "reqnParentOpAlternative" << "reqeP2N" << "cnClass" << "uscnActor" << "uscClassifier" << "uscExtensionPoint" << "untitledEdge_0" << "uscUseCase" << "uscExtend" << "uscInclude";

descriptions  << "Element" << "Diagram" << "NamedElement" << "Comment" << "Namespace" << "Package" << "PackageableElement" << "Type" << "TypedElement" << "RedefinableElement" << "Classifier" << "Property" << "Operation" << "Parameter" << "Feature" << "BehavioralFeature" << "StructuralFeature" << "Relationship" << "DirectedRelationship" << "CommentLink" << "ElementImport" << "PackageImport" << "Generalization" << "Behavour" << "Behavour" << "Behavour" << "Behavour" << "Featured Element" << "Concept Alternative" << "Leaf" << "Parent Node" << "Parent of Mandatory Features" << "Parent of Or-Features" << "Parent of Optional Features" << "Parent of Alternative Features" << "Parent of Optional-Alternative Features" << "Feature P2N Relationship" << "Class" << "Actor" << "Classifier" << "ExtensionPoint" << "embedded edge #0" << "UseCase" << "Extend" << "Include";

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

// from realreporoles.cpp

	QStringList l;
	l.clear();
		l << "state";
	map.insert(1, l);

	l.clear();
		l << "name";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(2, l);

	l.clear();
		l << "name";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(3, l);

	l.clear();
		l << "body";
		l << "state";
	map.insert(4, l);

	l.clear();
		l << "name";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(5, l);

	l.clear();
		l << "name";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(6, l);

	l.clear();
		l << "name";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(7, l);

	l.clear();
		l << "visibility";
		l << "name";
		l << "qualifiedName";
		l << "state";
	map.insert(8, l);

	l.clear();
		l << "type";
		l << "name";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(9, l);

	l.clear();
		l << "isLeaf";
		l << "name";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(10, l);

	l.clear();
		l << "isAbstract";
		l << "visibility";
		l << "name";
		l << "qualifiedName";
		l << "state";
		l << "isLeaf";
	map.insert(11, l);

	l.clear();
		l << "isReadOnly";
		l << "isStatic";
		l << "isLeaf";
		l << "name";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
		l << "type";
	map.insert(12, l);

	l.clear();
		l << "isQuery";
		l << "isOrdered";
		l << "isUnique";
		l << "lower";
		l << "upper";
		l << "type";
		l << "isAbstract";
		l << "isStatic";
		l << "isLeaf";
		l << "name";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(13, l);

	l.clear();
		l << "direction";
		l << "type";
		l << "name";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(14, l);

	l.clear();
		l << "isStatic";
		l << "isLeaf";
		l << "name";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(15, l);

	l.clear();
		l << "isAbstract";
		l << "isStatic";
		l << "isLeaf";
		l << "name";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(16, l);

	l.clear();
		l << "isReadOnly";
		l << "isStatic";
		l << "isLeaf";
		l << "name";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
		l << "type";
	map.insert(17, l);

	l.clear();
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "state";
	map.insert(18, l);

	l.clear();
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "state";
	map.insert(19, l);

	l.clear();
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
	map.insert(20, l);

	l.clear();
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "alias";
		l << "visibility";
		l << "state";
	map.insert(21, l);

	l.clear();
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "visibility";
		l << "state";
	map.insert(22, l);

	l.clear();
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "isSubstitutable";
		l << "state";
	map.insert(23, l);

	l.clear();
		l << "isAbstract";
		l << "visibility";
		l << "name";
		l << "qualifiedName";
		l << "state";
		l << "isLeaf";
	map.insert(24, l);

	l.clear();
		l << "isAbstract";
		l << "visibility";
		l << "name";
		l << "qualifiedName";
		l << "state";
		l << "isLeaf";
	map.insert(25, l);

	l.clear();
		l << "body";
		l << "language";
		l << "isAbstract";
		l << "visibility";
		l << "name";
		l << "qualifiedName";
		l << "state";
		l << "isLeaf";
	map.insert(26, l);

	l.clear();
		l << "body";
		l << "language";
		l << "isAbstract";
		l << "visibility";
		l << "name";
		l << "qualifiedName";
		l << "state";
		l << "isLeaf";
	map.insert(27, l);

	l.clear();
		l << "name";
		l << "description";
		l << "priority";
		l << "source";
		l << "status";
		l << "state";
	map.insert(28, l);

	l.clear();
		l << "name";
		l << "description";
		l << "priority";
		l << "source";
		l << "status";
		l << "state";
	map.insert(29, l);

	l.clear();
		l << "name";
		l << "description";
		l << "priority";
		l << "source";
		l << "status";
		l << "state";
	map.insert(30, l);

	l.clear();
		l << "name";
		l << "description";
		l << "priority";
		l << "source";
		l << "status";
		l << "state";
	map.insert(31, l);

	l.clear();
		l << "name";
		l << "description";
		l << "priority";
		l << "source";
		l << "status";
		l << "state";
	map.insert(32, l);

	l.clear();
		l << "name";
		l << "description";
		l << "priority";
		l << "source";
		l << "status";
		l << "state";
	map.insert(33, l);

	l.clear();
		l << "name";
		l << "description";
		l << "priority";
		l << "source";
		l << "status";
		l << "state";
	map.insert(34, l);

	l.clear();
		l << "name";
		l << "description";
		l << "priority";
		l << "source";
		l << "status";
		l << "state";
	map.insert(35, l);

	l.clear();
		l << "name";
		l << "description";
		l << "priority";
		l << "source";
		l << "status";
		l << "state";
	map.insert(36, l);

	l.clear();
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "state";
	map.insert(37, l);

	l.clear();
		l << "isAbstract";
		l << "visibility";
		l << "name";
		l << "qualifiedName";
		l << "state";
		l << "isLeaf";
	map.insert(38, l);

	l.clear();
		l << "name";
		l << "isAbstract";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
		l << "isLeaf";
	map.insert(39, l);

	l.clear();
		l << "name";
		l << "isAbstract";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
		l << "isLeaf";
	map.insert(40, l);

	l.clear();
		l << "name";
		l << "isLeaf";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(41, l);

	l.clear();
	map.insert(42, l);

	l.clear();
		l << "name";
		l << "isAbstract";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
		l << "isLeaf";
	map.insert(43, l);

	l.clear();
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "name";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(44, l);

	l.clear();
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "state";
	map.insert(45, l);

	initCompleted = true;
}

RealRepoInfo::RealRepoInfo()
{
	initStaticData();
}

RealRepoInfo::~RealRepoInfo()
{
}

QStringList RealRepoInfo::getObjectCategories() const
{
	QStringList l;
	for( int i=0; i<categories.size(); i++)
		l << categories.at(i).name;
	return l;
}

QList<int> RealRepoInfo::getObjects(int category) const
{
	return categories.at(category).objects;
}

QString RealRepoInfo::objectName( int id ) const
{
	return objects.at(id-1);
}

QString RealRepoInfo::objectDesc( int id ) const
{
	return descriptions.at(id-1);
}

QString RealRepoInfo::getColumnName(int type, int role) const
{
	return map.value(type).at(indexByRole(role));
}

QStringList RealRepoInfo::getColumnNames(int type) const
{
	return map.value(type);
}

