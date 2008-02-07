#include <QMap>
#include <QHash>
#include <QDebug>

#include "uml_nodeelement.h"
#include "objects.h"

const int ANY = -1;
const int NONE = 0;

using namespace UML;
int check(QList<int> pars, NodeElement* el){
	for( int i=0; i<pars.size(); i++)
		if( el->isChildOf(pars.at(i)) ){
			return pars.at(i);		
	}

	return NONE;
}

int checkParents(QList<int> pars, int id){
	switch(id){
		case -1: return -1; break;
		case 1: return check(pars, new krnnElementClass()); break;
		case 2: return check(pars, new krnnDiagramClass()); break;
		case 3: return check(pars, new krnnNamedElementClass()); break;
		case 4: return check(pars, new krnnCommentClass()); break;
		case 5: return check(pars, new krnnNamespaceClass()); break;
		case 6: return check(pars, new krnnPackageClass()); break;
		case 7: return check(pars, new krnnPackageableElemClass()); break;
		case 8: return check(pars, new krnnTypeClass()); break;
		case 9: return check(pars, new krnnTypedElemClass()); break;
		case 10: return check(pars, new krnnRedefElementClass()); break;
		case 11: return check(pars, new krnnClassifierClass()); break;
		case 12: return check(pars, new krnnPropertyClass()); break;
		case 13: return check(pars, new krnnOperationClass()); break;
		case 14: return check(pars, new krnnParameterClass()); break;
		case 15: return check(pars, new krnnFeatureClass()); break;
		case 16: return check(pars, new krnnBehavioralFeatureClass()); break;
		case 17: return check(pars, new krnnStructuralFeatureClass()); break;
		case 24: return check(pars, new cnClassClass()); break;
		default: return NONE; break;
	}

}

bool canBeConnected( int linkID, int from, int to ){
	QMap< int, QHash< int, int > > edges;
	QHash< int, int> hash;

	hash.clear();
	hash.insertMulti(12, 12);
	edges.insert(18, hash);

	hash.clear();
	hash.insertMulti(12, 12);
	edges.insert(19, hash);

	hash.clear();
	hash.insertMulti(15, 12);
	edges.insert(20, hash);

	hash.clear();
	hash.insertMulti(16, 18);
	edges.insert(21, hash);

	hash.clear();
	hash.insertMulti(16, 17);
	edges.insert(22, hash);

	hash.clear();
	hash.insertMulti(22, 22);
	edges.insert(23, hash);

	hash.clear();
	hash.insertMulti(12, 12);
	edges.insert(25, hash);

	hash.clear();
	hash.insertMulti(12, 12);
	edges.insert(26, hash);

	hash.clear();
	hash.insertMulti(12, 12);
	edges.insert(27, hash);

	if( from == ANY && to == ANY )
		return true; // (-1, -1)

	QHash<int, int> h;
	bool res;
	int fromID = NONE;
	int toID = NONE;
	h = edges.value(linkID);

	if( h.keys().contains(from) )
		fromID = from;
	else
		fromID = checkParents(h.keys(), from);

	if( h.values().contains(to) )
		toID = to;
	else
		toID = checkParents(h.values(), to);

	if( fromID == ANY && toID != NONE)
		return h.values().contains(toID);

	if( toID == ANY && fromID != NONE)
		return h.keys().contains(fromID);

	res = h.values(fromID).contains(toID);

	return res;
}
