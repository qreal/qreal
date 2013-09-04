#include "nullificationEncoderGenerator.h"

using namespace qReal::robots::generators::simple;

NullificationEncoderGenerator::NullificationEncoderGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "nullifyEncoder.t", QList<Binding *>(), parent)
{
}
