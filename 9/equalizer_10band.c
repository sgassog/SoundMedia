#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

/* ピーキングフィルタ */
void IIR_peaking(double fe, double Q, double g, double a[], double b[])
{
    double denom;               /* 分母 */

    double fc = tan(M_PI * fe) / (2.0 * M_PI);

    /* a[1], a[2], b[0], b[1], b[2] の計算 */
    denom = 1.0 + 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc;
    a[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0) / denom;
    a[2] = (1.0 - 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc) / denom;
    b[0] = (1.0 + (1.0 + g) * ((2.0*M_PI)*fc)/Q + 4.0*M_PI*M_PI*fc*fc) / denom;
    b[1] = (8.0 * M_PI * M_PI * fc * fc  - 2.0) / denom;
    b[2] = (1.0 - (1.0 + g) * ((2.0*M_PI)*fc)/Q + 4.0*M_PI*M_PI*fc*fc) / denom;
    /* b[0], b[1], b[2]について講義資料と上記を参考に書く */
}


/* イコライザ（１０帯域のピーキングフィルタ） */
int main(void)
{
    MONO_PCM pcm0, pcm1;
    int n, m, i, I, J;
    double fc, Q, g, a[3], b[3];

    /* 中心周波数 */
    double freq[10] = {
        /* ここに書く */
        31.25, 62.5, 125, 250, 500, 1000, 2000, 4000, 8000, 16000
    };

    /* 増幅率 */
    double gain[10] = {
        /* ここに書く */
        0.0, 2.0, 2.0, 0.0, -0.5, -0.5, -1.0, -1.0, 8.0, 8.0
    };

    /* クオリティファクター */
    double quality_factor[10] = {
        /* ここに書く */
        2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0
    };

    /* WAVEファイルからモノラルの音データを入力する */
    mono_wave_read(&pcm0, "sample04.wav");

    pcm1.fs = pcm0.fs; /* 標本化周波数 */
    pcm1.bits = pcm0.bits; /* 量子化精度 */
    pcm1.length = pcm0.length; /* 音データの長さ */
    pcm1.s = calloc(pcm1.length, sizeof(double)); /* メモリの確保 */

    /********** IIRフィルタによるイコライザ ***********/
    I = 2; /* 遅延器の数 */
    J = 2; /* 遅延器の数 */

    /* 10バンド イコライザ */
    for (i = 0; i < 10; i++) {   /* i がイコライザの通し番号を表す */
        fc = freq[i] / pcm0.fs; /* 中心周波数 */
        g = gain[i];                   /* 増幅率 */
        Q = quality_factor[i];         /* クオリティファクター */

        /* IIRフィルタの設計 */
        IIR_peaking(fc, Q, g, a, b);   /* ピーキングフィルタ */

        /****** IIRフィルタの適用 ********/
        for (n = 0; n < pcm1.length; n++) {
            /* 0で初期化 */
            pcm1.s[n] = 0.0;
        }

        /* IIRフィルタの計算 */
        for (n = 0; n < pcm1.length; n++) {
            /* ここに書く */
            for (m=1; m<=I; m++){
                if(n-m>=0){
                    pcm1.s[n] = pcm1.s[n] - a[m]*pcm1.s[n-m];
                }
            }
            for (m=0; m<=J; m++){
                if(n-m>=0){
                    pcm1.s[n] = pcm1.s[n] + b[m]*pcm0.s[n-m];
                }
            }
        }

        /* イコライザを重ねがけするために、
           計算済みのpcm1.sをpcm0.sにコピーして次の入力に用いる */
        for (n = 0; n < pcm1.length; n++) {
            pcm0.s[n] = pcm1.s[n];
        }
    }

    /* WAVEファイルにモノラルの音データを出力する */
    mono_wave_write(&pcm1, "equalizer_10band.wav");

    free(pcm0.s); /* メモリの解放 */
    free(pcm1.s); /* メモリの解放 */

    return 0;
}
