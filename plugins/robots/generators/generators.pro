TEMPLATE = subdirs

SUBDIRS = \
	generatorBase \
	trikGeneratorBase \
	trikQtsGenerator \
	nxtGeneratorBase \
	nxtOsekCGenerator \
	nxtRussianCGenerator \

trikGeneratorBase.file = $$PWD/trik/trikGeneratorBase
trikQtsGenerator.file = $$PWD/trik/trikQtsGenerator
nxtGeneratorBase.file = $$PWD/nxt/nxtGeneratorBase
nxtOsekCGenerator.file = $$PWD/nxt/nxtOsekCGenerator
nxtRussianCGenerator.file = $$PWD/nxt/nxtRussianCGenerator

trikGeneratorBase.depends = generatorBase
trikQtsGenerator.depends = trikGeneratorBase
nxtGeneratorBase.depends = generatorBase
nxtOsekCGenerator.depends = nxtGeneratorBase
nxtRussianCGenerator.depends = nxtGeneratorBase
