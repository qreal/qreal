#pragma once

#include "umllib/embedded/linkers/embeddedLinker.h"

class EmbeddedLinkers {
	public:
		EmbeddedLinkers();
	private:
		QList<EmbeddedLinker*> linkers;
};
