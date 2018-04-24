#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm;
    int n;
    double a, f0;
    char out_filename[] ="ex2_1.wav"; /* 出力ファイル名 */

    pcm.fs = 44100; /* 標本化周波数 */
    pcm.bits = 16; /* 量子化精度 */
    pcm.length = pcm.fs; /* 音データの長さ */
    pcm.s = (double *) calloc(pcm.length, sizeof(double)); /* 音データ */

    a = 0.5; /* 振幅 */
    f0 = 500.0; /* 基本周波数 */

    /* 正弦波の作成 */
    for (n = 0; n < pcm.length; n++) {
        /* M_PI : 円周率を表す定数マクロ */
        pcm.s[n] = a * sin(2.0 * M_PI * f0 * n / pcm.fs);
    }

    /* 音データの出力 */
    wave_write_16bit_mono(&pcm, out_filename);

    /* メモリの開放 */
    free(pcm.s);

    return 0;
}
