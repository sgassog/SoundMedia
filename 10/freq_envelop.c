#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm;
    int n;
    double a0, *f0, *g0;

    pcm.fs = 44100; /* 標本化周波数 */
    pcm.bits = 16; /* 量子化精度 */
    pcm.length = pcm.fs * 4; /* 音データの長さ */
    pcm.s = calloc(pcm.length, sizeof(double)); /* 音データ */

    a0 = 0.5; /* 振幅 */

    f0 = calloc(pcm.length, sizeof(double));
    g0 = calloc(pcm.length, sizeof(double));

    /* 周波数の時間エンベロープ */
    f0[0] = 2500.0;
    f0[pcm.length - 1] = 1500.0;
    /* f0を計算 */

    //変な副産物

    // for (n = 0; n < pcm.length; n++) {
    //     /* ここに書く */
    //     /* f0[n] = ... */
    //     f0[n] = f0[0] + ((f0[pcm.length - 1]-f0[0])/(2*(pcm.length-1)))*n;
    // }
    // /* g0を計算 */
    // for (n = 0; n < pcm.length; n++) {
    //     /* ここに書く */
    //     /* g0[n] = ... */
    //     g0[n] = g0[0] * n +((f0[pcm.length - 1]-f0[0])/(2*(pcm.length-1)))*n*n;
    // }

    for (n = 0; n < pcm.length; n++) {
        /* ここに書く */
        /* f0[n] = ... */
        f0[n] = f0[0] + ((f0[pcm.length - 1]-f0[0])/(2*(pcm.length-1)))*n;
    }
    /* g0を計算 */
    for (n = 0; n < pcm.length; n++) {
        /* ここに書く */
        /* g0[n] = ... */
        g0[n] = (f0[0]*n) +((f0[pcm.length - 1]-f0[0])/(2*(pcm.length-1)))*n*n;
    }

    /* 出力を計算 */
    for (n = 0; n < pcm.length; n++) {
        pcm.s[n] = a0 * sin(2.0 * M_PI * g0[n] / pcm.fs);
    }

    mono_wave_write(&pcm, "freq_envelop.wav");

    free(pcm.s);
    free(f0);
    free(g0);

    return 0;
}
