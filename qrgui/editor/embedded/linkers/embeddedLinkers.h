#pragma once

#include "editor/embedded/linkers/embeddedLinker.h"

class EmbeddedLinkers
{
public:
	EmbeddedLinkers();
private:
	QList<EmbeddedLinker*> linkers;
};
