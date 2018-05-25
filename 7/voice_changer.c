#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm0, pcm1;
    int n;
    double a, depth, rate;

    /* WAVEファイルからモノラルの音データを入力する */
    mono_wave_read(&pcm0, "wagahai_neko.wav");

    pcm1.fs = pcm0.fs; /* 標本化周波数 */
    pcm1.bits = pcm0.bits; /* 量子化精度 */
    pcm1.length = pcm0.length; /* 音データの長さ */
    pcm1.s = calloc(pcm1.length, sizeof(double)); /* メモリの確保 */

    depth = 1.0;
    rate = 2000;

    /* リング変調によるボイスチェンジャ */
    for (n = 0; n < pcm1.length; n++) {
      a = 1+depth*sin(6.28*rate*n/pcm1.fs);
      pcm1.s[n] = a*pcm0.s[n];
    }

    /* WAVEファイルにモノラルの音データを出力する */
    mono_wave_write(&pcm1, "voice_changer.wav");

    free(pcm0.s); /* メモリの解放 */
    free(pcm1.s); /* メモリの解放 */

    return 0;
}
