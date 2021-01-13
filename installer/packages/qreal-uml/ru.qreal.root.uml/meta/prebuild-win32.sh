
#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"

cp    $BIN_DIR/plugins/editors/editor.dll                    $PWD/../data/plugins/editors/
