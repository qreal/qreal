#include "possibleEdges.h"

PossibleEdgesStorage::PossibleEdgesStorage(const QSet<PossibleEdge> &edges) :
	edgesTo(),
	edgesFrom(),
	edgesBetween()
{
	foreach (const PossibleEdge edge, edges) {
		const ElementPattern to = edge.to();
		const ElementPattern from = edge.from();
		const EdgeType* type = new EdgeType(edge.type());

		edgesTo.insert(to, type);
		edgesFrom.insert(from, type);
		edgesBetween.insert(TwoElementPatterns(to, from), type);

		if (type->isDirected()) {
			edgesTo.insert(from, type);
			edgesFrom.insert(to, type);
			edgesBetween.insert(TwoElementPatterns(from, to), type);
		}
	}
}

QList<const EdgeType*> PossibleEdgesStorage::to(const ElementPattern &node) const {
	return edgesTo.values(node);
}
QList<const EdgeType*> PossibleEdgesStorage::from(const ElementPattern &node) const {
	return edgesFrom.values(node);
}
QList<const EdgeType*> PossibleEdgesStorage::between(const ElementPattern &first,
													 const ElementPattern &second) const {
	return edgesBetween.values(TwoElementPatterns(first, second));
}

//bool NodeElement::initPossibleEdges()
//{
//	if (!mPossibleEdges.isEmpty())
//		return true;

//	EditorInterface const * const editorInterface = mGraphicalAssistApi->editorManager().editorInterface(id().editor());
//	foreach(QString elementName, editorInterface->elements(id().diagram()))
//	{
//		int ne = editorInterface->isNodeOrEdge(elementName);
//		if (ne == -1) {
//			QList<StringPossibleEdge> list = editorInterface->getPossibleEdges(elementName);
//			foreach(StringPossibleEdge pEdge, list) {
//				if (editorInterface->isParentOf(id().diagram(), pEdge.first.first, id().diagram(), id().element())
//						|| (editorInterface->isParentOf(id().diagram(), pEdge.first.second, id().diagram(), id().element()) && !pEdge.second.first))
//				{
//					PossibleEdge possibleEdge = toPossibleEdge(pEdge);
//					mPossibleEdges.insert(possibleEdge);
//					mPossibleEdgeTypes.insert(possibleEdge.second);
//				}
//			}
//		}
//	}

//	return !mPossibleEdges.isEmpty();
//}

//bool EdgeElement::initPossibleEdges()
//{
//	if (!possibleEdges.isEmpty())
//		return true;
//	QString editor = id().editor();
//	//TODO: do a code generation for diagrams
//	QString diagram = id().diagram();
//	EditorInterface * editorInterface = mGraphicalAssistApi->editorManager().editorInterface(editor);
//	QList<StringPossibleEdge> stringPossibleEdges = editorInterface->getPossibleEdges(id().element());
//	foreach (StringPossibleEdge pEdge, stringPossibleEdges)
//	{
//		QPair<qReal::Id, qReal::Id> nodes(Id(editor, diagram, pEdge.first.first),
//										  Id(editor, diagram, pEdge.first.second));
//		QPair<bool, qReal::Id> edge(pEdge.second.first, Id(editor, diagram, pEdge.second.second));
//		PossibleEdge possibleEdge(nodes, edge);
//		possibleEdges.push_back(possibleEdge);
//	}

//	return (!possibleEdges.isEmpty());
//}

//PossibleEdge NodeElement::toPossibleEdge(const StringPossibleEdge &strPossibleEdge)
//{
//	QString editor = id().editor();
//	QString diagram = id().diagram();
//	QPair<qReal::Id, qReal::Id> nodes(qReal::Id(editor, diagram, strPossibleEdge.first.first),
//									  qReal::Id(editor, diagram, strPossibleEdge.first.second));
//	QPair<bool, qReal::Id> link(strPossibleEdge.second.first,
//								qReal::Id(editor, diagram, strPossibleEdge.second.second));
//	return QPair<QPair<qReal::Id, qReal::Id>, PossibleEdgeType>(nodes, link);
//}

//QList<PossibleEdge> NodeElement::getPossibleEdges()
//{
//	return QList<PossibleEdge>::fromSet(mPossibleEdges);
//}

