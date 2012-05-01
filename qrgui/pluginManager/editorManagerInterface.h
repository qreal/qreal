#pragma once
#include "../../qrkernel/ids.h"

namespace qReal {
	class EditorManagerInterface
	{
	public:
		enum MetaType {
			edge,
			node,
			none
		};

		virtual MetaType metaTypeOfElement(qReal::Id const& element) const = 0;
	};

}

