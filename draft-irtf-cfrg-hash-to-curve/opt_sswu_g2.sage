#!/usr/bin/env sage
# vim: syntax=python
#
# (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

from hash_to_base import *
from utils import *

# BLS12-381 G1 curve
p = 0x1a0111ea397fe69a4b1ba7b6434bacd764774b84f38512bf6730d2a0f6b0f6241eabfffeb153ffffb9feffffffffaaab
F = GF(p)
Ell = EllipticCurve(F, [0,4])
h = 3 * 11**2 * 10177**2 * 859267**2 * 52437899**2 # co-factor for G1
assert Ell.order() % h == 0
q = Ell.order() // h

# BLS12-381 G2 curve
F2.<X> = GF(p^2, modulus=[1, 0, 1])
Ell2 = EllipticCurve(F2, [0, 4 * (1 + X)])
h2 = Ell2.order() // q
assert Ell2.order() % q == 0

# 3-isogenous curve to Ell2
Ell2p_a = F2(240 * X)
Ell2p_b = F2(1012 * (1 + X))
Ell2p = EllipticCurve(F2, [Ell2p_a, Ell2p_b])
# isogeny map back to Ell2
iso2 = EllipticCurveIsogeny(Ell2p, [6 * (1 - X), 1], codomain=Ell2)

h2c_suite = "H2C-BLS12_381_2-SHA512-OSSWU-"

##
## clear cofactor via untwist-Frobenius-twist endomorphism
##
# this is based on
#   Budroni and Pintore, "Efficient Hash Maps to G2 on BLS curves."
#   ePrint 2017/419, https://eprint.iacr.org/2017/419
##
# constants for Psi, the untwist-Frobenius-twist map
iwsc_0 = 0xd0088f51cbff34d258dd3db21a5d66bb23ba5c279c2895fb39869507b587b120f55ffff58a9ffffdcff7fffffffd556
iwsc = F2(iwsc_0 * (1 + X) - X)
k_qi_x = 0x1a0111ea397fe699ec02408663d4de85aa0d857d89759ad4897d29650fb85f9b409427eb4f49fffd8bfd00000000aaad
k_qi_y = 0x6af0e0437ff400b6831e36d6bd17ffe48395dabc2d3435e77f76e17009241c5ee67992f72ec05f4c81084fbede3cc09
k_cx = F2(X * 0x1a0111ea397fe699ec02408663d4de85aa0d857d89759ad4897d29650fb85f9b409427eb4f49fffd8bfd00000000aaad)
k_cy = 0x135203e60180a68ee2e9c448d77a2cd91c3dedd930b1cf60ef396489f61eb45e304466cf3e67fa0af1ee7b04121bdea2
k_cy = F2(k_cy * (1 - X))
onei = F2(1 + X)
ell2_x = - 0xd201000000010000

# shortcut for evaluating untwist without resorting to Fp12 arithmetic --- X coordinate
def qi_x(x):
    vec = x._vector_()
    return F2(k_qi_x * (vec[0] - X * vec[1]))

# shortcut for evaluating untwist without resorting to Fp12 arithmetic --- Y coordinate
def qi_y(y):
    vec = y._vector_()
    return k_qi_y * F2(vec[0] + vec[1] + X * (vec[0] - vec[1]))

# untwist-Frobenius-twist
def psi(P):
    x = onei * qi_x(iwsc * P[0])
    y = onei * qi_y(iwsc * P[1])
    return Ell2(x, y)

# construction for Barreto-Lynn-Scott curves with embedding degree 12,
# given in section 4.1 of Budroni and Pintore
def clear_h2(P):
    pP = psi(P)
    pp2P = psi(psi(2 * P))
    first = (ell2_x ** 2 - ell2_x - 1) * P
    second = (ell2_x - 1) * pP
    return first + second + pp2P

##
## SWU map for G2
##
# xi is the distinguished non-square for the SWU map
xi_2 = F2(1 + X)
# roots of unity for use in computing square roots
roots1 = (F2(1)
  , F2(X)
  , F2(1028732146235106349975324479215795277384839936929757896155643118032610843298655225875571310552543014690878354869257*X
      + 1028732146235106349975324479215795277384839936929757896155643118032610843298655225875571310552543014690878354869257)
  , F2(2973677408986561043442465346520108879172042883009249989176415018091420807192182638567116318576472649347015917690530*X
      + 1028732146235106349975324479215795277384839936929757896155643118032610843298655225875571310552543014690878354869257)
  )
