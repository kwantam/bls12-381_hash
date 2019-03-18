#ifndef _HAVE_BINT_CHAINS_H

/* *** addchain for 1000602388805416848354447456433976039139220704984751971333014534031007912622709466110671907282253916009473568139947 *** */
/* Bos-Coster (win=4) : 458 links, 16 variables */
#define _bint_chain_sqrt(TMPVAR1, TMPVAR0) do { \
        uint64_t TMPVAR2[NWORDS]; \
        uint64_t TMPVAR3[NWORDS]; \
        uint64_t TMPVAR4[NWORDS]; \
        uint64_t TMPVAR5[NWORDS]; \
        uint64_t TMPVAR6[NWORDS]; \
        uint64_t TMPVAR7[NWORDS]; \
        uint64_t TMPVAR8[NWORDS]; \
        uint64_t TMPVAR9[NWORDS]; \
        uint64_t TMPVAR10[NWORDS]; \
        uint64_t TMPVAR11[NWORDS]; \
        uint64_t TMPVAR12[NWORDS]; \
        uint64_t TMPVAR13[NWORDS]; \
        uint64_t TMPVAR14[NWORDS]; \
        uint64_t TMPVAR15[NWORDS]; \
        bint_sqr(TMPVAR1, TMPVAR0);                                              /*    0 : 2 */ \
        bint_mul(TMPVAR9, TMPVAR1, TMPVAR0);                                     /*    1 : 3 */ \
        bint_sqr(TMPVAR5, TMPVAR1);                                              /*    2 : 4 */ \
        bint_mul(TMPVAR13, TMPVAR9, TMPVAR1);                                    /*    3 : 5 */ \
        bint_mul(TMPVAR7, TMPVAR5, TMPVAR9);                                     /*    4 : 7 */ \
        bint_mul(TMPVAR10, TMPVAR13, TMPVAR5);                                   /*    5 : 9 */ \
        bint_mul(TMPVAR2, TMPVAR7, TMPVAR5);                                     /*    6 : 11 */ \
        bint_mul(TMPVAR4, TMPVAR10, TMPVAR5);                                    /*    7 : 13 */ \
        bint_mul(TMPVAR8, TMPVAR2, TMPVAR5);                                     /*    8 : 15 */ \
        bint_mul(TMPVAR15, TMPVAR4, TMPVAR5);                                    /*    9 : 17 */ \
        bint_mul(TMPVAR11, TMPVAR8, TMPVAR5);                                    /*   10 : 19 */ \
        bint_mul(TMPVAR3, TMPVAR15, TMPVAR5);                                    /*   11 : 21 */ \
        bint_mul(TMPVAR12, TMPVAR11, TMPVAR5);                                   /*   12 : 23 */ \
        bint_sqr(TMPVAR1, TMPVAR4);                                              /*   13 : 26 */ \
        bint_mul(TMPVAR14, TMPVAR12, TMPVAR5);                                   /*   14 : 27 */ \
        bint_mul(TMPVAR6, TMPVAR1, TMPVAR9);                                     /*   15 : 29 */ \
        bint_mul(TMPVAR5, TMPVAR1, TMPVAR13);                                    /*   16 : 31 */ \
        for (int nops = 0; nops < 12; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }    /*   17 : 106496 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR15);                                    /*   29 : 106513 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   30 : 13633664 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*   37 : 13633679 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   38 : 218138864 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR13);                                    /*   42 : 218138869 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   43 : 13960887616 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR7);                                     /*   49 : 13960887623 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   50 : 1786993615744 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR12);                                    /*   57 : 1786993615767 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   58 : 57183795704544 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*   63 : 57183795704575 */ \
        for (int nops = 0; nops < 2; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   64 : 228735182818300 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*   66 : 228735182818303 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   67 : 14639051700371392 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*   73 : 14639051700371405 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   74 : 936899308823769920 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*   80 : 936899308823769933 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   81 : 59961555764721275712 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR10);                                    /*   87 : 59961555764721275721 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   88 : 479692446117770205768 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*   91 : 479692446117770205771 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   92 : 61400633103074586338688 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*   99 : 61400633103074586338701 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  100 : 982410129649193381419216 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  104 : 982410129649193381419229 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  105 : 62874248297548376410830656 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  111 : 62874248297548376410830671 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  112 : 4023951891043096090293162944 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR14);                                    /*  118 : 4023951891043096090293162971 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  119 : 32191615128344768722345303768 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR0);                                     /*  122 : 32191615128344768722345303769 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  123 : 8241053472856260792920397764864 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  131 : 8241053472856260792920397764877 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  132 : 1054854844525601381493810913904256 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR12);                                    /*  139 : 1054854844525601381493810913904279 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  140 : 33755355024819244207801949244936928 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*  145 : 33755355024819244207801949244936939 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  146 : 2160342721588431629299324751675964096 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  152 : 2160342721588431629299324751675964109 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  153 : 138261934181659624275156784107261702976 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR6);                                     /*  159 : 138261934181659624275156784107261703005 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  160 : 2212190946906553988402508545716187248080 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR10);                                    /*  164 : 2212190946906553988402508545716187248089 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  165 : 566320882408077821031042187703343935510784 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR6);                                     /*  173 : 566320882408077821031042187703343935510813 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  174 : 9061134118529245136496675003253502968173008 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  178 : 9061134118529245136496675003253502968173021 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  179 : 1159825167171743377471574400416448379926146688 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR12);                                    /*  186 : 1159825167171743377471574400416448379926146711 */ \
        for (int nops = 0; nops < 9; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  187 : 593830485591932609265446093013221570522187116032 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR11);                                    /*  196 : 593830485591932609265446093013221570522187116051 */ \
        for (int nops = 0; nops < 2; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  197 : 2375321942367730437061784372052886282088748464204 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*  199 : 2375321942367730437061784372052886282088748464207 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  200 : 76010302155767373985977099905692361026839950854624 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR7);                                     /*  205 : 76010302155767373985977099905692361026839950854631 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  206 : 9729318675938223870205068787928622211435513709392768 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR13);                                    /*  213 : 9729318675938223870205068787928622211435513709392773 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  214 : 1245352790520092655386248804854863643063745754802274944 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR10);                                    /*  221 : 1245352790520092655386248804854863643063745754802274953 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  222 : 79702578593285929944719923510711273156079728307345596992 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR12);                                    /*  228 : 79702578593285929944719923510711273156079728307345597015 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  229 : 2550482514985149758231037552342760740994551305835059104480 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR6);                                     /*  234 : 2550482514985149758231037552342760740994551305835059104509 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  235 : 81615440479524792263393201674968343711825641786721891344288 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR11);                                    /*  240 : 81615440479524792263393201674968343711825641786721891344307 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  241 : 2611694095344793352428582453598986998778420537175100523017824 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR11);                                    /*  246 : 2611694095344793352428582453598986998778420537175100523017843 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  247 : 668593688408267098221717108121340671687275657516825733892567808 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  255 : 668593688408267098221717108121340671687275657516825733892567821 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  256 : 85579992116258188572379789839531605975971284162153693938248681088 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  263 : 85579992116258188572379789839531605975971284162153693938248681109 */ \
        for (int nops = 0; nops < 9; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  264 : 43816955963524192549058452397840182259697297491022691296383324727808 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  273 : 43816955963524192549058452397840182259697297491022691296383324727823 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  274 : 1402142590832774161569870476730885832310313519712726121484266391290336 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  279 : 1402142590832774161569870476730885832310313519712726121484266391290349 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  280 : 11217140726662193292558963813847086658482508157701808971874131130322792 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*  283 : 11217140726662193292558963813847086658482508157701808971874131130322795 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  284 : 2871588026025521482895094736344854184571522088371663096799777569362635520 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  292 : 2871588026025521482895094736344854184571522088371663096799777569362635535 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  293 : 22972704208204171863160757890758833476572176706973304774398220554901084280 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*  296 : 22972704208204171863160757890758833476572176706973304774398220554901084283 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  297 : 2940506138650133998484577010017130685001238618492583011122972231027338788224 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR10);                                    /*  304 : 2940506138650133998484577010017130685001238618492583011122972231027338788233 */ \
        for (int nops = 0; nops < 9; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  305 : 1505539142988868607224103429128770910720634172668202501694961782285997459575296 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  314 : 1505539142988868607224103429128770910720634172668202501694961782285997459575311 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  315 : 96354505151287590862342619464241338286120587050764960108477554066303837412819904 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  321 : 96354505151287590862342619464241338286120587050764960108477554066303837412819925 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  322 : 6166688329682405815189927645711445650311717571248957446942563460243445594420475200 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  328 : 6166688329682405815189927645711445650311717571248957446942563460243445594420475231 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  329 : 197334026549836986086077684662766260809974962279966638302162030727790259021455207392 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  334 : 197334026549836986086077684662766260809974962279966638302162030727790259021455207423 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  335 : 6314688849594783554754485909208520345919198792958932425669184983289288288686566637536 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  340 : 6314688849594783554754485909208520345919198792958932425669184983289288288686566637567 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  341 : 101035021593516536876071774547336325534707180687342918810706959732628612618985066201072 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  345 : 101035021593516536876071774547336325534707180687342918810706959732628612618985066201085 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  346 : 808280172748132295008574196378690604277657445498743350485655677861028900951880529608680 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*  349 : 808280172748132295008574196378690604277657445498743350485655677861028900951880529608683 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  350 : 206919724223521867522194994272944794695080306047678297724327853532423398643681415579822848 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  358 : 206919724223521867522194994272944794695080306047678297724327853532423398643681415579822869 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  359 : 26485724700610799042840959266936933720970279174102822108713965252150195026391221194217327232 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  366 : 26485724700610799042840959266936933720970279174102822108713965252150195026391221194217327263 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  367 : 847543190419545569370910696541981879071048933571290307478846888068806240844519078214954472416 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  372 : 847543190419545569370910696541981879071048933571290307478846888068806240844519078214954472447 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  373 : 27121382093425458219869142289343420130273565874281289839323100418201799707024610502878543118304 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  378 : 27121382093425458219869142289343420130273565874281289839323100418201799707024610502878543118335 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  379 : 433942113494807331517906276629494722084377053988500637429169606691228795312393768046056689893360 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  383 : 433942113494807331517906276629494722084377053988500637429169606691228795312393768046056689893375 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  384 : 6943073815916917304286500426071915553350032863816010198866713707059660724998300288736907038294000 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR7);                                     /*  388 : 6943073815916917304286500426071915553350032863816010198866713707059660724998300288736907038294007 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  389 : 888713448437365414948672054537205190828804206568449305454939354503636572799782436958324100901632896 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  396 : 888713448437365414948672054537205190828804206568449305454939354503636572799782436958324100901632927 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  397 : 28438830349995693278357505745190566106521734610190377774558059344116370329593037982666371228852253664 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR6);                                     /*  402 : 28438830349995693278357505745190566106521734610190377774558059344116370329593037982666371228852253693 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  403 : 910042571199862184907440183846098115408695507526092088785857899011723850546977215445323879323272118176 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  408 : 910042571199862184907440183846098115408695507526092088785857899011723850546977215445323879323272118207 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  409 : 29121362278395589917038085883075139693078256240834946841147452768375163217503270894250364138344707782624 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  414 : 29121362278395589917038085883075139693078256240834946841147452768375163217503270894250364138344707782655 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  415 : 931883592908658877345218748258404470178504199706718298916718488588005222960104668616011652427030649044960 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  420 : 931883592908658877345218748258404470178504199706718298916718488588005222960104668616011652427030649044991 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  421 : 29820274973077084075046999944268943045712134390614985565334991634816167134723349395712372877664980769439712 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  426 : 29820274973077084075046999944268943045712134390614985565334991634816167134723349395712372877664980769439743 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  427 : 954248799138466690401503998216606177462788300499679538090719732314117348311147180662795932085279384622071776 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  432 : 954248799138466690401503998216606177462788300499679538090719732314117348311147180662795932085279384622071807 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  433 : 30535961572430934092848127942931397678809225615989745218903031434051755145956709781209469826728940307906297824 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  438 : 30535961572430934092848127942931397678809225615989745218903031434051755145956709781209469826728940307906297855 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  439 : 488575385158894945485570047086902362860947609855835923502448502944828082335307356499351517227663044926500765680 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  443 : 488575385158894945485570047086902362860947609855835923502448502944828082335307356499351517227663044926500765693 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  444 : 31268824650169276511076483013561751223100647030773499104156704188468997269459670815958497102570434875296049004352 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  450 : 31268824650169276511076483013561751223100647030773499104156704188468997269459670815958497102570434875296049004373 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  451 : 1000602388805416848354447456433976039139220704984751971333014534031007912622709466110671907282253916009473568139936 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*  456 : 1000602388805416848354447456433976039139220704984751971333014534031007912622709466110671907282253916009473568139947 */ \
    } while (0)

