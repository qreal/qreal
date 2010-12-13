#pragma once
#include <QtCore/QVariant>
#include <QtCore/QPointF>
#include "modelsImplementation/abstractModel.h"

namespace qReal {

	class EditorManager;

	namespace models {

		namespace details {

			namespace modelsImplementation {
				class AbstractModel;
			}

			class ModelsAssistApi
			{
			public:
				ModelsAssistApi(details::modelsImplementation::AbstractModel &model, EditorManager const &editorManager);
				EditorManager const &editorManager() const;
				virtual qReal::Id createElement(qReal::Id const &parent, qReal::Id const &type) = 0;
				virtual qReal::IdList children(qReal::Id const &element) const = 0;
				virtual void changeParent(qReal::Id const &element, qReal::Id const &parent, QPointF const &position = QPointF()) = 0;
				QModelIndex indexById(Id const &id) const;

			protected:
				void setProperty(qReal::Id const &elem, QVariant const &newValue, int const role);
				QVariant property(qReal::Id const &elem, int const role) const;
				int roleIndexByName(qReal::Id const &elem, QString const &roleName) const;

				ModelsAssistApi(ModelsAssistApi const &);
				ModelsAssistApi& operator =(ModelsAssistApi const &);

				details::modelsImplementation::AbstractModel &mModel;
				EditorManager const &mEditorManager;
			};
		}
	}
}
