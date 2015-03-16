#include "unsupportedBlock.h"

using namespace qReal::interpretation::blocks;

void UnsupportedBlock::run()
{
	error(tr("Block of a type which is unsupported by an interpreter"));
}
