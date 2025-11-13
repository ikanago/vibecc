#!/bin/bash
COLOR_VALID="\e[32;1m"
COLOR_INVALID="\e[31;1m"
COLOR_OFF="\e[m"

function run() {
    expected="$1"
    code="$2"
    ./bin/vibecc -c "${code}" > ./bin/tmp.s
    gcc -static -o ./bin/tmp ./bin/tmp.s
    ./bin/tmp
    actual="$?"

    if [ "$actual" -ne "$expected" ]; then
        echo -e "${COLOR_INVALID}⛔ Failed: ${code}"
		echo -e "Expected: ${expected}"
		echo -e "  Actual: ${actual}${COLOR_OFF}"
		exit 1
    fi
}

run 42 '{ return 42; }'
run 3 '{ return 1 + 2; }' 
run 6 '{ return 1 + 2 + 3; }'
run 7 '{ return 1 + 2 * 3; }'
run 6 '{ return 2 * 3; }'
run 24 '{ return 2 * 3 * 4; }'
run 1 '{ int a = 1; return a; }'
run 3 '{ int a = 1; int b = 2; return b; }'
echo -e "${COLOR_VALID}🎉 All tests passed.${COLOR_OFF}"
