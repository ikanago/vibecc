#!/bin/bash
COLOR_VALID="\e[32;1m"
COLOR_INVALID="\e[31;1m"
COLOR_OFF="\e[m"

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

function run() {
    expected="$1"
    target="$2"
    ./bin/vibecc "${SCRIPT_DIR}/${target}" > ./bin/tmp.s
    gcc -static -o ./bin/tmp ./bin/tmp.s
    ./bin/tmp
    actual="$?"

    if [ "$actual" -ne "$expected" ]; then
        echo -e "${COLOR_INVALID}Failed:${COLOR_OFF} ${target}"
		echo -e "${COLOR_INVALID}⛔  ${expected} expected, but got ${actual} ⛔${COLOR_OFF}"
		exit 1
    else
        echo "Test ${target} passed."
    fi
}

run 42 "integer.c"
run 6 "multiplication_simple.c"
echo -e "${COLOR_VALID}All tests passed.${COLOR_OFF}"
