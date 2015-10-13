#!/bin/bash
set -o nounset
set -o errexit


# Performs 'git checkout $1' if tag $1 exists or warns that $2 has no tag $1
function checkout_tag_or_warn { { PATTERN=$'\n'$1$'\n'; echo "$'\n'$(git tag)$'\n'" | grep -E $PATTERN; } && git checkout $1 || echo "Warning: $2 doesn't have tag "$1; }
