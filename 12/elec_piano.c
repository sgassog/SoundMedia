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

int main(void)
{
    MONO_PCM pcm0, pcm1;
    int n, A, D, R, gate, duration;
    double *ac, fc, *am, fm, ratio, gain, S;

    /* pcm0 減衰音 */
    pcm0.fs = 44100; /* 標本化周波数 */
    pcm0.bits = 16; /* 量子化精度 */
    pcm0.length = pcm0.fs * 4; /* 音データの長さ */
    pcm0.s = calloc(pcm0.length, sizeof(double)); /* 音データ */

    /* pcm1 打撃音 */
    pcm1.fs = pcm0.fs; /* 標本化周波数 */
    pcm1.bits = pcm0.bits; /* 量子化精度 */
    pcm1.length = pcm0.length; /* 音データの長さ */
    pcm1.s = calloc(pcm1.length, sizeof(double)); /* 音データ */

    ac = calloc(pcm0.length, sizeof(double));
    am = calloc(pcm0.length, sizeof(double));

    /***************** 減衰音(pcm0) *****************/
    /* キャリア振幅 */
    /* ADSR設定 */
    /* gate = ... */
    gate = 4 * pcm0.fs;
    /* duration = ... */
    duration = 4 * pcm0.fs;
    /* A = ... */
    A = 0;
    /* D = ... */
    D = 4 * pcm0.fs;
    /* S = ... */
    S = 0.0;
    /* R = ... */
    R = 1 * pcm0.fs;
    /* ADSR(ac, ...); */
    ADSR(am,A,D,S,R,gate,duration);

    /* fc = ... */  /* キャリア周波数 */
    fc = 440;

    /* モジュレータ振幅 */
    /* ADSR設定 */
    /* gate = ... */
    gate = 4 * pcm0.fs;
    /* duration = ... */
    duration = 4 * pcm0.fs;
    /* A = ... */
    A = 0;
    /* D = ... */
    D = 2 * pcm0.fs;
    /* S = ... */
    S = 0.0;
    /* R = ... */
    R = 2 * pcm0.fs;
    /* ADSR(am, ...); */
    ADSR(am,A,D,S,R,gate,duration);

    /* ratio = ... */ /* 周波数比 */
    ratio = 1.0;
    /* fm = ... */  /* モジュレータ周波数 */
    fm = fc * ratio;

    /* FM音源(減衰音) */
    for (n = 0; n < pcm0.length; n++) {
        /* ここに書く */
        /* pcm.s[n] = ... */
        pcm0.s[n] = ac[n] * sin(2*3.14*fc*n/pcm0.fs+am[n]*sin(2*3.14*fm*n/pcm0.fs));
    }
    /******************************************/

    /***************** 打撃音(pcm1) *****************/
    /* キャリア振幅 */
    /* ADSR設定 */
    /* gate = ... */
    gate = 4 * pcm1.fs;
    /* duration = ... */
    duration = 4 * pcm1.fs;
    /* A = ... */
    A = 0 * pcm1.fs;
    /* D = ... */
    D = 1 * pcm1.fs;
    /* S = ... */
    S = 0.0;
    /* R = ... */
    R = 1 * pcm1.fs;
    /* ADSR(ac, ...); */
    ADSR(ac,A,D,S,R,gate,duration);

    /* fc = ... */  /* キャリア周波数 */
    fc = 440;

    /* モジュレータ振幅 */
    /* ADSR設定 */
    /* gate = ... */
    gate = 4 * pcm1.fs;
    /* duration = ... */
    duration = 4 * pcm1.fs;
    /* A = ... */
    A = 0;
    /* D = ... */
    D = 1 * pcm1.fs;
    /* S = ... */
    S = 0.0;
    /* R = ... */
    R = 1 * pcm1.fs;
    /* ADSR(am, ...); */
    ADSR(am,A,D,S,R,gate,duration);

    /* ratio = ... */ /* 周波数比 */
    ratio = 14.0;
    /* fm = ... */  /* モジュレータ周波数 */
    fm = fc * ratio;

    /* FM音源(打撃音) */
    for (n = 0; n < pcm1.length; n++) {
        /* ここに書く */
        /* pcm.s[n] = ... */
        pcm1.s[n] = ac[n] * sin(2*3.14*fc*n/pcm1.fs+am[n]*sin(2*3.14*fm*n/pcm1.fs));
    }
    /******************************************/

    /* 減衰音(pcm0)と打撃音(pcm1)の重ね合わせ */
    for (n = 0; n < pcm1.length; n++) {
        /* pcm1.s[n] = ... */
        pcm1.s[n] = pcm0.s[n] + pcm1.s[n];
    }

    gain = 0.1; /* ゲイン */
    for (n = 0; n < pcm1.length; n++) {
        pcm1.s[n] *= gain;
    }

    mono_wave_write(&pcm1, "elec_piano.wav");

    free(pcm0.s);
    free(pcm1.s);
    free(ac);
    free(am);

    return 0;
}
