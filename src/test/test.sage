#!/usr/bin/env sage
# vim: syntax=python

import sys

p = 0x1a0111ea397fe69a4b1ba7b6434bacd764774b84f38512bf6730d2a0f6b0f6241eabfffeb153ffffb9feffffffffaaab
F = GF(p)
Ell = EllipticCurve(F, [0, 4])
h = 3 * 11**2 * 10177**2 * 859267**2 * 52437899**2
gPrime = Ell(0x12d016e80c318f8a0097be925a89b9407479550d744e573a9438f1b2b41e750a2a5f1885c1ec0f54835eb4f7069257d2,
             0xd2553c973f28a7b1d2a781cfe09e8cb405529cdd1074ff20408a2943bef24938df961937462bbdea85bd083a1c43bc)

cx1 = F((F(3) + sqrt(F(-27))) / F(2))
cx2 = F((F(3) - sqrt(F(-27))) / F(2))

def svdw(t):
    x12val = F(t ** 2) * sqrt(F(-27)) / F(23 - t ** 2)

    x1 = F(cx1 + x12val)
    x2 = F(cx2 - x12val)
    x3 = F(-3 - (23 - t ** 2) ** 2 / F(27 * t ** 2))

    fx1 = F(x1 ** 3 + 4)
    fx2 = F(x2 ** 3 + 4)
    fx3 = F(x3 ** 3 + 4)

    negate = 1 if t < (p + 1) / 2 else -1
    if fx1.is_square():
        y = pow(fx1, (p+1)//4, p)
        return Ell(x1, y * negate)

    if fx2.is_square():
        y = pow(fx2, (p+1)//4, p)
        return Ell(x2, y * negate)

    y = pow(fx3, (p+1)//4, p)
    return Ell(x3, y * negate)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: %s <type>\n<type> is one of 'try', '1', '2', 'rG'\n")
        sys.exit(1)

    if sys.argv[1] == "try":
        assert all( Ell(xOut, yOut) == h * Ell(xIn, yIn)
                    for (xIn, yIn, xOut, yOut) in ( eval(l) for l in sys.stdin.readlines() ) )

    elif sys.argv[1] == "1":
        assert all( Ell(xOut, yOut) == h * svdw(t)
                    for (t, xOut, yOut) in ( eval(l) for l in sys.stdin.readlines() ) )

    elif sys.argv[1] == "2":
        assert all( Ell(xOut, yOut) == h * (svdw(t1) + svdw(t2))
                    for (t1, t2, xOut, yOut) in ( eval(l) for l in sys.stdin.readlines() ) )

    elif sys.argv[1] == "rG":
        assert all( Ell(xOut, yOut) == h * svdw(t) + r * gPrime
                    for (t, r, xOut, yOut) in ( eval(l) for l in sys.stdin.readlines() ) )
