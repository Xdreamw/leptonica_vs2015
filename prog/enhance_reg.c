/*====================================================================*
 -  Copyright (C) 2001 Leptonica.  All rights reserved.
 -  This software is distributed in the hope that it will be
 -  useful, but with NO WARRANTY OF ANY KIND.
 -  No author or distributor accepts responsibility to anyone for the
 -  consequences of using this software, or for whether it serves any
 -  particular purpose or works at all, unless he or she says so in
 -  writing.  Everyone is granted permission to copy, modify and
 -  redistribute this source code, for commercial or non-commercial
 -  purposes, with the following restrictions: (1) the origin of this
 -  source code must not be misrepresented; (2) modified versions must
 -  be plainly marked as such; and (3) this notice may not be removed
 -  or altered from any source or modified source distribution.
 *====================================================================*/

/*
 * enhance_reg.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "allheaders.h"

static const char *filein = "test24.jpg";
static const l_int32 WIDTH = 150;

main(int    argc,
     char **argv)
{
l_int32      w, h, d, i, same;
l_float32    scalefact, sat;
L_KERNEL    *kel;
NUMA        *na;
PIX         *pix, *pixs, *pixs1, *pixs2, *pixd;
PIX         *pixt0, *pixt1, *pixt2, *pixt3, *pixt4;
PIXA        *pixa, *pixaf;
static char  mainName[] = "enhance_reg";


    pix = pixRead(filein);
    pixGetDimensions(pix, &w, &h, &d);
    if (d != 32)
        return ERROR_INT("file not 32 bpp", mainName, 1);
    scalefact = (l_float32)WIDTH / (l_float32)w;
    pixs = pixScale(pix, scalefact, scalefact);
    w = pixGetWidth(pixs);
    pixaf = pixaCreate(5);

        /* TRC: vary gamma */
    pixa = pixaCreate(20);
    for (i = 0; i < 20; i++) {
        pixt0 = pixGammaTRC(NULL, pixs, 0.3 + 0.15 * i, 0, 255);
        pixaAddPix(pixa, pixt0, L_INSERT);
    }
    pixt1 = pixaDisplayTiledAndScaled(pixa, 32, w, 5, 0, 10, 2);
    pixSaveTiled(pixt1, pixaf, 1, 1, 20, 32);
    pixWrite("/tmp/junktrcgam.png", pixt1, IFF_PNG);
    pixDisplayWithTitle(pixt1, 0, 100, "TRC Gamma", 1);
    pixDestroy(&pixt1);
    pixaDestroy(&pixa);

        /* TRC: vary black point */
    pixa = pixaCreate(20);
    for (i = 0; i < 20; i++) {
        pixt0 = pixGammaTRC(NULL, pixs, 1.0, 5 * i, 255);
        pixaAddPix(pixa, pixt0, L_INSERT);
    }
    pixt1 = pixaDisplayTiledAndScaled(pixa, 32, w, 5, 0, 10, 2);
    pixSaveTiled(pixt1, pixaf, 1, 1, 20, 0);
    pixWrite("/tmp/junktrc.png", pixt1, IFF_PNG);
    pixDisplayWithTitle(pixt1, 300, 100, "TRC", 1);
    pixDestroy(&pixt1);
    pixaDestroy(&pixa);

        /* Vary hue */
    pixa = pixaCreate(20);
    for (i = 0; i < 20; i++) {
        pixt0 = pixModifyHue(NULL, pixs, 0.01 + 0.05 * i);
        pixaAddPix(pixa, pixt0, L_INSERT);
    }
    pixt1 = pixaDisplayTiledAndScaled(pixa, 32, w, 5, 0, 10, 2);
    pixSaveTiled(pixt1, pixaf, 1, 1, 20, 0);
    pixWrite("/tmp/junkhue.png", pixt1, IFF_PNG);
    pixDisplayWithTitle(pixt1, 600, 100, "Hue", 1);
    pixDestroy(&pixt1);
    pixaDestroy(&pixa);

        /* Vary saturation */
    pixa = pixaCreate(20);
    na = numaCreate(20);
    for (i = 0; i < 20; i++) {
        pixt0 = pixModifySaturation(NULL, pixs, -0.9 + 0.1 * i);
        pixMeasureSaturation(pixt0, 1, &sat);
        pixaAddPix(pixa, pixt0, L_INSERT);
        numaAddNumber(na, sat);
    }
    pixt1 = pixaDisplayTiledAndScaled(pixa, 32, w, 5, 0, 10, 2);
    pixSaveTiled(pixt1, pixaf, 1, 1, 20, 0);
    gplotSimple1(na, GPLOT_X11, "junkplot", "Average Saturation");
    pixWrite("/tmp/junksat.png", pixt1, IFF_PNG);
    pixDisplayWithTitle(pixt1, 900, 100, "Saturation", 1);
    numaDestroy(&na);
    pixDestroy(&pixt1);
    pixaDestroy(&pixa);

        /* Vary contrast */
    pixa = pixaCreate(20);
    for (i = 0; i < 20; i++) {
        pixt0 = pixContrastTRC(NULL, pixs, 0.1 * i);
        pixaAddPix(pixa, pixt0, L_INSERT);
    }
    pixt1 = pixaDisplayTiledAndScaled(pixa, 32, w, 5, 0, 10, 2);
    pixSaveTiled(pixt1, pixaf, 1, 1, 20, 0);
    pixWrite("/tmp/junkcontrast.png", pixt1, IFF_PNG);
    pixDisplayWithTitle(pixt1, 0, 400, "Contrast", 1);
    pixDestroy(&pixt1);
    pixaDestroy(&pixa);

        /* Vary sharpening */
    pixa = pixaCreate(20);
    for (i = 0; i < 20; i++) {
        pixt0 = pixUnsharpMasking(pixs, 3, 0.01 + 0.15 * i);
        pixaAddPix(pixa, pixt0, L_INSERT);
    }
    pixt1 = pixaDisplayTiledAndScaled(pixa, 32, w, 5, 0, 10, 2);
    pixSaveTiled(pixt1, pixaf, 1, 1, 20, 0);
    pixWrite("/tmp/junksharp.png", pixt1, IFF_PNG);
    pixDisplayWithTitle(pixt1, 300, 400, "Sharp", 1);
    pixDestroy(&pixt1);
    pixaDestroy(&pixa);

        /* Display results */
    pixd = pixaDisplay(pixaf, 0, 0);
    pixDisplay(pixd, 100, 100);
    pixWrite("/tmp/junkenhance.jpg", pixd, IFF_JFIF_JPEG);
    pixDestroy(&pixd);
    pixaDestroy(&pixaf);

    pixDestroy(&pix);
    pixDestroy(&pixs);

    /* -----------------------------------------------*
     *           Test global color transforms         *
     * -----------------------------------------------*/
        /* Make identical cmap and rgb images */
    pix = pixRead("wet-day.jpg");
    pixs1 = pixOctreeColorQuant(pix, 200, 0);
    pixs2 = pixRemoveColormap(pixs1, REMOVE_CMAP_TO_FULL_COLOR);
    pixEqual(pixs1, pixs2, &same);
    fprintf(stderr, "same: %d\n", same);

        /* Make a diagonal color transform matrix */
    kel = kernelCreate(3, 3);
    kernelSetElement(kel, 0, 0, 0.7);
    kernelSetElement(kel, 1, 1, 0.4);
    kernelSetElement(kel, 2, 2, 1.3);

        /* Apply to both cmap and rgb images. */
    pixt1 = pixMultMatrixColor(pixs1, kel);
    pixt2 = pixMultMatrixColor(pixs2, kel);
    pixEqual(pixt1, pixt2, &same);
    same ? fprintf(stderr, "1 and 2 are equal\n") :
           fprintf(stderr, "1 and 2 differ\n");

        /* Apply the same transform in the simpler interface */
    pixt3 = pixMultConstantColor(pixs1, 0.7, 0.4, 1.3);
    pixt4 = pixMultConstantColor(pixs2, 0.7, 0.4, 1.3);
    pixEqual(pixt3, pixt4, &same);
    same ? fprintf(stderr, "3 and 4 are equal\n") :
           fprintf(stderr, "3 and 4 differ\n");
    pixEqual(pixt1, pixt3, &same);
    same ? fprintf(stderr, "1 and 3 are equal\n") :
           fprintf(stderr, "1 and 3 differ\n");
    pixWrite("/tmp/junktrans.png", pixt1, IFF_PNG);

    pixDestroy(&pix);
    pixDestroy(&pixs1);
    pixDestroy(&pixs2);
    pixDestroy(&pixt1);
    pixDestroy(&pixt2);
    pixDestroy(&pixt3);
    pixDestroy(&pixt4);
    kernelDestroy(&kel);

    return 0;
}

