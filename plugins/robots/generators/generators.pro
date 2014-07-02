TEMPLATE = subdirs

SUBDIRS = \
	generatorBase \
	nxtGenerator \
	trikGenerator \
	russianCGenerator \
	fSharpGenerator \

nxtGenerator.depends = generatorBase
trikGenerator.depends = generatorBase
fSharpGenerator.depends = generatorBase
russianCGenerator.depends = generatorBase
