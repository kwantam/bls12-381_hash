#ifndef __bls_hash__src__curve__iso_params_h__
// clang-format off

#include <stdint.h>

static const uint64_t ELLP_a[] = { 0x5cf428082d584c1dLL, 0x98936f8da0e0f97fLL, 0xd8e8981aefd881acLL, 0xb0ea985383ee66a8LL, 0x3d693a02c96d4982LL, 0x144698a3b8e943LL, };

static const uint64_t ELLP_negB[] = { 0xe832b71671e87ecbLL, 0xc488dea47fe71559LL, 0xc677115127bd972eLL, 0x446089940135d24eLL, 0x499094cdce0cbe9cLL, 0x71e815d2817666aLL, };

#define ELLP_XMAP_NUM_LEN 12
static const uint64_t ELLP_XMAP_NUM[][6] = {
    { 0xaeac1662734649b7LL, 0x5610c2d5f2e62d6eLL, 0xf2627b56cdb4e2c8LL, 0x6b303e88a2d7005fLL, 0xb809101dd9981585LL, 0x11a05f2b1e833340LL, },
    { 0xe834eef1b3cb83bbLL, 0x4838f2a6f318c356LL, 0xf565e33c70d1e86bLL, 0x7c17e75b2f6a8417LL, 0x588bab22147a81cLL, 0x17294ed3e943ab2fLL, },
    { 0xe0179f9dac9edcb0LL, 0x958c3e3d2a09729fLL, 0x6878e501ec68e25cLL, 0xce032473295983e5LL, 0x1d1048c5d10a9a1bLL, 0xd54005db97678ecLL, },
    { 0xc5b388641d9b6861LL, 0x5336e25ce3107193LL, 0xf1b33289f1b33083LL, 0xd7f5e4656a8dbf25LL, 0x4e0609d307e55412LL, 0x1778e7166fcc6db7LL, },
    { 0x51154ce9ac8895d9LL, 0x985a286f301e77c4LL, 0x86eeb65982fac18LL, 0x99db995a1257fb3fLL, 0x6642b4b3e4118e54LL, 0xe99726a3199f443LL, },
    { 0xcd13c1c66f652983LL, 0xa0870d2dcae73d19LL, 0x9ed3ab9097e68f90LL, 0xdb3cb17dd952799bLL, 0x1d1201bf7a74ab5LL, 0x1630c3250d7313ffLL, },
    { 0xddd7f225a139ed84LL, 0x8da25128c1052ecaLL, 0x9008e218f9c86b2aLL, 0xb11586264f0f8ce1LL, 0x6a3726c38ae652bfLL, 0xd6ed6553fe44d29LL, },
    { 0x9ccb5618e3f0c88eLL, 0x39b7c8f8c8f475afLL, 0xa682c62ef0f27533LL, 0x356de5ab275b4db1LL, 0xe8743884d1117e53LL, 0x17b81e7701abdbe2LL, },
    { 0x6d71986a8497e317LL, 0x4fa295f296b74e95LL, 0xa2c596c928c5d1deLL, 0xc43b756ce79f5574LL, 0x7b90b33563be990dLL, 0x80d3cf1f9a78fc4LL, },
    { 0x7f241067be390c9eLL, 0xa3190b2edc032779LL, 0x676314baf4bb1b7fLL, 0xdd2ecb803a0c5c99LL, 0x2e0c37515d138f22LL, 0x169b1f8e1bcfa7c4LL, },
    { 0xca67df3f1605fb7bLL, 0xf69b771f8c285decLL, 0xd50af36003b14866LL, 0xfa7dccdde6787f96LL, 0x72d8ec09d2565b0dLL, 0x10321da079ce07e2LL, },
    { 0xa9c8ba2e8ba2d229LL, 0xc24b1b80b64d391fLL, 0x23c0bf1bc24c6b68LL, 0x31d79d7e22c837bcLL, 0xbd1e962381edee3dLL, 0x6e08c248e260e70LL, },
};

