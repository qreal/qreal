TEMPLATE = subdirs

SUBDIRS = \
	generatorBase \
	trikQtsGenerator \
	nxtGeneratorBase \
	nxtOsekCGenerator \
	nxtRussianCGenerator \

trikQtsGenerator.file = $$PWD/trik/trikQtsGenerator
nxtGeneratorBase.file = $$PWD/nxt/nxtGeneratorBase
nxtOsekCGenerator.file = $$PWD/nxt/nxtOsekCGenerator
nxtRussianCGenerator.file = $$PWD/nxt/nxtRussianCGenerator

trikQtsGenerator.depends = generatorBase
nxtGeneratorBase.depends = generatorBase
nxtOsekCGenerator.depends = nxtGeneratorBase
nxtRussianCGenerator.depends = nxtGeneratorBase
