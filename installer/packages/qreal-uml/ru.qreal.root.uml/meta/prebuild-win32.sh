
#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/plugins/editors/classMetamodel.dll                    $PWD/../data/plugins/editors/
