#pragma once

#include "type.h"

namespace qrmc {
	class NonGraphicType : public Type
	{
	public:
		virtual bool resolve();

		virtual void print();

		virtual bool isGraphicalType() const;

		virtual QString generateProperties(QString const &namesTemplate) const;
		virtual QString generatePropertyDefaults(QString const &namesTemplate) const;
		virtual QString generateContainers(QString const &lineTemplate) const;
		virtual QString generateConnections(QString const &lineTemplate) const;
		virtual QString generateUsages(QString const &lineTemplate) const;
		virtual QString generateIsNodeOrEdge(QString const &lineTemplate) const;
		virtual QString generatePossibleEdges(QString const &lineTemplate) const;

		virtual QString generateNodeClass(QString const &classTemplate);
		virtual QString generateEdgeClass(QString const &classTemplate) const;
		virtual QString generateResourceLine(QString const &resourceTemplate) const;

	protected:
		NonGraphicType(Diagram *diagram, qrRepo::RepoApi *api, qReal::Id const &id);
	};
}