# eta values for converting a failed attempt at sqrt(g(X0(t))) to sqrt(g(X1(t)))
etas = (F2(426061185569912361983521454249761337083267257081408520893788542915383290290183480196466860748572708974347122096641)
   , F2(426061185569912361983521454249761337083267257081408520893788542915383290290183480196466860748572708974347122096641*X)
   , F2(1288825690270127928862280779549770369920038885627059587892741294824265852728860506840371064237610802480748737721626*X
       + 1288825690270127928862280779549770369920038885627059587892741294824265852728860506840371064237610802480748737721626)
   , F2(2713583864951539464555509046186133786636843934311948297439316841299765797761977357602316564891404861557145534838161*X
       + 1288825690270127928862280779549770369920038885627059587892741294824265852728860506840371064237610802480748737721626)
   )

# we define an element to be negative if its first coordinate in the canonical power basis is negative
def is_negative(x):
    return x._vector_()[0] > (p-1) // 2

# y^2 = g2p(x) is the curve equation for Ell2p
def g2p(x):
    return F2(x**3 + Ell2p_a * x + Ell2p_b)

# apply optimized simplified SWU map to a point t
def osswu2_help(t):
    # compute the value X0(t)
    num_den_common = F2(xi_2 ** 2 * t ** 4 + xi_2 * t ** 2)
    if num_den_common == 0:
        # exceptional case: use x0 = Ell2p_b / (xi_2 * Ell2p_a), which is square by design
        x0 = F2(Ell2p_b) / F2(xi_2 * Ell2p_a)
    else:
        x0 = F2(-Ell2p_b * (num_den_common + 1)) / F2(Ell2p_a * num_den_common)

    # g(X0), where y^2 = g(x) is the curve 3-isogenous to BLS12-381 Ell2
    gx0 = g2p(x0)

    # check whether gx0 is square by computing gx0 ^ ((p+1)/4)
    sqrt_candidate = gx0 ** ((p**2 + 7) // 16)
    # the square root will be given by sqrt_candidate times a root of unity; check them all
    for root_of_unity in roots1:
        y0_candidate = sqrt_candidate * root_of_unity
        if y0_candidate ** 2 == gx0:
            # found y0
            negate = -1 if is_negative(t) else 1
            y0 = y0_candidate * negate
            return iso2(Ell2p(x0, y0))

    # if we got here, the g(X0(t)) was not square
    # X1(t) == xi t^2 X0(t), g(X1(t)) = xi^2 t^6 X0(t)
    x1 = F2(xi_2 * t ** 2 * x0)
    gx1 = xi_2 ** 3 * t ** 6 * gx0
    assert gx1 == g2p(x1)

    # if g(X0(t)) is not square, then sqrt(g(X1(t))) == eta * t^3 * g(X0(t)) ^ ((p+7)/16) for one of the etas above
    for eta_value in etas:
        y1_candidate = sqrt_candidate * eta_value * t ** 3
        if y1_candidate ** 2 == gx1:
            # found y1
            # don't need to negate because t^3 preserves the sign of t
            y1 = y1_candidate
            return iso2(Ell2p(x1, y1))

    # if we got here, something went very wrong
    assert False, "osswu2_help failed"

# map from a string, optionally clearing the cofactor
def map2curve_osswu2(alpha, clear=False):
    # XXX how do we actually want to handle hashing to an element of Fp2?
    t1 = h2b_from_label(h2c_suite + "coord1", alpha)
    t2 = h2b_from_label(h2c_suite + "coord2", alpha)
    t = F2(t1 + X * t2)
    tv("t1 ", t1, 48)
    tv("t2 ", t2, 48)
    P = osswu2_help(t)
    if clear:
        return clear_h2(P)
    return P

if __name__ == "__main__":
    enable_debug()
    print "## Optimized Simplified SWU to BLS12-381 G2"
    for alpha in map2curve_alphas:
        tv_text("alpha", pprint_hex(alpha))
    for alpha in map2curve_alphas:
        print "\n~~~"
        print("Input:")
        print("")
        tv_text("alpha", pprint_hex(alpha))
        print("")
        P = map2curve_osswu2(alpha, False)
        Pc = map2curve_osswu2(alpha, True)
        assert P * h2 * (3 * ell2_x ** 2 - 3) == Pc  # make sure fast cofactor clear method worked
        assert Pc * q == Ell2(0,1,0)                 # make sure that Pc is of the correct order
        print("Output:")
        print("")
        vec = Pc[0]._vector_()
        tv("x1 ", vec[0], 48)
        tv("x2 ", vec[1], 48)
        vec = Pc[1]._vector_()
        tv("y1 ", vec[0], 48)
        tv("y2 ", vec[1], 48)
        print "~~~"
