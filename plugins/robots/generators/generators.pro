TEMPLATE = subdirs

SUBDIRS = \
	generatorBase \
	trikQtsGenerator \
	nxtOsekCGenerator \
	nxtRussianCGenerator \

trikQtsGenerator.file = $$PWD/trik/trikQtsGenerator
nxtOsekCGenerator.file = $$PWD/nxt/nxtOsekCGenerator
nxtRussianCGenerator.file = $$PWD/nxt/nxtRussianCGenerator

trikQtsGenerator.depends = generatorBase
nxtOsekCGenerator.depends = generatorBase
nxtRussianCGenerator.depends = generatorBase
