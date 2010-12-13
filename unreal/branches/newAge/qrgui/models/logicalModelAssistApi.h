#pragma once
#include "../kernel/ids.h"
#include "details/logicalModel.h"
#include "details/modelsAssistApi.h"

namespace qReal {

	class EditorManager;

	namespace models {

		namespace details {
			class LogicalModel;
		}

		class LogicalModelAssistApi : public details::ModelsAssistApi
		{
		public:
			LogicalModelAssistApi(details::LogicalModel &logicalModel, EditorManager const &editorManager);
			qrRepo::LogicalRepoApi const &logicalRepoApi() const;
			virtual Id createElement(qReal::Id const &parent, qReal::Id const &type);
			virtual qReal::IdList children(qReal::Id const &element) const;
			virtual void changeParent(qReal::Id const &element, qReal::Id const &parent, QPointF const &position = QPointF());

			void connect(qReal::Id const &source, qReal::Id const &destination);
			void disconnect(qReal::Id const &source, qReal::Id const &destination);
			void addUsage(qReal::Id const &source, qReal::Id const &destination);
			void deleteUsage(qReal::Id const &source, qReal::Id const &destination);
			void createConnected(qReal::Id const &sourceElement, qReal::Id const &elementType);
			void createUsed(qReal::Id const &sourceElement, qReal::Id const &elementType);
			Id createConnectedElement(Id const &source, Id const &elementType);
			IdList diagramsFromList(IdList const &list) const;
			qReal::IdList diagramsAbleToBeConnectedTo(qReal::Id const &element) const;
			qReal::IdList diagramsAbleToBeUsedIn(qReal::Id const &element) const;

			void setTo(qReal::Id const &elem, qReal::Id const &newValue);
			qReal::Id to(qReal::Id const &elem) const;

			void setFrom(qReal::Id const &elem, qReal::Id const &newValue);
			qReal::Id from(qReal::Id const &elem) const;

			void setPropertyByRoleName(qReal::Id const &elem, QVariant const &newValue, QString const &roleName);
			QVariant propertyByRoleName(qReal::Id const &elem, QString const &roleName) const;

		private:
			LogicalModelAssistApi(LogicalModelAssistApi const &);  // Copying is forbidden
			LogicalModelAssistApi& operator =(LogicalModelAssistApi const &); // Assignment is forbidden also

			details::LogicalModel &mLogicalModel;
		};
	}
}
