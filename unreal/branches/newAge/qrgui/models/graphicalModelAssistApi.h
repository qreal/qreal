#pragma once
#include "../kernel/ids.h"
#include "details/graphicalModel.h"
#include "details/modelsAssistApi.h"

namespace qReal {

	class EditorManager;

	namespace models {

		namespace details {
			class GraphicalModel;
		}

		class GraphicalModelAssistApi : public details::ModelsAssistApi {
		public:
			GraphicalModelAssistApi(details::GraphicalModel &graphicalModel, EditorManager const &editorManager);
			qrRepo::GraphicalRepoApi const &graphicalRepoApi() const;
			virtual Id createElement(qReal::Id const &parent, qReal::Id const &type);
			virtual qReal::IdList children(qReal::Id const &element) const;
			virtual void changeParent(qReal::Id const &element, qReal::Id const &parent, QPointF const &position);

			void setConfiguration(qReal::Id const &elem, QPolygon const &newValue);
			QPolygon configuration(qReal::Id const &elem) const;

			void setPosition(qReal::Id const &elem, QPointF const &newValue);
			QPointF position(qReal::Id const &elem) const;

			void setToPort(qReal::Id const &elem, qreal const &newValue);
			qreal toPort(qReal::Id const &elem) const;

			void setFromPort(qReal::Id const &elem, qreal const &newValue);
			qreal fromPort(qReal::Id const &elem) const;

			void setName(qReal::Id const &elem, QString const &newValue);
			QString name(qReal::Id const &elem) const;

			void setToolTip(qReal::Id const &elem, QString const &newValue);
			QString toolTip(qReal::Id const &elem) const;

		private:
			GraphicalModelAssistApi(GraphicalModelAssistApi const &);
			GraphicalModelAssistApi& operator =(GraphicalModelAssistApi const &);

			details::GraphicalModel &mGraphicalModel;
		};
	}
}