/* *** addchain for 4002409555221667393417789825735904156556882819939007885332058136124031650490837864442687629129015664037894272559785 *** */
/* Bos-Coster (win=4) : 461 links, 16 variables */
#define _bint_chain_inv(TMPVAR1, TMPVAR0) do { \
        uint64_t TMPVAR2[NWORDS]; \
        uint64_t TMPVAR3[NWORDS]; \
        uint64_t TMPVAR4[NWORDS]; \
        uint64_t TMPVAR5[NWORDS]; \
        uint64_t TMPVAR6[NWORDS]; \
        uint64_t TMPVAR7[NWORDS]; \
        uint64_t TMPVAR8[NWORDS]; \
        uint64_t TMPVAR9[NWORDS]; \
        uint64_t TMPVAR10[NWORDS]; \
        uint64_t TMPVAR11[NWORDS]; \
        uint64_t TMPVAR12[NWORDS]; \
        uint64_t TMPVAR13[NWORDS]; \
        uint64_t TMPVAR14[NWORDS]; \
        uint64_t TMPVAR15[NWORDS]; \
        bint_sqr(TMPVAR1, TMPVAR0);                                              /*    0 : 2 */ \
        bint_mul(TMPVAR9, TMPVAR1, TMPVAR0);                                     /*    1 : 3 */ \
        bint_sqr(TMPVAR5, TMPVAR1);                                              /*    2 : 4 */ \
        bint_mul(TMPVAR2, TMPVAR9, TMPVAR1);                                     /*    3 : 5 */ \
        bint_mul(TMPVAR7, TMPVAR5, TMPVAR9);                                     /*    4 : 7 */ \
        bint_mul(TMPVAR10, TMPVAR2, TMPVAR5);                                    /*    5 : 9 */ \
        bint_mul(TMPVAR13, TMPVAR7, TMPVAR5);                                    /*    6 : 11 */ \
        bint_mul(TMPVAR4, TMPVAR10, TMPVAR5);                                    /*    7 : 13 */ \
        bint_mul(TMPVAR8, TMPVAR13, TMPVAR5);                                    /*    8 : 15 */ \
        bint_mul(TMPVAR15, TMPVAR4, TMPVAR5);                                    /*    9 : 17 */ \
        bint_mul(TMPVAR11, TMPVAR8, TMPVAR5);                                    /*   10 : 19 */ \
        bint_mul(TMPVAR3, TMPVAR15, TMPVAR5);                                    /*   11 : 21 */ \
        bint_mul(TMPVAR12, TMPVAR11, TMPVAR5);                                   /*   12 : 23 */ \
        bint_sqr(TMPVAR1, TMPVAR4);                                              /*   13 : 26 */ \
        bint_mul(TMPVAR14, TMPVAR12, TMPVAR5);                                   /*   14 : 27 */ \
        bint_mul(TMPVAR6, TMPVAR1, TMPVAR9);                                     /*   15 : 29 */ \
        bint_mul(TMPVAR5, TMPVAR1, TMPVAR2);                                     /*   16 : 31 */ \
        for (int nops = 0; nops < 12; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }    /*   17 : 106496 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR15);                                    /*   29 : 106513 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   30 : 13633664 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*   37 : 13633679 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   38 : 218138864 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*   42 : 218138869 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   43 : 13960887616 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR7);                                     /*   49 : 13960887623 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   50 : 1786993615744 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR12);                                    /*   57 : 1786993615767 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   58 : 57183795704544 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*   63 : 57183795704575 */ \
        for (int nops = 0; nops < 2; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   64 : 228735182818300 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*   66 : 228735182818303 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   67 : 14639051700371392 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*   73 : 14639051700371405 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   74 : 936899308823769920 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*   80 : 936899308823769933 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   81 : 59961555764721275712 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR10);                                    /*   87 : 59961555764721275721 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   88 : 479692446117770205768 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*   91 : 479692446117770205771 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   92 : 61400633103074586338688 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*   99 : 61400633103074586338701 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  100 : 982410129649193381419216 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  104 : 982410129649193381419229 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  105 : 62874248297548376410830656 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  111 : 62874248297548376410830671 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  112 : 4023951891043096090293162944 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR14);                                    /*  118 : 4023951891043096090293162971 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  119 : 32191615128344768722345303768 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR0);                                     /*  122 : 32191615128344768722345303769 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  123 : 8241053472856260792920397764864 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  131 : 8241053472856260792920397764877 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  132 : 1054854844525601381493810913904256 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR12);                                    /*  139 : 1054854844525601381493810913904279 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  140 : 33755355024819244207801949244936928 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR13);                                    /*  145 : 33755355024819244207801949244936939 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  146 : 2160342721588431629299324751675964096 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  152 : 2160342721588431629299324751675964109 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  153 : 138261934181659624275156784107261702976 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR6);                                     /*  159 : 138261934181659624275156784107261703005 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  160 : 2212190946906553988402508545716187248080 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR10);                                    /*  164 : 2212190946906553988402508545716187248089 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  165 : 566320882408077821031042187703343935510784 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR6);                                     /*  173 : 566320882408077821031042187703343935510813 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  174 : 9061134118529245136496675003253502968173008 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  178 : 9061134118529245136496675003253502968173021 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  179 : 1159825167171743377471574400416448379926146688 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR12);                                    /*  186 : 1159825167171743377471574400416448379926146711 */ \
        for (int nops = 0; nops < 9; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  187 : 593830485591932609265446093013221570522187116032 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR11);                                    /*  196 : 593830485591932609265446093013221570522187116051 */ \
        for (int nops = 0; nops < 2; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  197 : 2375321942367730437061784372052886282088748464204 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*  199 : 2375321942367730437061784372052886282088748464207 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  200 : 76010302155767373985977099905692361026839950854624 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR7);                                     /*  205 : 76010302155767373985977099905692361026839950854631 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  206 : 9729318675938223870205068787928622211435513709392768 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*  213 : 9729318675938223870205068787928622211435513709392773 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  214 : 1245352790520092655386248804854863643063745754802274944 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR10);                                    /*  221 : 1245352790520092655386248804854863643063745754802274953 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  222 : 79702578593285929944719923510711273156079728307345596992 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR12);                                    /*  228 : 79702578593285929944719923510711273156079728307345597015 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  229 : 2550482514985149758231037552342760740994551305835059104480 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR6);                                     /*  234 : 2550482514985149758231037552342760740994551305835059104509 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  235 : 81615440479524792263393201674968343711825641786721891344288 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR11);                                    /*  240 : 81615440479524792263393201674968343711825641786721891344307 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  241 : 2611694095344793352428582453598986998778420537175100523017824 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR11);                                    /*  246 : 2611694095344793352428582453598986998778420537175100523017843 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  247 : 668593688408267098221717108121340671687275657516825733892567808 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  255 : 668593688408267098221717108121340671687275657516825733892567821 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  256 : 85579992116258188572379789839531605975971284162153693938248681088 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  263 : 85579992116258188572379789839531605975971284162153693938248681109 */ \
        for (int nops = 0; nops < 9; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  264 : 43816955963524192549058452397840182259697297491022691296383324727808 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  273 : 43816955963524192549058452397840182259697297491022691296383324727823 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  274 : 1402142590832774161569870476730885832310313519712726121484266391290336 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  279 : 1402142590832774161569870476730885832310313519712726121484266391290349 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  280 : 11217140726662193292558963813847086658482508157701808971874131130322792 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*  283 : 11217140726662193292558963813847086658482508157701808971874131130322795 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  284 : 2871588026025521482895094736344854184571522088371663096799777569362635520 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  292 : 2871588026025521482895094736344854184571522088371663096799777569362635535 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  293 : 22972704208204171863160757890758833476572176706973304774398220554901084280 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*  296 : 22972704208204171863160757890758833476572176706973304774398220554901084283 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  297 : 2940506138650133998484577010017130685001238618492583011122972231027338788224 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR10);                                    /*  304 : 2940506138650133998484577010017130685001238618492583011122972231027338788233 */ \
        for (int nops = 0; nops < 9; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  305 : 1505539142988868607224103429128770910720634172668202501694961782285997459575296 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  314 : 1505539142988868607224103429128770910720634172668202501694961782285997459575311 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  315 : 96354505151287590862342619464241338286120587050764960108477554066303837412819904 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  321 : 96354505151287590862342619464241338286120587050764960108477554066303837412819925 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  322 : 6166688329682405815189927645711445650311717571248957446942563460243445594420475200 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  328 : 6166688329682405815189927645711445650311717571248957446942563460243445594420475231 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  329 : 197334026549836986086077684662766260809974962279966638302162030727790259021455207392 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  334 : 197334026549836986086077684662766260809974962279966638302162030727790259021455207423 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  335 : 6314688849594783554754485909208520345919198792958932425669184983289288288686566637536 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  340 : 6314688849594783554754485909208520345919198792958932425669184983289288288686566637567 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  341 : 101035021593516536876071774547336325534707180687342918810706959732628612618985066201072 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  345 : 101035021593516536876071774547336325534707180687342918810706959732628612618985066201085 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  346 : 808280172748132295008574196378690604277657445498743350485655677861028900951880529608680 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*  349 : 808280172748132295008574196378690604277657445498743350485655677861028900951880529608683 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  350 : 206919724223521867522194994272944794695080306047678297724327853532423398643681415579822848 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  358 : 206919724223521867522194994272944794695080306047678297724327853532423398643681415579822869 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  359 : 26485724700610799042840959266936933720970279174102822108713965252150195026391221194217327232 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  366 : 26485724700610799042840959266936933720970279174102822108713965252150195026391221194217327263 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  367 : 847543190419545569370910696541981879071048933571290307478846888068806240844519078214954472416 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  372 : 847543190419545569370910696541981879071048933571290307478846888068806240844519078214954472447 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  373 : 27121382093425458219869142289343420130273565874281289839323100418201799707024610502878543118304 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  378 : 27121382093425458219869142289343420130273565874281289839323100418201799707024610502878543118335 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  379 : 433942113494807331517906276629494722084377053988500637429169606691228795312393768046056689893360 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  383 : 433942113494807331517906276629494722084377053988500637429169606691228795312393768046056689893375 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  384 : 6943073815916917304286500426071915553350032863816010198866713707059660724998300288736907038294000 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR7);                                     /*  388 : 6943073815916917304286500426071915553350032863816010198866713707059660724998300288736907038294007 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  389 : 888713448437365414948672054537205190828804206568449305454939354503636572799782436958324100901632896 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  396 : 888713448437365414948672054537205190828804206568449305454939354503636572799782436958324100901632927 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  397 : 28438830349995693278357505745190566106521734610190377774558059344116370329593037982666371228852253664 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR6);                                     /*  402 : 28438830349995693278357505745190566106521734610190377774558059344116370329593037982666371228852253693 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  403 : 910042571199862184907440183846098115408695507526092088785857899011723850546977215445323879323272118176 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  408 : 910042571199862184907440183846098115408695507526092088785857899011723850546977215445323879323272118207 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  409 : 29121362278395589917038085883075139693078256240834946841147452768375163217503270894250364138344707782624 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  414 : 29121362278395589917038085883075139693078256240834946841147452768375163217503270894250364138344707782655 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  415 : 931883592908658877345218748258404470178504199706718298916718488588005222960104668616011652427030649044960 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  420 : 931883592908658877345218748258404470178504199706718298916718488588005222960104668616011652427030649044991 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  421 : 29820274973077084075046999944268943045712134390614985565334991634816167134723349395712372877664980769439712 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  426 : 29820274973077084075046999944268943045712134390614985565334991634816167134723349395712372877664980769439743 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  427 : 954248799138466690401503998216606177462788300499679538090719732314117348311147180662795932085279384622071776 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  432 : 954248799138466690401503998216606177462788300499679538090719732314117348311147180662795932085279384622071807 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  433 : 30535961572430934092848127942931397678809225615989745218903031434051755145956709781209469826728940307906297824 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  438 : 30535961572430934092848127942931397678809225615989745218903031434051755145956709781209469826728940307906297855 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  439 : 488575385158894945485570047086902362860947609855835923502448502944828082335307356499351517227663044926500765680 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  443 : 488575385158894945485570047086902362860947609855835923502448502944828082335307356499351517227663044926500765693 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  444 : 31268824650169276511076483013561751223100647030773499104156704188468997269459670815958497102570434875296049004352 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  450 : 31268824650169276511076483013561751223100647030773499104156704188468997269459670815958497102570434875296049004373 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  451 : 500301194402708424177223728216988019569610352492375985666507267015503956311354733055335953641126958004736784069968 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*  455 : 500301194402708424177223728216988019569610352492375985666507267015503956311354733055335953641126958004736784069973 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  456 : 4002409555221667393417789825735904156556882819939007885332058136124031650490837864442687629129015664037894272559784 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR0);                                     /*  459 : 4002409555221667393417789825735904156556882819939007885332058136124031650490837864442687629129015664037894272559785 */ \
    } while (0)

