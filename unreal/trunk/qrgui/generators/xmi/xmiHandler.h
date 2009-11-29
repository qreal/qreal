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

		class XmiHandler
		{
		public:
			explicit XmiHandler(qrRepo::RepoApi const &api);

			QString exportToXmi(QString const &pathToFile);
		private:
			void serializeLinks(QDomElement &parent, bool direction, Id const &idParent);
			void serializeChildren(QDomElement &parent, Id const &idParent);
			void serializeObject(QDomElement &parent, Id const &id, Id const &parentId);
			QDomElement serializeOutcomingLink(Id const &id);
			QDomElement serializeIncomingLink(Id const &id);
			QDomElement createOwnedEnd(QString const &direction, Id const &id, Id const &target);
			QDomElement serializeLink(Id const &id);
			void serializeLinkBodies(QDomElement &parent, Id const &id);

			QDomElement createPrimitiveType(QString const &typeName);
			void initPrimitiveTypes(QDomElement &parent);
			QDomElement createMultiplicityNode(QString const &tagName, Id const &id, QString const &value);
			void serializeMultiplicity(QDomElement &parent, Id const &id, QString const &multiplicity);

			QDomElement createDomElement(QString const &tagName, QString const &id);
			QDomElement createDomElement(QString const &tagName, QString const &id, QString const &type);
			void createDomElementWithIdRef(QDomElement &parent, QString const &tagName, QString const &idRef);
			QDomElement createDomElementWithIdRef(QString const &tagName, QString const &idRef);

			bool isTypeSuitable(QString const &type) const;
			bool isVisibilitySuitable(QString const &type) const;

			void addError(QString const &errorText);

			qrRepo::RepoApi const &mApi;
			QDomDocument mDocument;
			QString mErrorText;
		};

	}
}
