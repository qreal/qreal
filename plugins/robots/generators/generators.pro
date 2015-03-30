TEMPLATE = subdirs

SUBDIRS = \
	generatorBase \
	trikGeneratorBase \
	trikQtsGenerator \
	trikFSharpGenerator \
	trikRuntimeUploaderPlugin \
	nxtGeneratorBase \
	nxtOsekCGenerator \
	nxtRussianCGenerator \
	ev3GeneratorBase \
	ev3RbfGenerator \

trikGeneratorBase.file = $$PWD/trik/trikGeneratorBase/trikGeneratorBase.pro
trikQtsGenerator.file = $$PWD/trik/trikQtsGenerator/trikQtsGenerator.pro
trikFSharpGenerator.file = $$PWD/trik/trikFSharpGenerator/trikFSharpGenerator.pro
trikRuntimeUploaderPlugin.file = $$PWD/trik/trikRuntimeUploaderPlugin/trikRuntimeUploaderPlugin.pro
nxtGeneratorBase.file = $$PWD/nxt/nxtGeneratorBase/nxtGeneratorBase.pro
nxtOsekCGenerator.file = $$PWD/nxt/nxtOsekCGenerator/nxtOsekCGenerator.pro
nxtRussianCGenerator.file = $$PWD/nxt/nxtRussianCGenerator/nxtRussianCGenerator.pro
ev3GeneratorBase.file = $$PWD/ev3/ev3GeneratorBase/ev3GeneratorBase.pro
ev3RbfGenerator.file = $$PWD/ev3/ev3RbfGenerator/ev3RbfGenerator.pro

trikGeneratorBase.depends = generatorBase
trikQtsGenerator.depends = trikGeneratorBase
trikFSharpGenerator.depends = trikGeneratorBase
trikRuntimeUploaderPlugin.depends = trikGeneratorBase
nxtGeneratorBase.depends = generatorBase
nxtOsekCGenerator.depends = nxtGeneratorBase
nxtRussianCGenerator.depends = nxtGeneratorBase
ev3GeneratorBase.depends = generatorBase
ev3RbfGenerator.depends = ev3GeneratorBase
