#pragma once
#include "../../qrkernel/ids.h"
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
			virtual Id createElement(Id const &parent, Id const &type);
			virtual Id createElement(Id const &parent, Id const &id, bool isFromLogicalModel, QString const &name, QPointF const &position);
			virtual IdList children(Id const &element) const;
			virtual void changeParent(Id const &element, Id const &parent, QPointF const &position = QPointF());

			void connect(Id const &source, Id const &destination);
			void disconnect(Id const &source, Id const &destination);
			void addUsage(Id const &source, Id const &destination);
			void deleteUsage(Id const &source, Id const &destination);
			void createConnected(Id const &sourceElement, Id const &elementType);
			void createUsed(Id const &sourceElement, Id const &elementType);
			Id createConnectedElement(Id const &source, Id const &elementType);
			IdList diagramsAbleToBeConnectedTo(Id const &element) const;
			IdList diagramsAbleToBeUsedIn(Id const &element) const;

			void setPropertyByRoleName(Id const &elem, QVariant const &newValue, QString const &roleName);
			QVariant propertyByRoleName(Id const &elem, QString const &roleName) const;

			bool isLogicalId(Id const &id) const;

		private:
			details::LogicalModel &mLogicalModel;

			LogicalModelAssistApi(LogicalModelAssistApi const &);  // Copying is forbidden
			LogicalModelAssistApi& operator =(LogicalModelAssistApi const &); // Assignment is forbidden also

			IdList diagramsFromList(IdList const &list) const;
		};
	}
}
