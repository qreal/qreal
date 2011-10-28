#pragma once

#include "embeddedLinker.h"

class EmbeddedLinkers {
	public:
		EmbeddedLinkers();
	private:
		QList<EmbeddedLinker*> linkers;
};
