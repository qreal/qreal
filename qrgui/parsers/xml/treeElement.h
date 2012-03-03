#pragma once

#include <QtCore/QList>
#include "../../../qrkernel/ids.h"

namespace qrRepo {
	class RepoApi;
}

namespace qReal {

	class EditorManagerInterface;

	namespace parsers {

		class TreeElement
		{
		public:
			TreeElement(const Id &id);
			void addChild(TreeElement *const child);

		private:
			QList<TreeElement*> mChildren;
			Id mId;
		};
	}
}

