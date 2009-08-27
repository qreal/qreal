#pragma once

#include <QtCore/QString>

#include "../../kernel/ids.h"

namespace qReal {

	namespace client {
		class RepoApi;
	}

	namespace generators {

		class XmiHandler
		{
		public:
			explicit XmiHandler(client::RepoApi const &api);

			QString exportToXmi(QString const &pathToFile);
		private:
			QString serializeObject(Id const &id, Id const &parentId);
			QString serializeChildren(Id const &id);
			QString serializeOutcomingLink(Id const &id);
			QString serializeIncomingLink(Id const &id);
			QString serializeLinkBodies(Id const &id);
			QString serializeLink(Id const &id);
			QString initPrimitiveTypes() const;

			QString serializeMultiplicity(Id const &id, QString const &multiplicity) const;
			bool isTypeSuitable(QString const &type) const;
			bool isVisibilitySuitable(QString const &type) const;

			void addError(QString const &errorText);

			client::RepoApi const &mApi;
			QString mErrorText;
		};

	}
}
