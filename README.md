# hashing to BLS12-381

We describe and implement a map to [BLS12-381](https://z.cash/blog/new-snark-curve/)
based on [SvdW06](#bib). The map we construct is similar to the one described in
[FT12](#bib), except that our map is defined at every point in the base field.
This makes a constant time hash implementation possible (and relatively simple).

The [paper](paper/) subdirectory derives the map, describes optimizations, and briefly
surveys approaches to hashing to elliptic curves.

The [src](src/) subdirectory implements four hashes to the G1 subgroup of the BLS12-381
curve.

- "hash-and-try", as described in [BLS03](#bib)

- one evaluation of the SvdW map followed by a point multiplication to clear the cofactor

- sum of two evaluations of the SvdW map followed by a point multiplication to clear the cofactor

- sum of one evaluation of the SvdW map with cofactor cleared plus one random element of the G1 subgroup

Please note that these implementations are *not* constant time!

### <a name="bib">bibliography</a>

BLS01: Boneh, Lynn, and Shacham.
["Short signatures from the Weil pairing."](https://hovav.net/ucsd/dist/sigs.pdf)
Proc. ASIACRYPT, 2001.

FT12: Fouque and Tibouchi,
["Indifferentiable hashing to Barreto-Naehrig curves."](https://link.springer.com/chapter/10.1007/978-3-642-33481-8_1)
Proc.  LATINCRYPT, 2012.

SvdW06: Shallue and van de Woestijne,
["Construction of rational points on elliptic curves over finite fields."](https://works.bepress.com/andrew_shallue/1/download/)
Proc. ANTS 2006.