/* *** addchain for 3001807166416250545063342369301928117417662114954255913999043602093023737868128398332015721846761748028420704419839 *** */
/* Bos-Coster (win=5) : 457 links, 25 variables */
#define _bint_chain_invsqrt(TMPVAR1, TMPVAR0) do { \
        uint64_t TMPVAR2[NWORDS]; \
        uint64_t TMPVAR3[NWORDS]; \
        uint64_t TMPVAR4[NWORDS]; \
        uint64_t TMPVAR5[NWORDS]; \
        uint64_t TMPVAR6[NWORDS]; \
        uint64_t TMPVAR7[NWORDS]; \
        uint64_t TMPVAR8[NWORDS]; \
        uint64_t TMPVAR9[NWORDS]; \
        uint64_t TMPVAR10[NWORDS]; \
        uint64_t TMPVAR11[NWORDS]; \
        uint64_t TMPVAR12[NWORDS]; \
        uint64_t TMPVAR13[NWORDS]; \
        uint64_t TMPVAR14[NWORDS]; \
        uint64_t TMPVAR15[NWORDS]; \
        uint64_t TMPVAR16[NWORDS]; \
        uint64_t TMPVAR17[NWORDS]; \
        uint64_t TMPVAR18[NWORDS]; \
        uint64_t TMPVAR19[NWORDS]; \
        uint64_t TMPVAR20[NWORDS]; \
        uint64_t TMPVAR21[NWORDS]; \
        uint64_t TMPVAR22[NWORDS]; \
        uint64_t TMPVAR23[NWORDS]; \
        uint64_t TMPVAR24[NWORDS]; \
        bint_sqr(TMPVAR2, TMPVAR0);                                              /*    0 : 2 */ \
        bint_sqr(TMPVAR1, TMPVAR2);                                              /*    1 : 4 */ \
        bint_mul(TMPVAR18, TMPVAR1, TMPVAR0);                                    /*    2 : 5 */ \
        bint_mul(TMPVAR6, TMPVAR1, TMPVAR2);                                     /*    3 : 6 */ \
        bint_mul(TMPVAR14, TMPVAR18, TMPVAR2);                                   /*    4 : 7 */ \
        bint_sqr(TMPVAR4, TMPVAR1);                                              /*    5 : 8 */ \
        bint_mul(TMPVAR19, TMPVAR18, TMPVAR1);                                   /*    6 : 9 */ \
        bint_mul(TMPVAR21, TMPVAR6, TMPVAR18);                                   /*    7 : 11 */ \
        bint_mul(TMPVAR13, TMPVAR14, TMPVAR6);                                   /*    8 : 13 */ \
        bint_mul(TMPVAR15, TMPVAR4, TMPVAR14);                                   /*    9 : 15 */ \
        bint_mul(TMPVAR8, TMPVAR15, TMPVAR4);                                    /*   10 : 23 */ \
        bint_mul(TMPVAR12, TMPVAR8, TMPVAR2);                                    /*   11 : 25 */ \
        bint_mul(TMPVAR9, TMPVAR8, TMPVAR1);                                     /*   12 : 27 */ \
        bint_mul(TMPVAR2, TMPVAR8, TMPVAR4);                                     /*   13 : 31 */ \
        bint_mul(TMPVAR23, TMPVAR12, TMPVAR4);                                   /*   14 : 33 */ \
        bint_mul(TMPVAR20, TMPVAR9, TMPVAR4);                                    /*   15 : 35 */ \
        bint_mul(TMPVAR11, TMPVAR2, TMPVAR6);                                    /*   16 : 37 */ \
        bint_mul(TMPVAR7, TMPVAR2, TMPVAR4);                                     /*   17 : 39 */ \
        bint_mul(TMPVAR22, TMPVAR23, TMPVAR4);                                   /*   18 : 41 */ \
        bint_mul(TMPVAR5, TMPVAR11, TMPVAR4);                                    /*   19 : 45 */ \
        bint_mul(TMPVAR3, TMPVAR7, TMPVAR4);                                     /*   20 : 47 */ \
        bint_mul(TMPVAR10, TMPVAR22, TMPVAR4);                                   /*   21 : 49 */ \
        bint_mul(TMPVAR24, TMPVAR5, TMPVAR6);                                    /*   22 : 51 */ \
        bint_mul(TMPVAR16, TMPVAR5, TMPVAR4);                                    /*   23 : 53 */ \
        bint_mul(TMPVAR17, TMPVAR24, TMPVAR4);                                   /*   24 : 59 */ \
        bint_mul(TMPVAR6, TMPVAR16, TMPVAR4);                                    /*   25 : 61 */ \
        bint_mul(TMPVAR4, TMPVAR17, TMPVAR1);                                    /*   26 : 63 */ \
        bint_sqr(TMPVAR1, TMPVAR7);                                              /*   27 : 78 */ \
        for (int nops = 0; nops < 12; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }    /*   28 : 319488 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR24);                                    /*   40 : 319539 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   41 : 40900992 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*   48 : 40901037 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   49 : 2617666368 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR6);                                     /*   55 : 2617666429 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   56 : 83765325728 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR21);                                    /*   61 : 83765325739 */ \
        for (int nops = 0; nops < 9; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   62 : 42887846778368 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*   71 : 42887846778431 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   72 : 1372411096909792 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*   77 : 1372411096909819 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   78 : 87834310202228416 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR15);                                    /*   84 : 87834310202228431 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   85 : 22485583411770478336 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR17);                                    /*   93 : 22485583411770478395 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   94 : 1439077338353310617280 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR23);                                    /*  100 : 1439077338353310617313 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  101 : 184201899309223759016064 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR22);                                    /*  108 : 184201899309223759016105 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  109 : 23577843111580641154061440 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR6);                                     /*  116 : 23577843111580641154061501 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  117 : 754490979570580516929968032 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR12);                                    /*  122 : 754490979570580516929968057 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  123 : 96574845385034306167035911296 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR21);                                    /*  130 : 96574845385034306167035911307 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  131 : 24723160418568782378761193294592 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR7);                                     /*  139 : 24723160418568782378761193294631 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  140 : 1582282266788402072240716370856384 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR20);                                    /*  146 : 1582282266788402072240716370856419 */ \
        for (int nops = 0; nops < 9; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  147 : 810128520595661860987246781878486528 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR13);                                    /*  156 : 810128520595661860987246781878486541 */ \
        for (int nops = 0; nops < 10; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }    /*  157 : 829571605089957745650940704643570217984 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR10);                                    /*  167 : 829571605089957745650940704643570218033 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  168 : 106185165451514591443320410194376987908224 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR16);                                    /*  175 : 106185165451514591443320410194376987908277 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  176 : 1698962647224233463093126563110031806532432 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR19);                                    /*  180 : 1698962647224233463093126563110031806532441 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  181 : 54366804711175470818980050019521017809038112 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR15);                                    /*  186 : 54366804711175470818980050019521017809038127 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  187 : 3479475501515230132414723201249345139778440128 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR18);                                    /*  193 : 3479475501515230132414723201249345139778440133 */ \
        for (int nops = 0; nops < 9; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  194 : 1781491456775797827796338279039664711566561348096 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR17);                                    /*  203 : 1781491456775797827796338279039664711566561348155 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  204 : 228030906467302121957931299717077083080519852563840 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR16);                                    /*  211 : 228030906467302121957931299717077083080519852563893 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  212 : 29187956027814671610615206363785866634306541128178304 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR15);                                    /*  219 : 29187956027814671610615206363785866634306541128178319 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  220 : 934014592890069491539686603641147732297809316101706208 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR14);                                    /*  225 : 934014592890069491539686603641147732297809316101706215 */ \
        for (int nops = 0; nops < 10; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }    /*  226 : 956430943119431159336639082128535277872956739688147164160 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*  236 : 956430943119431159336639082128535277872956739688147164191 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  237 : 122423160719287188395089802512452515567738462680082837016448 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR13);                                    /*  244 : 122423160719287188395089802512452515567738462680082837016461 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  245 : 7835082286034380057285747360796960996335261611525301569053504 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR12);                                    /*  251 : 7835082286034380057285747360796960996335261611525301569053529 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  252 : 2005781065224801294665151324364022015061826972550477201677703424 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR7);                                     /*  260 : 2005781065224801294665151324364022015061826972550477201677703463 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  261 : 256739976348774565717139369518594817927913852486461081814746043264 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  268 : 256739976348774565717139369518594817927913852486461081814746043327 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  269 : 65725433945286288823587678596760273389545946236534036944574987091712 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  277 : 65725433945286288823587678596760273389545946236534036944574987091735 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  278 : 525803471562290310588701428774082187116367569892272295556599896733880 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR0);                                     /*  281 : 525803471562290310588701428774082187116367569892272295556599896733881 */ \
        for (int nops = 0; nops < 11; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }    /*  282 : 1076845509759570556085660526129320319214320783139373661299916588510988288 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR11);                                    /*  293 : 1076845509759570556085660526129320319214320783139373661299916588510988325 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  294 : 68918112624612515589482273672276500429716530120919914323194661664703252800 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR10);                                    /*  300 : 68918112624612515589482273672276500429716530120919914323194661664703252849 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  301 : 8821518415950401995453731030051392055003715855477749033368916693082016364672 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*  308 : 8821518415950401995453731030051392055003715855477749033368916693082016364699 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  309 : 2258308714483302910836155143693156366080951259002303752542442673428996189362944 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  317 : 2258308714483302910836155143693156366080951259002303752542442673428996189362967 */ \
        for (int nops = 0; nops < 14; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }    /*  318 : 37000129978094434891139565874268673901870305427493744681655380761460673566522851328 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  332 : 37000129978094434891139565874268673901870305427493744681655380761460673566522851391 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  333 : 1184004159299021916516466107976597564859849773679799829812972184366741554128731244512 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*  338 : 1184004159299021916516466107976597564859849773679799829812972184366741554128731244543 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  339 : 37888133097568701328526915455251122075515192757753594554015109899735729732119399825376 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*  344 : 37888133097568701328526915455251122075515192757753594554015109899735729732119399825407 */ \
        for (int nops = 0; nops < 11; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }    /*  345 : 77594896583820700320823122852354298010655114767879361646622945074658774491380530842433536 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR7);                                     /*  356 : 77594896583820700320823122852354298010655114767879361646622945074658774491380530842433575 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  357 : 4966073381364524820532679862550675072681927345144279145383868484778161567448353973915748800 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR6);                                     /*  363 : 4966073381364524820532679862550675072681927345144279145383868484778161567448353973915748861 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  364 : 317828696407329588514091511203243204651643350089233865304567583025802340316694654330607927104 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  370 : 317828696407329588514091511203243204651643350089233865304567583025802340316694654330607927167 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  371 : 20341036570069093664901856717007565097705174405710967379492325313651349780268457877158907338688 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  377 : 20341036570069093664901856717007565097705174405710967379492325313651349780268457877158907338751 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  378 : 650913170242210997276859414944242083126565580982750956143754410036843192968590652069085034840032 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*  383 : 650913170242210997276859414944242083126565580982750956143754410036843192968590652069085034840063 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  384 : 166633771582006015302876010225725973280400788731584244772801128969431857399959206929685768919056128 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  392 : 166633771582006015302876010225725973280400788731584244772801128969431857399959206929685768919056173 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  393 : 10664561381248384979384064654446462289945650478821391665459272254043638873597389243499889210819595072 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  399 : 10664561381248384979384064654446462289945650478821391665459272254043638873597389243499889210819595135 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  400 : 2730127713599586554722320551538294346226086522578276266357573697035171551640931646335971637969816354560 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  408 : 2730127713599586554722320551538294346226086522578276266357573697035171551640931646335971637969816354623 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  409 : 174728173670373539502228515298450838158469537445009681046884716610250979305019625365502184830068246695872 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  415 : 174728173670373539502228515298450838158469537445009681046884716610250979305019625365502184830068246695935 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  416 : 11182603114903906528142624979100853642142050396480619587000621863056062675521256023392139829124367788539840 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  422 : 11182603114903906528142624979100853642142050396480619587000621863056062675521256023392139829124367788539903 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  423 : 715686599353850017801127998662454633097091225374759653568039799235588011233360385497096949063959538466553792 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  429 : 715686599353850017801127998662454633097091225374759653568039799235588011233360385497096949063959538466553855 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  430 : 45803942358646401139272191914397096518213838423984617828354547151077632718935064671814204740093410461859446720 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  436 : 45803942358646401139272191914397096518213838423984617828354547151077632718935064671814204740093410461859446783 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  437 : 2931452310953369672913420282521414177165685659135015541014691017668968494011844138996109103365978269559004594112 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  443 : 2931452310953369672913420282521414177165685659135015541014691017668968494011844138996109103365978269559004594159 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  444 : 93806473950507829533229449040685253669301941092320497312470112565406991808379012447875491307711304625888147013088 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*  449 : 93806473950507829533229449040685253669301941092320497312470112565406991808379012447875491307711304625888147013119 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  450 : 3001807166416250545063342369301928117417662114954255913999043602093023737868128398332015721846761748028420704419808 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*  455 : 3001807166416250545063342369301928117417662114954255913999043602093023737868128398332015721846761748028420704419839 */ \
    } while (0)

