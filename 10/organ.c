#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm;
    int n;
    double *a0, *a1, *a2, *a3, *a4, *f0, *f1, *f2, *f3, *f4, gain;

    pcm.fs = 44100; /* 標本化周波数 */
    pcm.bits = 16; /* 量子化精度 */
    pcm.length = pcm.fs * 4; /* 音データの長さ */
    pcm.s = calloc(pcm.length, sizeof(double)); /* 音データ */

    a0 = calloc(pcm.length, sizeof(double));
    a1 = calloc(pcm.length, sizeof(double));
    a2 = calloc(pcm.length, sizeof(double));
    a3 = calloc(pcm.length, sizeof(double));
    a4 = calloc(pcm.length, sizeof(double));

    f0 = calloc(pcm.length, sizeof(double));
    f1 = calloc(pcm.length, sizeof(double));
    f2 = calloc(pcm.length, sizeof(double));
    f3 = calloc(pcm.length, sizeof(double));
    f4 = calloc(pcm.length, sizeof(double));

    /* 時間エンベロープ */
    for (n = 0; n < pcm.length; n++) {
        a0[n] = 0.5;
        /* ここに書く */
        /* a1[n] = ... */
        a1[n] = 1.0;
        a2[n] = 0.7;
        a3[n] = 0.5;
        a4[n] = 0.3;

        f0[n] = 440;
        /* ここに書く */
        /* f1[n] = ... */
        f1[n] = 880;
        f2[n] = 1320;
        f3[n] = 1760;
        f4[n] = 2200;
    }

    /* 加算合成 */
    for (n = 0; n < pcm.length; n++) {
        pcm.s[n] += a0[n] * sin(2.0 * M_PI * f0[n] * n / pcm.fs);
        /* ここに書く */
        /* pcm.s[n] += a1[n] * ... */
        pcm.s[n] += a1 [n] * sin(2.0 * M_PI * f1[n] * n / pcm.fs);
        pcm.s[n] += a2 [n] * sin(2.0 * M_PI * f2[n] * n / pcm.fs);
        pcm.s[n] += a3 [n] * sin(2.0 * M_PI * f3[n] * n / pcm.fs);
        pcm.s[n] += a4 [n] * sin(2.0 * M_PI * f4[n] * n / pcm.fs);
    }

    gain = 0.1; /* ゲイン */

    for (n = 0; n < pcm.length; n++) {
        pcm.s[n] *= gain;
    }

    /* フェード処理（説明は省略） */
    for (n = 0; n < pcm.fs * 0.01; n++) {
        pcm.s[n] *= (double)n / (pcm.fs * 0.01);
        pcm.s[pcm.length - n - 1] *= (double)n / (pcm.fs * 0.01);
    }

    mono_wave_write(&pcm, "organ.wav");

    free(pcm.s);
    free(a0);
    free(a1);
    free(a2);
    free(a3);
    free(a4);
    free(f0);
    free(f1);
    free(f2);
    free(f3);
    free(f4);

    return 0;
}
