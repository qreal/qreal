#pragma once

#include "../kernel/NewType.h"

namespace qReal {

	class EditorManager;

	namespace model {

		class Model;

		/** @class ModelAssistApi
		 *	@brief class used for syncronization of logical model and repo/model.
		 *	if GUI wants to change model in a way different from setData(),
		 *	it should call this class' methods. ModelAssistApi modifies the repo
		 *	and notifies the model. it's in fact yet another interface to the model
		 *	and even may become a proxy model for repo one day.
		 *	ModelAssistApi used to somehow control GUI's actions towards the repo,
		 *	model's mutableApi() should be used only in parsers and here
		 * */

		class ModelAssistApi {
		public:
			ModelAssistApi(Model &model, EditorManager const &editorManager);
			virtual EditorManager const &editorManager() const;

                        virtual void connect(qReal::NewType const &source, qReal::NewType const &destination);
                        virtual void disconnect(qReal::NewType const &source, qReal::NewType const &destination);
                        virtual void addUsage(qReal::NewType const &source, qReal::NewType const &destination);
                        virtual void deleteUsage(qReal::NewType const &source, qReal::NewType const &destination);

                        virtual NewType createElement(qReal::NewType const &parent, qReal::NewType const &type);

                        virtual void setProperty(qReal::NewType const &elem, int const role, QVariant const &newValue);
                        virtual QVariant getProperty(qReal::NewType const &elem, int const role) const;

                        void createConnected(qReal::NewType const &sourceElement, qReal::NewType const &elementType);
                        void createUsed(qReal::NewType const &sourceElement, qReal::NewType const &elementType);
                        qReal::TypeList diagramsAbleToBeConnectedTo(qReal::NewType const &element) const;
                        qReal::TypeList diagramsAbleToBeUsedIn(qReal::NewType const &element) const;

                        qReal::TypeList children(qReal::NewType const &element) const;

		private:
			ModelAssistApi(ModelAssistApi const &);  // Copying is forbidden
			ModelAssistApi& operator =(ModelAssistApi const &); // Assignment is forbidden also

                        static TypeList diagramsFromList(TypeList const &list);
                        NewType createConnectedElement(NewType const &source, NewType const &elementType);

			Model &mModel;
			EditorManager const &mEditorManager;
		};

	}

}
