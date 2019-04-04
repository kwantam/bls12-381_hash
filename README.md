# hashing to BLS12-381

We describe and implement maps to [BLS12-381](https://z.cash/blog/new-snark-curve/) based
on [SvdW06](#bib) and and the "simplified SWU" map of [BCIMRT10](#bib).

The SvdW map is similar to the one described in [FT12](#bib), except that our map is defined
at every point in the base field. This may simplify constant-time implementations.

The SWU map uses two new tricks to speed up evaluation. It also uses only field operations,
and in particular does not require fast Legendre symbol or extended Euclidean algorithms, which
the SvdW map requires for efficiency. This simplifies implementation---especially constant-time
implementation---since both of those algorithms would require implementing arbitrary modular
reductions rather than reductions modulo a fixed prime.  It is also faster by about 10% than
the SvdW map when both are restricted to field ops.

The [paper](paper/) subdirectory derives the maps, describes optimizations, and briefly
surveys approaches to hashing to elliptic curves.

The [src](src/) subdirectory implements seven hashes to the G1 subgroup of the BLS12-381
curve.

- "hash-and-try", as described in [BLS03](#bib)

- one evaluation of the SvdW map followed by a point multiplication to clear the cofactor

- sum of two evaluations of the SvdW map followed by a point multiplication to clear the cofactor

- sum of one evaluation of the SvdW map with cofactor cleared plus one random element of the G1 subgroup

- one evaluation of the SWU map followed by a point multiplication to clear the cofactor

- sum of two evaluations of the SWU map followed by a point multiplication to clear the cofactor

- sum of one evaluation of the SWU map with cofactor cleared plus one random element of the G1 subgroup

For several of these hashes, we've also implemented constant-time versions.

### <a name="bib">bibliography</a>

BCIMRT10: Brier, Coron, Icart, Madore, Randriam, Tibouchi.
["Efficient Indifferentiable Hashing into Ordinary Elliptic Curves."](https://eprint.iacr.org/2009/340)
Proc. CRYPTO, 2010.

BLS01: Boneh, Lynn, and Shacham.
["Short signatures from the Weil pairing."](https://hovav.net/ucsd/dist/sigs.pdf)
Proc. ASIACRYPT, 2001.

FT12: Fouque and Tibouchi,
["Indifferentiable hashing to Barreto-Naehrig curves."](https://link.springer.com/chapter/10.1007/978-3-642-33481-8_1)
Proc.  LATINCRYPT, 2012.

SvdW06: Shallue and van de Woestijne,
["Construction of rational points on elliptic curves over finite fields."](https://works.bepress.com/andrew_shallue/1/download/)
Proc. ANTS 2006.
