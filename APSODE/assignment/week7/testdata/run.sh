#!/usr/bin/env bash
set -u

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BIN="${BIN:-$DIR/../../../../cmake-build-debug/bin/week7_ManageCircleMain}"
DATA="$DIR/std.txt"

if [[ ! -x "$BIN" ]]; then
    echo "binary not found: $BIN" >&2
    exit 1
fi

if [[ $# -eq 0 ]]; then
    set -- "$DIR"/cmd_*.txt
fi

work="$(mktemp)"
trap 'rm -f "$work"' EXIT

for cmd in "$@"; do
    if [[ ! -f "$cmd" && -f "$DIR/$cmd" ]]; then
        cmd="$DIR/$cmd"
    fi

    echo "===== $(basename "$cmd")"
    cp "$DATA" "$work"

    "$BIN" "$work" < "$cmd"

    echo "--- saved file"
    cat "$work"

    echo "--- reload"
    printf 'L\nQ\n' | "$BIN" "$work"
    echo
done
