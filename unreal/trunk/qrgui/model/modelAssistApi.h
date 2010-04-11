#pragma once

#include "../mainwindow/editormanager.h"

namespace qReal {

	namespace model {

		class Model;

		/** @class ModelAssistApi
		 * 	@brief Класс, предназначенный для синхронизации работы с логической
		 *  моделью и репозиторием и модели. Если в GUI возникает потребность
		 *  изменить логическую модель каким-то более хитрым способом, чем
		 *  просто setData, вызывается метод этого класса. Он модифицирует
		 *  репозиторий и при необходимости оповещает модель об изменении.
		 *  Представляет собой по сути ещё один интерфейс к модели, может быть,
		 *  в будущем станет полноценным прокси к репозиторию.
		 *  Предназначен для того, чтобы как-то контролировать действия GIU в репозитории,
		 *  метод модели mutableApi должен использоваться только в
		 *  парсерах, ну и здесь.
		 * 	*/
		class ModelAssistApi {
		public:
			ModelAssistApi(Model &model, EditorManager const &editorManager);
			virtual EditorManager const &editorManager() const;

			virtual void connect(qReal::Id const &source, qReal::Id const &destination);
			virtual void disconnect(qReal::Id const &source, qReal::Id const &destination);

			virtual Id createElement(qReal::Id const &parent, qReal::Id const &type);
		private:
			ModelAssistApi(ModelAssistApi const &);  // Копирование запрещено.
			ModelAssistApi& operator =(ModelAssistApi const &);  // Присваивание тоже.

			Model &mModel;
			EditorManager const &mEditorManager;
		};

	}

}
