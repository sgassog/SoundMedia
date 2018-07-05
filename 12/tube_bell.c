#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

void ADSR(double e[], int A, int D, double S, int R, int gate, int duration)
{
    int n;

    if (A != 0) {
        for (n = 0; n < A; n++) {
            e[n] = 1.0 - exp(-5.0 * n / A);
        }
    }

    if (D != 0) {
        for (n = A; n < gate; n++) {
            e[n] = S + (1 - S) * exp(-5.0 * (n - A) / D);
        }
    }
    else {
        for (n = A; n < gate; n++) {
            e[n] = S;
        }
    }

    if (R != 0) {
        for (n = gate; n < duration; n++) {
            e[n] = e[gate - 1] * exp(-5.0 * (n - gate + 1) / R);
        }
    }
}

/* チューブラーベル */
int main(void)
{
    MONO_PCM pcm;
    int n, A, D, R, gate, duration;
    double *ac, fc, *am, fm, ratio, gain, S;

    pcm.fs = 44100; /* 標本化周波数 */
    pcm.bits = 16; /* 量子化精度 */
    pcm.length = pcm.fs * 4; /* 音データの長さ */
    pcm.s = calloc(pcm.length, sizeof(double)); /* 音データ */

    ac = calloc(pcm.length, sizeof(double));
    am = calloc(pcm.length, sizeof(double));

    /* キャリア振幅 */
    /* ADSR設定 */
    /* gate = ... */
    gate = 4 * pcm.fs;
    /* duration = ... */
    duration = 4 * pcm.fs;
    /* A = ... */
    A = 0 ;
    /* D = ... */
    D = 4 * pcm.fs;
    /* S = ... */
    S = 0.0;
    /* R = ... */
    R = 4 * pcm.fs;
    /* ADSR(ac, ...); */
    ADSR(ac,A,D,S,R,gate,duration);

    /* fc = ... */  /* キャリア周波数 */
    fc = 440;

    /* モジュレータ振幅 */
    /* ADSR設定 */
    /* gate = ... */
    gate = 4 * pcm.fs;
    /* duration = ... */
    duration = 4 * pcm.fs;
    /* A = ... */
    A = 0;
    /* D = ... */
    D = 2 * pcm.fs;
    /* S = ... */
    S = 0.0;
    /* R = ... */
    R = 2 * pcm.fs;
    /* ADSR(am, ...); */
    ADSR(am,A,D,S,R,gate,duration);

    /* ratio = ... */ /* 周波数比 */
    ratio = 3.5;
    /* fm = ... */  /* モジュレータ周波数 */
    fm = fc * ratio;

    /* FM音源 */
    for (n = 0; n < pcm.length; n++) {
        /* ここに書く */
        /* pcm.s[n] = ... */
        pcm.s[n] = ac[n]*sin(2*3.14*fc*n/pcm.fs+am[n]*sin(2*3.14*fm*n/pcm.fs));
    }

    gain = 0.1; /* ゲイン */
    for (n = 0; n < pcm.length; n++) {
        pcm.s[n] *= gain;
    }

    mono_wave_write(&pcm, "tube_bell.wav");

    free(pcm.s);
    free(ac);
    free(am);

    return 0;
}