/* *** addchain for 2001204777610833696708894912867952078278441409969503942666029068062015825245418932221343814564507832018947136279893 *** */
/* Bos-Coster (win=4) : 459 links, 16 variables */
#define _bint_chain_jacobi(TMPVAR1, TMPVAR0) do { \
        uint64_t TMPVAR2[NWORDS]; \
        uint64_t TMPVAR3[NWORDS]; \
        uint64_t TMPVAR4[NWORDS]; \
        uint64_t TMPVAR5[NWORDS]; \
        uint64_t TMPVAR6[NWORDS]; \
        uint64_t TMPVAR7[NWORDS]; \
        uint64_t TMPVAR8[NWORDS]; \
        uint64_t TMPVAR9[NWORDS]; \
        uint64_t TMPVAR10[NWORDS]; \
        uint64_t TMPVAR11[NWORDS]; \
        uint64_t TMPVAR12[NWORDS]; \
        uint64_t TMPVAR13[NWORDS]; \
        uint64_t TMPVAR14[NWORDS]; \
        uint64_t TMPVAR15[NWORDS]; \
        bint_sqr(TMPVAR1, TMPVAR0);                                              /*    0 : 2 */ \
        bint_mul(TMPVAR8, TMPVAR1, TMPVAR0);                                     /*    1 : 3 */ \
        bint_sqr(TMPVAR4, TMPVAR1);                                              /*    2 : 4 */ \
        bint_mul(TMPVAR12, TMPVAR8, TMPVAR1);                                    /*    3 : 5 */ \
        bint_mul(TMPVAR6, TMPVAR4, TMPVAR8);                                     /*    4 : 7 */ \
        bint_mul(TMPVAR9, TMPVAR12, TMPVAR4);                                    /*    5 : 9 */ \
        bint_mul(TMPVAR13, TMPVAR6, TMPVAR4);                                    /*    6 : 11 */ \
        bint_mul(TMPVAR3, TMPVAR9, TMPVAR4);                                     /*    7 : 13 */ \
        bint_mul(TMPVAR7, TMPVAR13, TMPVAR4);                                    /*    8 : 15 */ \
        bint_mul(TMPVAR15, TMPVAR3, TMPVAR4);                                    /*    9 : 17 */ \
        bint_mul(TMPVAR10, TMPVAR7, TMPVAR4);                                    /*   10 : 19 */ \
        bint_mul(TMPVAR2, TMPVAR15, TMPVAR4);                                    /*   11 : 21 */ \
        bint_mul(TMPVAR11, TMPVAR10, TMPVAR4);                                   /*   12 : 23 */ \
        bint_sqr(TMPVAR1, TMPVAR3);                                              /*   13 : 26 */ \
        bint_mul(TMPVAR14, TMPVAR11, TMPVAR4);                                   /*   14 : 27 */ \
        bint_mul(TMPVAR5, TMPVAR1, TMPVAR8);                                     /*   15 : 29 */ \
        bint_mul(TMPVAR4, TMPVAR1, TMPVAR12);                                    /*   16 : 31 */ \
        for (int nops = 0; nops < 12; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }    /*   17 : 106496 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR15);                                    /*   29 : 106513 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   30 : 13633664 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR7);                                     /*   37 : 13633679 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   38 : 218138864 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR12);                                    /*   42 : 218138869 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   43 : 13960887616 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR6);                                     /*   49 : 13960887623 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   50 : 1786993615744 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR11);                                    /*   57 : 1786993615767 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   58 : 57183795704544 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*   63 : 57183795704575 */ \
        for (int nops = 0; nops < 2; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   64 : 228735182818300 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*   66 : 228735182818303 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   67 : 14639051700371392 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*   73 : 14639051700371405 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   74 : 936899308823769920 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*   80 : 936899308823769933 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   81 : 59961555764721275712 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*   87 : 59961555764721275721 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   88 : 479692446117770205768 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*   91 : 479692446117770205771 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*   92 : 61400633103074586338688 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*   99 : 61400633103074586338701 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  100 : 982410129649193381419216 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  104 : 982410129649193381419229 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  105 : 62874248297548376410830656 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR7);                                     /*  111 : 62874248297548376410830671 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  112 : 4023951891043096090293162944 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR14);                                    /*  118 : 4023951891043096090293162971 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  119 : 32191615128344768722345303768 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR0);                                     /*  122 : 32191615128344768722345303769 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  123 : 8241053472856260792920397764864 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  131 : 8241053472856260792920397764877 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  132 : 1054854844525601381493810913904256 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR11);                                    /*  139 : 1054854844525601381493810913904279 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  140 : 33755355024819244207801949244936928 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR13);                                    /*  145 : 33755355024819244207801949244936939 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  146 : 2160342721588431629299324751675964096 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  152 : 2160342721588431629299324751675964109 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  153 : 138261934181659624275156784107261702976 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  159 : 138261934181659624275156784107261703005 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  160 : 2212190946906553988402508545716187248080 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*  164 : 2212190946906553988402508545716187248089 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  165 : 566320882408077821031042187703343935510784 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  173 : 566320882408077821031042187703343935510813 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  174 : 9061134118529245136496675003253502968173008 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  178 : 9061134118529245136496675003253502968173021 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  179 : 1159825167171743377471574400416448379926146688 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR11);                                    /*  186 : 1159825167171743377471574400416448379926146711 */ \
        for (int nops = 0; nops < 9; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  187 : 593830485591932609265446093013221570522187116032 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR10);                                    /*  196 : 593830485591932609265446093013221570522187116051 */ \
        for (int nops = 0; nops < 2; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  197 : 2375321942367730437061784372052886282088748464204 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  199 : 2375321942367730437061784372052886282088748464207 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  200 : 76010302155767373985977099905692361026839950854624 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR6);                                     /*  205 : 76010302155767373985977099905692361026839950854631 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  206 : 9729318675938223870205068787928622211435513709392768 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR12);                                    /*  213 : 9729318675938223870205068787928622211435513709392773 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  214 : 1245352790520092655386248804854863643063745754802274944 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*  221 : 1245352790520092655386248804854863643063745754802274953 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  222 : 79702578593285929944719923510711273156079728307345596992 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR11);                                    /*  228 : 79702578593285929944719923510711273156079728307345597015 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  229 : 2550482514985149758231037552342760740994551305835059104480 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  234 : 2550482514985149758231037552342760740994551305835059104509 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  235 : 81615440479524792263393201674968343711825641786721891344288 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR10);                                    /*  240 : 81615440479524792263393201674968343711825641786721891344307 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  241 : 2611694095344793352428582453598986998778420537175100523017824 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR10);                                    /*  246 : 2611694095344793352428582453598986998778420537175100523017843 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  247 : 668593688408267098221717108121340671687275657516825733892567808 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  255 : 668593688408267098221717108121340671687275657516825733892567821 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  256 : 85579992116258188572379789839531605975971284162153693938248681088 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*  263 : 85579992116258188572379789839531605975971284162153693938248681109 */ \
        for (int nops = 0; nops < 9; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  264 : 43816955963524192549058452397840182259697297491022691296383324727808 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR7);                                     /*  273 : 43816955963524192549058452397840182259697297491022691296383324727823 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  274 : 1402142590832774161569870476730885832310313519712726121484266391290336 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  279 : 1402142590832774161569870476730885832310313519712726121484266391290349 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  280 : 11217140726662193292558963813847086658482508157701808971874131130322792 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  283 : 11217140726662193292558963813847086658482508157701808971874131130322795 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  284 : 2871588026025521482895094736344854184571522088371663096799777569362635520 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR7);                                     /*  292 : 2871588026025521482895094736344854184571522088371663096799777569362635535 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  293 : 22972704208204171863160757890758833476572176706973304774398220554901084280 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  296 : 22972704208204171863160757890758833476572176706973304774398220554901084283 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  297 : 2940506138650133998484577010017130685001238618492583011122972231027338788224 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR9);                                     /*  304 : 2940506138650133998484577010017130685001238618492583011122972231027338788233 */ \
        for (int nops = 0; nops < 9; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  305 : 1505539142988868607224103429128770910720634172668202501694961782285997459575296 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR7);                                     /*  314 : 1505539142988868607224103429128770910720634172668202501694961782285997459575311 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  315 : 96354505151287590862342619464241338286120587050764960108477554066303837412819904 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*  321 : 96354505151287590862342619464241338286120587050764960108477554066303837412819925 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  322 : 6166688329682405815189927645711445650311717571248957446942563460243445594420475200 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  328 : 6166688329682405815189927645711445650311717571248957446942563460243445594420475231 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  329 : 197334026549836986086077684662766260809974962279966638302162030727790259021455207392 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  334 : 197334026549836986086077684662766260809974962279966638302162030727790259021455207423 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  335 : 6314688849594783554754485909208520345919198792958932425669184983289288288686566637536 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  340 : 6314688849594783554754485909208520345919198792958932425669184983289288288686566637567 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  341 : 101035021593516536876071774547336325534707180687342918810706959732628612618985066201072 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  345 : 101035021593516536876071774547336325534707180687342918810706959732628612618985066201085 */ \
        for (int nops = 0; nops < 3; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  346 : 808280172748132295008574196378690604277657445498743350485655677861028900951880529608680 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR8);                                     /*  349 : 808280172748132295008574196378690604277657445498743350485655677861028900951880529608683 */ \
        for (int nops = 0; nops < 8; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  350 : 206919724223521867522194994272944794695080306047678297724327853532423398643681415579822848 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*  358 : 206919724223521867522194994272944794695080306047678297724327853532423398643681415579822869 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  359 : 26485724700610799042840959266936933720970279174102822108713965252150195026391221194217327232 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  366 : 26485724700610799042840959266936933720970279174102822108713965252150195026391221194217327263 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  367 : 847543190419545569370910696541981879071048933571290307478846888068806240844519078214954472416 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  372 : 847543190419545569370910696541981879071048933571290307478846888068806240844519078214954472447 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  373 : 27121382093425458219869142289343420130273565874281289839323100418201799707024610502878543118304 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  378 : 27121382093425458219869142289343420130273565874281289839323100418201799707024610502878543118335 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  379 : 433942113494807331517906276629494722084377053988500637429169606691228795312393768046056689893360 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR7);                                     /*  383 : 433942113494807331517906276629494722084377053988500637429169606691228795312393768046056689893375 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  384 : 6943073815916917304286500426071915553350032863816010198866713707059660724998300288736907038294000 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR6);                                     /*  388 : 6943073815916917304286500426071915553350032863816010198866713707059660724998300288736907038294007 */ \
        for (int nops = 0; nops < 7; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  389 : 888713448437365414948672054537205190828804206568449305454939354503636572799782436958324100901632896 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  396 : 888713448437365414948672054537205190828804206568449305454939354503636572799782436958324100901632927 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  397 : 28438830349995693278357505745190566106521734610190377774558059344116370329593037982666371228852253664 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR5);                                     /*  402 : 28438830349995693278357505745190566106521734610190377774558059344116370329593037982666371228852253693 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  403 : 910042571199862184907440183846098115408695507526092088785857899011723850546977215445323879323272118176 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  408 : 910042571199862184907440183846098115408695507526092088785857899011723850546977215445323879323272118207 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  409 : 29121362278395589917038085883075139693078256240834946841147452768375163217503270894250364138344707782624 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  414 : 29121362278395589917038085883075139693078256240834946841147452768375163217503270894250364138344707782655 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  415 : 931883592908658877345218748258404470178504199706718298916718488588005222960104668616011652427030649044960 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  420 : 931883592908658877345218748258404470178504199706718298916718488588005222960104668616011652427030649044991 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  421 : 29820274973077084075046999944268943045712134390614985565334991634816167134723349395712372877664980769439712 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  426 : 29820274973077084075046999944268943045712134390614985565334991634816167134723349395712372877664980769439743 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  427 : 954248799138466690401503998216606177462788300499679538090719732314117348311147180662795932085279384622071776 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  432 : 954248799138466690401503998216606177462788300499679538090719732314117348311147180662795932085279384622071807 */ \
        for (int nops = 0; nops < 5; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  433 : 30535961572430934092848127942931397678809225615989745218903031434051755145956709781209469826728940307906297824 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR4);                                     /*  438 : 30535961572430934092848127942931397678809225615989745218903031434051755145956709781209469826728940307906297855 */ \
        for (int nops = 0; nops < 4; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  439 : 488575385158894945485570047086902362860947609855835923502448502944828082335307356499351517227663044926500765680 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR3);                                     /*  443 : 488575385158894945485570047086902362860947609855835923502448502944828082335307356499351517227663044926500765693 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  444 : 31268824650169276511076483013561751223100647030773499104156704188468997269459670815958497102570434875296049004352 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*  450 : 31268824650169276511076483013561751223100647030773499104156704188468997269459670815958497102570434875296049004373 */ \
        for (int nops = 0; nops < 6; nops++) { bint_sqr(TMPVAR1, TMPVAR1); }     /*  451 : 2001204777610833696708894912867952078278441409969503942666029068062015825245418932221343814564507832018947136279872 */ \
        bint_mul(TMPVAR1, TMPVAR1, TMPVAR2);                                     /*  457 : 2001204777610833696708894912867952078278441409969503942666029068062015825245418932221343814564507832018947136279893 */ \
    } while (0)

#define _HAVE_BINT_CHAINS_H
#endif // _HAVE_BINT_CHAINS_H
