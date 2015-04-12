#pragma once

#include "type.h"

namespace qrmc {
	class NonGraphicType : public Type
	{
	public:
		virtual bool resolve();

		virtual void print();

		virtual bool isGraphicalType() const;

		virtual QString generateProperties(const QString &namesTemplate) const;
		virtual QString generatePropertyDefaults(const QString &namesTemplate) const;
		virtual QString generatePropertyDisplayedNames(const QString &lineTemplate) const;
		virtual QString generateReferenceProperties(const QString &lineTemplate) const;
		virtual QString generatePortTypes(const QString &lineTemplate) const;//fix
		virtual QString generatePropertyName(const QString &lineTemplate) const;
		virtual QString generateParents(const QString &lineTemplate) const;
		virtual QString generateContainers(const QString &lineTemplate) const;
		virtual QString generateConnections(const QString &lineTemplate) const;
		virtual QString generateUsages(const QString &lineTemplate) const;
		virtual QString generateIsNodeOrEdge(const QString &lineTemplate) const;
		virtual QString generatePossibleEdges(const QString &lineTemplate) const;

		virtual QString generateNodeClass(const QString &classTemplate);
		virtual QString generateEdgeClass(const QString &classTemplate) const;
		virtual QString generateResourceLine(const QString &resourceTemplate) const;

	protected:
		NonGraphicType(Diagram *diagram, qrRepo::LogicalRepoApi *api, const qReal::Id &id);
	};
}
