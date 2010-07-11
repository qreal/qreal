#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QHash>

#include "../../kernel/ids.h"

namespace qrRepo {
	class RepoApi;
}

namespace qReal {

	namespace generators {

		class MetaGenerator
		{
		public:
			explicit MetaGenerator(qrRepo::RepoApi const &api);

			QHash<Id, QString> getMetamodelList();
			QString generateEditor(Id const metamodelId, QString const &pathToFile);
		private:
			void serializeObjects(QDomElement &parent, Id const &idParent);
			void createNode(QDomElement &parent, Id const &id);
			void createEdge(QDomElement &parent, Id const &id);
			void createEnum(QDomElement &parent, Id const &id);
			void setLogicAttributes(QDomElement &parent, Id const &id);
			void setValues(QDomElement &parent, Id const &id);
			void setAssotiations(QDomElement &parent, Id const &id);
			void setUsages(QDomElement &parent, Id const &id);
			void setConnections(QDomElement &parent, Id const &id);
			void setPossibleEdges(QDomElement &parent, Id const &id);
			void setPin(QDomElement &parent, Id const &id);
			void setStatusElement(QDomElement &parent, Id const &id, QString const &tagName, QString const &propertyName);
			void setAction(QDomElement &parent, Id const &id);
			void createDiagrams(QDomElement &parent, Id const &id);
			void setContainer(QDomElement &parent, QString name, Id const &id);
			void setContainerProperties(QDomElement &parent, Id const &id);
			void setImported(QDomElement &parent, Id const &idParent);
			void newSetConnections(QDomElement &parent, const Id &id,
					QString const &commonTagName, QString const &internalTagName, QString const &typeName);
			void ensureCorrectness (QDomElement element,QString const &tagName, QString const &value);

			qrRepo::RepoApi const &mApi;
			QDomDocument mDocument;
			QString mErrorText;
			IdList mElements;
			QString mDiagramName;
		};
	}
}
