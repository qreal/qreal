#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include "../../kernel/NewType.h"

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
                        void serializeLinks(QDomElement &parent, bool direction, NewType const &idParent);
                        void serializeChildren(QDomElement &parent, NewType const &idParent);
                        void serializeObject(QDomElement &parent, NewType const &type, NewType const &parentId);
                        QDomElement serializeOutcomingLink(NewType const &type);
                        QDomElement serializeIncomingLink(NewType const &type);
                        QDomElement createOwnedEnd(QString const &direction, NewType const &type, NewType const &target);
                        QDomElement serializeLink(NewType const &type);
                        void serializeLinkBodies(QDomElement &parent, NewType const &type);

			QDomElement createPrimitiveType(QString const &typeName);
			void initPrimitiveTypes(QDomElement &parent);
                        QDomElement createMultiplicityNode(QString const &tagName, NewType const &type, QString const &value);
                        void serializeMultiplicity(QDomElement &parent, NewType const &type, QString const &multiplicity);

                        QDomElement createDomElement(QString const &tagName, QString const &typeString);
                        QDomElement createDomElement(QString const &tagName, QString const &idString, QString const &typeString);
			void createDomElementWithIdRef(QDomElement &parent, QString const &tagName, QString const &idRef);
			QDomElement createDomElementWithIdRef(QString const &tagName, QString const &idRef);

                        bool isTypeSuitable(QString const &typeString) const;
                        bool isVisibilitySuitable(QString const &typeString) const;

			void addError(QString const &errorText);

			qrRepo::RepoApi const &mApi;
			QDomDocument mDocument;
			QString mErrorText;
		};

	}
}
