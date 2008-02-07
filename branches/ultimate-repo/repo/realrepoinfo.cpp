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
static QList< QIcon > icons;
static QMap<int, QStringList> map;

static void initStaticData()
{
	if ( initCompleted )
		return;

	Category cat;

	cat.objects.clear();
	cat.name = "Kernel";
	cat.objects  << 2 << 3 << 18 << 19 << 20 << 21 << 22 << 23	;
	categories << cat;

	cat.objects.clear();
	cat.name = "CLASS DIAGRAM";
	cat.objects  << 2 << 18 << 24 << 25 << 26 << 27	;
	categories << cat;

objects  << "krnnElement" << "krnnDiagram" << "krnnNamedElement" << "krnnComment" << "krnnNamespace" << "krnnPackage" << "krnnPackageableElem" << "krnnType" << "krnnTypedElem" << "krnnRedefElement" << "krnnClassifier" << "krnnProperty" << "krnnOperation" << "krnnParameter" << "krnnFeature" << "krnnBehavioralFeature" << "krnnStructuralFeature" << "krneRelationship" << "krneDirRelationship" << "krneComLink" << "krneElementImport" << "krnePackageImport" << "krneGeneralization" << "cnClass" << "ceAggregation" << "ceComposition" << "ceDependency";

descriptions  << "Element" << "Diagram" << "NamedElement" << "Comment" << "Namespace" << "Package" << "PackageableElement" << "Type" << "TypedElement" << "RedefinableElement" << "Classifier" << "Property" << "Operation" << "Parameter" << "Feature" << "BehavioralFeature" << "StructuralFeature" << "Relationship" << "DirectedRelationship" << "CommentLink" << "ElementImport" << "PackageImport" << "Generalization" << "Class" << "Aggregation" << "Composition" << "Dependency";

// from realreporoles.cpp

	QStringList l;
	l.clear();
		l << "name";
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
		l << "name";
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
		l << "name";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(8, l);

	l.clear();
		l << "name";
		l << "type";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(9, l);

	l.clear();
		l << "name";
		l << "isLeaf";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(10, l);

	l.clear();
		l << "name";
		l << "isAbstract";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
		l << "isLeaf";
	map.insert(11, l);

	l.clear();
		l << "name";
		l << "isReadOnly";
		l << "isStatic";
		l << "isLeaf";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
		l << "type";
	map.insert(12, l);

	l.clear();
		l << "name";
		l << "isQuery";
		l << "isOrdered";
		l << "isUnique";
		l << "lower";
		l << "upper";
		l << "type";
		l << "isAbstract";
		l << "isStatic";
		l << "isLeaf";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(13, l);

	l.clear();
		l << "name";
		l << "direction";
		l << "type";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(14, l);

	l.clear();
		l << "name";
		l << "isStatic";
		l << "isLeaf";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(15, l);

	l.clear();
		l << "name";
		l << "isAbstract";
		l << "isStatic";
		l << "isLeaf";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
	map.insert(16, l);

	l.clear();
		l << "name";
		l << "isReadOnly";
		l << "isStatic";
		l << "isLeaf";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
		l << "type";
	map.insert(17, l);

	l.clear();
		l << "name";
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "state";
	map.insert(18, l);

	l.clear();
		l << "name";
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "state";
	map.insert(19, l);

	l.clear();
		l << "name";
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
	map.insert(20, l);

	l.clear();
		l << "name";
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "alias";
		l << "visibility";
		l << "state";
	map.insert(21, l);

	l.clear();
		l << "name";
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "visibility";
		l << "state";
	map.insert(22, l);

	l.clear();
		l << "name";
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "isSubstitutable";
		l << "state";
	map.insert(23, l);

	l.clear();
		l << "name";
		l << "vals";
		l << "methods";
		l << "isAbstract";
		l << "visibility";
		l << "qualifiedName";
		l << "state";
		l << "isLeaf";
	map.insert(24, l);

	l.clear();
		l << "name";
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "state";
	map.insert(25, l);

	l.clear();
		l << "name";
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "state";
	map.insert(26, l);

	l.clear();
		l << "name";
		l << "from";
		l << "to";
		l << "fromPort";
		l << "toPort";
		l << "state";
	map.insert(27, l);

//initializing icons

	icons
		<< QIcon()
		<< QIcon(":/shapes/krnnDiagramClass.svg")
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon()
		<< QIcon(":/shapes/cnClassClass.svg")
		<< QIcon()
		<< QIcon()
		<< QIcon();

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

QIcon RealRepoInfo::objectIcon( int id ) const
{
	if ( id > 0 && id <= icons.size() )
		return icons.at(id-1);
	else
		return QIcon();
}
