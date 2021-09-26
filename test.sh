#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./chibicc "$input" >tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

# assert 0 '0;'
# assert 42 '42;'
# assert 21 '5+20-4;'
# assert 41 ' 12 + 34 - 5 ;'
# assert 47 '5+6*7;'
# assert 15 '5*(9-6);'
# assert 4 '(3+5)/2;'
# assert 10 '-10+20;'
# assert 10 '- -10;'
# assert 10 '- - +10;'

# assert 0 '0==1;'
# assert 1 '42==42;'
# assert 1 '0!=1;'
# assert 0 '42!=42;'

# assert 1 '0<1;'
# assert 0 '1<1;'
# assert 0 '2<1;'
# assert 1 '0<=1;'
# assert 1 '1<=1;'
# assert 0 '2<=1;'

# assert 1 '1>0;'
# assert 0 '1>1;'
# assert 0 '1>2;'
# assert 1 '1>=0;'
# assert 1 '1>=1;'
# assert 0 '1>=2;'
# assert 3 'a = 3; a;'
# assert 22 'a = 3; b = 5 * 6 - 8; b;'
# assert 14 'a = 3; b = 5 * 6 - 8; a + b / 2;'
# assert 14 'foo = 3; bar = 5 * 6 - 8; foo + bar / 2;'
# assert 3 'return 3;'
# assert 3 'return 3; return 5;'
# assert 3 'foo = 3; return foo; bar = 5 * 6 - 8; foo + bar / 2;'
# assert 2 'if (0) return 1; return 2;'
# assert 1 'if (1) return 1; return 2;'
# assert 2 'if (0) return 1; else return 2;'
# assert 1 'if (1) return 1; else return 2;'
# assert 3 'if (0) return 1; else 2; return 3;'
# assert 10 'i = 0; while (i < 10) i = i + 1; return i;'
# assert 10 'i = 0; for (;i < 10;) i = i + 1; return i;'
# assert 10 'i = 0; sum = 0; for (;i < 5; i = i+1) sum = sum + i; return sum;'
# assert 10 'sum = 0; for (i = 1; i < 5; i = i+1) sum = sum + i; return sum;'
# assert 2 'if (0) {return 1;} return 2;'
# assert 1 'if (1) { return 1; } return 2;'
# assert 2 'if (0) { return 1; } else { return 2; }'
# assert 1 'if (1) { return 1; } else { return 2; }'
# assert 3 'if (0) { return 1; } else 2; return 3;'
# assert 55 'sum = 0; i = 0; while (i < 10) { i = i + 1; sum = sum + i; } return sum;'
# assert 55 'sum = 0; for (i = 0;i < 10;) { i = i + 1 ; sum = sum + i; } return sum;'
# assert 10 'i = 0; sum = 0; for (;i < 5; i = i+1) { sum = sum + i; } return sum;'
# assert 10 'sum = 0; for (i = 0; i < 5; i = i+1)  { sum = sum + i; } return sum;'
assert 1 'foo(); return 1;'

echo OK
