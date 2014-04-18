TEMPLATE = subdirs

SUBDIRS = \
	generatorBase \
	nxtGenerator \
	trikGenerator \
	russianCGenerator \

nxtGenerator.depends = generatorBase
trikGenerator.depends = generatorBase
russianCGenerator.depends = generatorBase
