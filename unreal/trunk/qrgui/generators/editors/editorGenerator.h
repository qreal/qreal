#pragma once

#include <QDomDocument>

#include "../../client/repoApi.h"

namespace qReal {

	namespace generators {

		class EditorGenerator {
		public:
			EditorGenerator(client::RepoApi const &api);
			void generate(Id const &editor);
		private:
			Id getOtherEntityFromLink(Id const &linkId, Id const &firstNode);
			QDomElement createNode(QDomDocument doc, Id const &nodeId, QString const &prefix);
			void createMetaEditor(Id const &editor);
			client::RepoApi const &mApi;
		};

	}

}
