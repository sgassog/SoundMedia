#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm;
    int n;
    double *a0, *a1, *a2, *a3, *a4, *f0, *f1, *f2, *f3, *f4, gain;

    /* 各倍音における時刻0の振幅 */
    double a0_0, a1_0, a2_0, a3_0, a4_0;

    /* 各倍音における減衰のスピード */
    double t0, t1, t2, t3, t4;

    pcm.fs = 44100; /* 標本化周波数 */
    pcm.bits = 16; /* 量子化精度 */
    pcm.length = pcm.fs * 3; /* 音データの長さ */

    /* 音データのメモリの確保（中身は0で初期化） */
    pcm.s = calloc(pcm.length, sizeof(double));

    /* メモリの確保（中身は0で初期化）*/
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

    /* 時刻0の振幅を設定 */
    a0_0 = 1.0;
    /* ここに書く */
    /* a1_0 = ... */
    a1_0 = 0.8;
    a2_0 = 0.6;
    a3_0 = 0.5;
    a4_0 = 0.4;

    /* 振幅減衰のスピードを設定 (値が大きいほど早く減衰) */
    t0 = 4.0;
    /* ここに書く */
    /* t1 = ... */
    t1 = 2.0;
    t2 = 1.0;
    t3 = 0.5;
    t4 = 0.2;

    /* 時間エンベロープ */
    for (n = 0; n < pcm.length; n++) {
        a0[n] = a0_0 * exp(-5.0 * n / (pcm.fs * t0));
        /* ここに書く */
        /* a1[n] = ... */
        a1[n] = a1_0 * exp(-5.0 * n / (pcm.fs * t1));
        a2[n] = a2_0 * exp(-5.0 * n / (pcm.fs * t2));
        a3[n] = a3_0 * exp(-5.0 * n / (pcm.fs * t3));
        a4[n] = a4_0 * exp(-5.0 * n / (pcm.fs * t4));

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
        pcm.s[n] += a0 [n] * sin(2.0 * M_PI * f0[n] * n / pcm.fs);
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

    /* フェード処理（説明は省略）*/
    for (n = 0; n < pcm.fs * 0.01; n++) {
        pcm.s[n] *= (double)n / (pcm.fs * 0.01);
        pcm.s[pcm.length - n - 1] *= (double)n / (pcm.fs * 0.01);
    }

    mono_wave_write(&pcm, "piano.wav");

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
