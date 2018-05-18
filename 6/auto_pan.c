#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

/* オートパン */
int main(void)
{
    MONO_PCM pcm0; /* モノラルの音データ */
    STEREO_PCM pcm1; /* ステレオの音データ */
    int n;
    double aL, aR, depth, rate;

    mono_wave_read(&pcm0, "sample08.wav"); /* WAVEファイルからモノラルの音データを入力する */

    pcm1.fs = pcm0.fs; /* 標本化周波数 */
    pcm1.bits = pcm0.bits; /* 量子化精度 */
    pcm1.length = pcm0.length; /* 音データの長さ */
    pcm1.sL = calloc(pcm1.length, sizeof(double)); /* メモリの確保 (Lチャンネル) */
    pcm1.sR = calloc(pcm1.length, sizeof(double)); /* メモリの確保 (Rチャンネル) */

    depth = 1.0;
    rate = 0.2;

    /* オートパン */
    for (n = 0; n < pcm1.length; n++) {
        /* Lチャンネル */
        aL = 1 + depth*sin(6.28*rate*n/pcm1.fs);
        pcm1.sL[n] = aL*pcm0.s[n];
        /* Rチャンネル */
        aR = 1 - depth*sin(6.28*rate*n/pcm1.fs);
        pcm1.sR[n] = aR*pcm0.s[n];
    }

    stereo_wave_write(&pcm1, "auto_pan.wav"); /* WAVEファイルにステレオの音データを出力する */

    free(pcm0.s); /* メモリの解放 */
    free(pcm1.sL); /* メモリの解放 */
    free(pcm1.sR); /* メモリの解放 */

    return 0;
}
