#pragma once

#include <QtCore/QString>

#include "../../kernel/ids.h"

namespace qReal {

	namespace client {
		class RepoApi;
	}

	namespace generators {

		class JavaHandler
		{
		public:
			explicit JavaHandler(client::RepoApi const &api);

			QString exportToJava(QString const &pathToFile);
		private:
			QString serializeObject(Id const &id, Id const &parentId);
			QString serializeChildren(Id const &id);
			QString serializeOutcomingLink(Id const &id);
			QString serializeIncomingLink(Id const &id);
			QString serializeLinkBodies(Id const &id);
			QString serializeLink(Id const &id);
			QString initPrimitiveTypes() const;
			QString JavaHandler::getVisibility(Id const &id);
			QString JavaHandler::getType(Id const &id);
			QString JavaHandler::getDefaultValue(Id const &id);
			QString JavaHandler::getOperationFactors(Id const &id);
			QString JavaHandler::isFinal(Id const &id);
			QString JavaHandler::isStatic(Id const &id);
			QString JavaHandler::isNative(Id const &id);
			QString JavaHandler::isSynchronized(Id const &id);
			QString JavaHandler::isAbstract(Id const &id);
			QString JavaHandler::isVolatile(Id const &id);
			QString JavaHandler::isTransient(Id const &id);

			QString serializeMultiplicity(Id const &id, QString const &multiplicity) const;
			bool isTypeSuitable(QString const &type) const;
			bool isVisibilitySuitable(QString const &type) const;

			void addError(QString const &errorText);

			client::RepoApi const &mApi;
			QString mErrorText;
		};

	}
}