#define ELLP_XMAP_DEN_LEN 10
static const uint64_t ELLP_XMAP_DEN[][6] = {
    { 0x993cf9fa40d21b1cLL, 0xb558d681be343df8LL, 0x9c9588617fc8ac62LL, 0x1d5ef4ba35b48baLL, 0x18b2e62f4bd3fa6fLL, 0x8ca8d548cff19aeLL, },
    { 0xe5c8276ec82b3bffLL, 0x13daa8846cb026e9LL, 0x126c2588c48bf57LL, 0x7041e8ca0cf0800cLL, 0x48b4711298e53636LL, 0x12561a5deb559c43LL, },
    { 0xfcc239ba5cb83e19LL, 0xd6a3d0967c94fedcLL, 0xfca64e00b11aceacLL, 0x6f89416f5a718cd1LL, 0x8137e629bff2991fLL, 0xb2962fe57a3225eLL, },
    { 0x130de8938dc62cd8LL, 0x4976d5243eecf5c4LL, 0x54cca8abc28d6fd0LL, 0x5b08243f16b16551LL, 0xc83aafef7c40eb54LL, 0x3425581a58ae2feLL, },
    { 0x539d395b3532a21eLL, 0x9bd29ba81f35781dLL, 0x8d6b44e833b306daLL, 0xffdfc759a12062bbLL, 0xa6f1d5f43e7a07dLL, 0x13a8e162022914a8LL, },
    { 0xc02df9a29f6304a5LL, 0x7400d24bc4228f11LL, 0xa43bcef24b8982fLL, 0x395735e9ce9cad4dLL, 0x55390f7f0506c6e9LL, 0xe7355f8e4e667b9LL, },
    { 0xec2574496ee84a3aLL, 0xea73b3538f0de06cLL, 0x4e2e073062aede9cLL, 0x570f5799af53a189LL, 0xf3e0c63e0596721LL, 0x772caacf1693619LL, },
    { 0x11f7d99bbdcc5a5eLL, 0xfa5b9489d11e2d3LL, 0x1996e1cdf9822c58LL, 0x6e7f63c21bca68a8LL, 0x30b3f5b074cf0199LL, 0x14a7ac2a9d64a8b2LL, },
    { 0x4776ec3a79a1d641LL, 0x3826692abba4370LL, 0x74100da67f398835LL, 0xe07f8d1d7161366bLL, 0x5e920b3dafc7a3ccLL, 0xa10ecf6ada54f82LL, },
    { 0x2d6384d168ecdd0aLL, 0x93174e4b4b786500LL, 0x76df533978f31c15LL, 0xf682b4ee96f7d037LL, 0x476d6e3eb3a56680LL, 0x95fc13ab9e92ad4LL, },
};

#define ELLP_YMAP_NUM_LEN 16
static const uint64_t ELLP_YMAP_NUM[][6] = {
    { 0xbe9845719707bb33LL, 0xcd0c7aee9b3ba3c2LL, 0x2b52af6c956543d3LL, 0x11ad138e48a86952LL, 0x259d1f094980dcfaLL, 0x90d97c81ba24ee0LL, },
    { 0xe097e75a2e41c696LL, 0xd6c56711962fa8bfLL, 0xf906343eb67ad34LL, 0x1223e96c254f383dLL, 0xd51036d776fb4683LL, 0x134996a104ee5811LL, },
    { 0xb8dfe240c72de1f6LL, 0xd26d521628b00523LL, 0xc344be4b91400da7LL, 0x2552e2d658a31ce2LL, 0xf4a384c86a3b4994LL, 0xcc786baa966e66LL, },
    { 0xa6355c77b0e5f4cbLL, 0xde405aba9ec61decLL, 0x9e4a3ec03251cf9LL, 0xd42aa7b90eeb791cLL, 0x7898751ad8746757LL, 0x1f86376e8981c21LL, },
    { 0x41b6daecf2e8fedbLL, 0x2ee7f8dc099040a8LL, 0x79833fd221351adcLL, 0x195536fbe3ce50b8LL, 0x5caf4fe2a21529c4LL, 0x8cc03fdefe0ff13LL, },
    { 0x99b23ab13633a5f0LL, 0x203f6326c95a8072LL, 0x76505c3d3ad5544eLL, 0x74a7d0d4afadb7bdLL, 0x2211e11db8f0a6a0LL, 0x16603fca40634b6aLL, },
    { 0xc961f8855fe9d6f2LL, 0x47a87ac2460f415eLL, 0x5231413c4d634f37LL, 0xe75bb8ca2be184cbLL, 0xb2c977d027796b3cLL, 0x4ab0b9bcfac1bbcLL, },
    { 0xa15e4ca31870fb29LL, 0x42f64550fedfe935LL, 0xfd038da6c26c8426LL, 0x170a05bfe3bdd81fLL, 0xde9926bd2ca6c674LL, 0x987c8d5333ab86fLL, },
    { 0x60370e577bdba587LL, 0x69d65201c78607a3LL, 0x1e8b6e6a1f20cabeLL, 0x8f3abd16679dc26cLL, 0xe88c9e221e4da1bbLL, 0x9fc4018bd96684bLL, },
    { 0x2bafaaebca731c30LL, 0x9b3f7055dd4eba6fLL, 0x6985e7ed1e4d43bLL, 0xc42a0ca7915af6feLL, 0x223abde7ada14a23LL, 0xe1bba7a1186bdb5LL, },
    { 0xe813711ad011c132LL, 0x31bf3a5cce3fbafcLL, 0xd1183e416389e610LL, 0xcd2fcbcb6caf493fLL, 0xdfd0b8f1d43fb93LL, 0x19713e47937cd1beLL, },
    { 0xce07c8a4d0074d8eLL, 0x49d9cdf41b44d606LL, 0x2e6bfe7f911f6432LL, 0x523559b8aaf0c246LL, 0xb918c143fed2edccLL, 0x18b46a908f36f6deLL, },
    { 0xd4c04f00b971ef8LL, 0x6c851c1919211f2LL, 0xc02710e807b4633fLL, 0x7aa7b12a3426b08eLL, 0xd155096004f53f44LL, 0xb182cac101b9399LL, },
    { 0x42d9d3f5db980133LL, 0xc6cf90ad1c232a64LL, 0x13e6632d3c40659cLL, 0x757b3b080d4c1580LL, 0x72fc00ae7be315dcLL, 0x245a394ad1eca9bLL, },
    { 0x866b1e715475224bLL, 0x6ba1049b6579afb7LL, 0xd9ab0f5d396a7ce4LL, 0x5e673d81d7e86568LL, 0x2a159f748c4a3fcLL, 0x5c129645e44cf11LL, },
    { 0x4b456be69c8b604LL, 0xb665027efec01c77LL, 0x57add4fa95af01b2LL, 0xcb181d8f84965a39LL, 0x4ea50b3b42df2eb5LL, 0x15e6be4e990f03ceLL, },
};

