#pragma once

#include "graphicType.h"

#include <QList>

namespace qrmc {
	class MetaCompiler;

	class NodeType : public GraphicType
	{
	public:
		NodeType(Diagram *diagram, qrRepo::LogicalRepoApi *api, qReal::Id const id);
		virtual Type* clone() const;
		virtual ~NodeType();

		virtual bool init(QString const &context);

		virtual void print();

		virtual QString generateIsNodeOrEdge(QString const &lineTemplate) const;
		virtual QString generateNodeClass(QString const &classTemplate);
		virtual QString generateEdgeClass(QString const &classTemplate) const;
		virtual QString generateResourceLine(QString const &resourceTemplate) const;

	private:
		void generateContainerStuff(QString &classTemplate) const;
		void generateContextMenuItems(QString &classTemplate, MetaCompiler *compiler) const;
		QString loadBoolProperty(qReal::Id const &id, QString const &property) const;
		QString loadIntProperty(qReal::Id const &id, QString const &property) const;

		bool mIsPin;
		bool mIsHavePin;
	};
}
