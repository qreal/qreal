#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"
source $INSTALLER_ROOT/utils/mac_utils.sh

mkdir -p "$BUNDLE_CONTENTS/Lib/"

cp    $BIN_DIR/librobots-pioneer-kit*.dylib                          		"$BUNDLE_CONTENTS/Lib/"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/librobots-pioneer-kit.dylib"


mkdir -p "$BUNDLE_CONTENTS/Lib/plugins/tools/kitPlugins"
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-pioneer-interpreter.dylib	"$BUNDLE_CONTENTS/Lib/plugins/tools/kitPlugins/"
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/plugins/tools/kitPlugins/librobots-pioneer-interpreter.dylib"
cp     $BIN_DIR/plugins/tools/kitPlugins/librobots-pioneer-lua-generator.dylib	"$BUNDLE_CONTENTS/Lib/plugins/tools/kitPlugins/" 
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/plugins/tools/kitPlugins/librobots-pioneer-lua-generator.dylib"


mkdir -p "$BUNDLE_CONTENTS/Lib/plugins/editors"
cp $BIN_DIR/plugins/editors/libpioneerMetamodel.dylib				"$BUNDLE_CONTENTS/Lib/plugins/editors/" 
fix_qreal_dependencies "$BUNDLE_CONTENTS/Lib/plugins/editors/libpioneerMetamodel.dylib"
