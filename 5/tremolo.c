#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm0, pcm1;
    int n;
    double a = 0.0, depth, rate;

    mono_wave_read(&pcm0, "sample05.wav"); /* WAVEファイルからモノラルの音データを入力する */

    pcm1.fs = pcm0.fs; /* 標本化周波数 */
    pcm1.bits = pcm0.bits; /* 量子化精度 */
    pcm1.length = pcm0.length; /* 音データの長さ */
    pcm1.s = calloc(pcm1.length, sizeof(double)); /* メモリの確保 */

    depth = 0.5;
    rate = 5.0;

    /* トレモロ */
    for (n = 0; n < pcm1.length; n++) {
        a = 1+depth * sin(6.28*rate*n/pcm1.fs);
        pcm1.s[n] = a * pcm0.s[n];
    }

    mono_wave_write(&pcm1, "tremolo.wav"); /* WAVEファイルにモノラルの音データを出力する */

    free(pcm0.s); /* メモリの解放 */
    free(pcm1.s); /* メモリの解放 */

    return 0;
}