#define ELLP_YMAP_DEN_LEN 15
static const uint64_t ELLP_YMAP_DEN[][6] = {
    { 0x1479253b03663c1LL, 0x7f3688ef60c206dLL, 0xeec3232b5be72e7aLL, 0x601a6de578980be6LL, 0x52181140fad0eae9LL, 0x16112c4c3a9c98b2LL, },
    { 0x32f6102c2e49a03dLL, 0x78a4260763529e35LL, 0xa4a10356f453e01fLL, 0x85c84ff731c4d59cLL, 0x1a0cbd6c43c348b8LL, 0x1962d75c2381201eLL, },
    { 0x1e2538b53dbf67f2LL, 0xa6757cd636f96f89LL, 0xc35a5dd279cd2ecLL, 0x78c4855551ae7f31LL, 0x6faaae7d6e8eb157LL, 0x58df3306640da27LL, },
    { 0xa8d26d98445f5416LL, 0x727364f2c28297adLL, 0x123da489e726af41LL, 0xd115c5dbddbcd30eLL, 0xf20d23bf89edb4d1LL, 0x16b7d288798e5395LL, },
    { 0xda39142311a5001dLL, 0xa20b15dc0fd2ededLL, 0x542eda0fc9dec916LL, 0xc6d19c9f0f69bbb0LL, 0xb00cc912f8228ddcLL, 0xbe0e079545f43e4LL, },
    { 0x2c6477faaf9b7acLL, 0x49f38db9dfa9cce2LL, 0xc5ecd87b6f0f5a64LL, 0xb70152c65550d881LL, 0x9fb266eaac783182LL, 0x8d9e5297186db2dLL, },
    { 0x3d1a1399126a775cLL, 0xd5fa9c01a58b1fb9LL, 0x5dd365bc400a0051LL, 0x5eecfdfa8d0cf8efLL, 0xc3ba8734ace9824bLL, 0x166007c08a99db2fLL, },
    { 0x60ee415a15812ed9LL, 0xb920f5b00801dee4LL, 0xfeb34fd206357132LL, 0xe5a4375efa1f4fd7LL, 0x3bcddfabba6ff6eLL, 0x16a3ef08be3ea7eaLL, },
    { 0x6b233d9d55535d4aLL, 0x52cfe2f7bb924883LL, 0xabc5750c4bf39b48LL, 0xf9fb0ce4c6af5920LL, 0x1a1be54fd1d74cc4LL, 0x1866c8ed336c6123LL, },
    { 0x346ef48bb8913f55LL, 0xc7385ea3d529b35eLL, 0x5308592e7ea7d4fbLL, 0x3216f763e13d87bbLL, 0xea820597d94a8490LL, 0x167a55cda70a6e1cLL, },
    { 0xf8b49cba8f6aa8LL, 0x71a5c29f4f830604LL, 0xe591b36e636a5c8LL, 0x9c6dd039bb61a629LL, 0x48f010a01ad2911dLL, 0x4d2f259eea405bdLL, },
    { 0x9684b529e2561092LL, 0x16f968986f7ebbeaLL, 0x8c0f9a88cea79135LL, 0x7f94ff8aefce42d2LL, 0xf5852c1e48c50c47LL, 0xaccbb67481d033fLL, },
    { 0x1e99b138573345ccLL, 0x93000763e3b90ac1LL, 0x7d5ceef9a00d9b86LL, 0x543346d98adf0226LL, 0xc3613144b45f1496LL, 0xad6b9514c767fe3LL, },
    { 0xd1fadc1326ed06f7LL, 0x420517bd8714cc80LL, 0xcb748df27942480eLL, 0xbf565b94e72927c1LL, 0x628bdd0d53cd76f2LL, 0x2660400eb2e4f3bLL, },
    { 0x4415473a1d634b8fLL, 0x5ca2f570f1349780LL, 0x324efcd6356caa20LL, 0x71c40f65e273b853LL, 0x6b24255e0d7819c1LL, 0xe0fa1d816ddc03eLL, },
};

// clang-format on
#define __bls_hash__src__curve__iso_params_h__
#endif  // __bls_hash__src__curve__iso_params_h__
