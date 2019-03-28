#!/usr/bin/env sage
# vim: syntax=python

import sys

p = 0x1a0111ea397fe69a4b1ba7b6434bacd764774b84f38512bf6730d2a0f6b0f6241eabfffeb153ffffb9feffffffffaaab
F = GF(p)
Ell = EllipticCurve(F, [0, 4])
h = 3 * 11**2 * 10177**2 * 859267**2 * 52437899**2
gPrime = Ell(0x12d016e80c318f8a0097be925a89b9407479550d744e573a9438f1b2b41e750a2a5f1885c1ec0f54835eb4f7069257d2,
             0xd2553c973f28a7b1d2a781cfe09e8cb405529cdd1074ff20408a2943bef24938df961937462bbdea85bd083a1c43bc)

EllP_a = 12190336318893619529228877361869031420615612348429846051986726275283378313155663745811710833465465981901188123677
EllP_b = 2906670324641927570491258158026293881577086121416628140204402091718288198173574630967936031029026176254968826637280
kpoly = [ 2955185177626169647543871026716143749798776473583611826109464091094397474242531465369792566109106337274460167892487
        , 368290328279919699380600918531504139062319804596715301915067438724165980997443279928362378653594520746075110864816
        , 2956695361339193066682181833380513374227982887255347166494009902960246484641139874171795235175561134113346246778169
        , 1426262776564302641609273031767820829543220110075666692709214249985201803031422328413319637086750536774929261013316
        , 721398571713745716315313195033211010796752582794315199168745550044278639029030032465331939076562082409261408087685
        , 1
        ]
EllP = EllipticCurve(F, [EllP_a, EllP_b])
# this is the isogeny, but with the opposite sign for y (Sage, you are weird)
iso = EllipticCurveIsogeny(EllP, kpoly, codomain=Ell, degree=11)

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

    negate = 1 if t < (p + 1) // 2 else -1
    if fx1.is_square():
        y = pow(fx1, (p+1)//4, p)
        return Ell(x1, y * negate)

    if fx2.is_square():
        y = pow(fx2, (p+1)//4, p)
        return Ell(x2, y * negate)

    y = pow(fx3, (p+1)//4, p)
    return Ell(x3, y * negate)

def swu(u):
    if u in (-1, 0, 1):
        return EllP(0, 1, 0)

    x0 = F(-EllP_b / F(EllP_a) * (1 + 1 / F(u^4 - u^2)))
    gx0 = F(x0^3 + EllP_a * x0 + EllP_b)
    sqrtCand = pow(gx0, (p+1)//4, p)

    if F(sqrtCand^2) == gx0:
        negate = 1 if u < (p + 1) // 2 else -1
        return EllP(x0, sqrtCand * negate)
    return EllP(F(-u^2 * x0), F(u^3 * sqrtCand))

def usage():
    print("Usage: %s <type>\n<type> is one of 'try', '1', '2', 'rG', 'u1', 'u2', 'urG'\n")
    sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        usage()

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

    elif sys.argv[1] == "u1":
        assert all( Ell(xOut, yOut) == -h * iso(swu(u))
                    for (xOut, yOut, u) in ( eval(l) for l in sys.stdin.readlines() ) )

    elif sys.argv[1] == "u2":
        assert all( Ell(xOut, yOut) == -h * iso(swu(u1) + swu(u2))
                    for (xOut, yOut, u1, u2) in ( eval(l) for l in sys.stdin.readlines() ) )

    elif sys.argv[1] == "urG":
        assert all( Ell(xOut, yOut) == -h * iso(swu(u)) + r * gPrime
                    for (xOut, yOut, u, r) in ( eval(l) for l in sys.stdin.readlines() ) )

    else:
        usage()
