#pragma once

#include "../qrgui/kernel/definitions.h"
#include "private/client.h"

namespace qReal {

	namespace client {

		class RepoApi {
		public:
			RepoApi();
			// Default destructor ok.

			QString name(Id const &id) const;
			void setName(Id const &id, QString const &name);

			IdList children(Id const &id) const;
			void addChild(Id const &id, Id const &child);
			void removeChild(Id const &id, Id const &child);

			void removeElement(Id const &id);

			IdList parents(Id const &id) const;
			void addParent(Id const &id, Id const &parent);

			IdList outcomingLinks(Id const &id) const;
			IdList incomingLinks(Id const &id) const;
			IdList links(Id const &id) const;

			QString typeName(Id const &id) const;

			QVariant property(Id const &id, QString const &propertyName) const;
			QString stringProperty(Id const &id, QString const &propertyName) const;
			void setProperty(Id const &id, QString const &propertyName, QVariant const &value);
			void removeProperty(Id const &id, QString const &propertyName);
			bool hasProperty(Id const &id, QString const &propertyName) const;

			Id from(Id const &id) const;
			void setFrom(Id const &id, Id const &from);

			Id to(Id const &id) const;
			void setTo(Id const &id, Id const &to);

			double fromPort(Id const &id) const;
			void setFromPort(Id const &id, double fromPort);

			double toPort(Id const &id) const;
			void setToPort(Id const &id, double toPort);

			Id otherEntityFromLink(Id const &linkId, Id const &firstNode) const;

			void exterminate();
			void save() const;

		private:
			RepoApi(RepoApi const &other);  // Копировать нельзя.
			RepoApi& operator =(RepoApi const &);  // Присваивать тоже.

			void addToIdList(Id const &target, QString const &listName, Id const &data);
			void removeFromList(Id const &target, QString const &listName, Id const &data);

			IdList links(Id const &id, QString const &direction) const;
			void removeLinkEnds(QString const &endName, Id const &id);

			details::Client mClient;
		};

	}

}
