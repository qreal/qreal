#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

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

			QString generateEditor(QString const &pathToFile);
		private:
			void serializeObjects(QDomElement &parent, Id const &idParent);
			void createNode(QDomElement &parent, Id const &id);
			void createEdge (QDomElement &parent, Id const &id);
			void createEnum (QDomElement &parent, Id const &id);
			void setLogicAttributes (QDomElement &parent, Id const &id);
			void setValues (QDomElement &parent, Id const &id);
			void setAssotiations (QDomElement &parent, Id const &id);
			void setUsages (QDomElement &parent, Id const &id);
			void setConnections (QDomElement &parent, Id const &id);
			void createDiagrams (QDomElement &parent, Id const &id);
			void setContainer (QDomElement &parent, QString name);
			void setImported (QDomElement &parent, Id const &idParent);
			void newSetConnections(QDomElement &parent, const Id &id,
					QString const &commonTagName, QString const &internalTagName, QString const &typeName);
			// void addError(QString const &errorText);

			qrRepo::RepoApi const &mApi;
			QDomDocument mDocument;
			QString mErrorText;
			IdList mElements;
		};
	}
}
