#pragma once

#include "../../../kernel/patterns.h"
#include "../../../editorManager/editorManager.h"

using namespace qReal;

typedef QPair<ElementPattern, ElementPattern> TwoElementPatterns;

class EdgeType;
class PossibleEdge;
class PossibleEdgesStorage {
	public:
		PossibleEdgesStorage(
			const QSet<PossibleEdge> &edges);
		PossibleEdgesStorage(EditorManager* editor);
		QList<const EdgeType*> to(const ElementPattern &node) const;
		QList<const EdgeType*> from(const ElementPattern &node) const;
		QList<const EdgeType*> between(const ElementPattern &first,
								const ElementPattern &second) const;
	private:
		QMultiHash<const ElementPattern, const EdgeType*> edgesTo;
		QMultiHash<const ElementPattern, const EdgeType*> edgesFrom;
		QMultiHash<const TwoElementPatterns, const EdgeType*> edgesBetween;
};

/* Assistant types */

class EdgeType {
	public:
		EdgeType(
			const bool &directed,
			const ElementPattern &edge) :
				directed(directed), edge(edge) {}
		EdgeType(const EdgeType &copy) :
				directed(copy.isDirected()),
				edge(copy.getEdge()) {}
		bool isDirected() const {return directed;}
		ElementPattern getEdge() const {return edge;}
	private:
		const bool &directed;
		const ElementPattern &edge;
};

class PossibleEdge {
	public:
		PossibleEdge(
			const ElementPattern &from,
			const ElementPattern &to,
			const EdgeType &type) :
				nodeFrom(from), nodeTo(to), edgeType(type) {}
		ElementPattern from() const {return nodeFrom;}
		ElementPattern to() const {return nodeTo;}
		EdgeType type() const {return edgeType;}
	private:
		const ElementPattern &nodeFrom;
		const ElementPattern &nodeTo;
		const EdgeType &edgeType;
};
