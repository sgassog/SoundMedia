#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

#define LOW 0
#define MIDDLE 1
#define HIGH 2

void IIR_low_shelving(double fe, double Q, double g, double a[], double b[])
{
    double denom;               /* 分母 */

    double fc = tan(M_PI * fe) / (2.0 * M_PI);

    /* a[1], a[2], b[0], b[1], b[2] の計算 */
    denom = 1.0 + 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc;
    a[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0) / denom;
    a[2] = (1.0 - 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc) / denom;
    /* b[0], b[1], b[2]について講義資料と上記を参考に書く */
    b[0] = (4.0 * M_PI * M_PI * fc * fc) / denom;
    b[1] = (8.0 * M_PI * M_PI * fc * fc) / denom;
    b[2] = (4.0 * M_PI * M_PI * fc * fc) / denom;
}

void IIR_high_shelving(double fe, double Q, double g, double a[], double b[])
{
    double denom;               /* 分母 */

    double fc = tan(M_PI * fe) / (2.0 * M_PI);

    /* a[1], a[2], b[0], b[1], b[2] の計算 */
    denom = 1.0 + 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc;
    a[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0) / denom;
    a[2] = (1.0 - 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc) / denom;
    /* b[0], b[1], b[2]について講義資料と上記を参考に書く */
    b[0] = 1 / denom;
    b[1] = -2 / denom;
    b[2] = 1 / denom;
}

void IIR_peaking(double fe, double Q, double g, double a[], double b[])
{
    double denom;               /* 分母 */

    double fc = tan(M_PI * fe) / (2.0 * M_PI);

    /* a[1], a[2], b[0], b[1], b[2] の計算 */
    denom = 1.0 + 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc;
    a[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0) / denom;
    a[2] = (1.0 - 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc) / denom;
    /* b[0], b[1], b[2]について講義資料と上記を参考に書く */
    b[0] = 1.0+(1+g) + 2.0 * M_PI * fc / Q +(4.0 * M_PI * M_PI * fc * fc) / denom;
    b[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0) / denom;
    b[2] = 1.0-(1+g) + 2.0 * M_PI * fc / Q +(4.0 * M_PI * M_PI * fc * fc) / denom;
}

int main(void)
{
    MONO_PCM pcm0, pcm1;
    int n, m, i, I, J;
    double fc, Q, g, a[3], b[3];

    /* 遮断周波数 or 中心周波数
       ローシェルビング・ハイシェルビングのときは遮断周波数
       ピーキングのときは中心周波数 */
    double freq[3] = { 500, 1000, 2000};        /* 値を書く */

    /* 増幅率 */
    /* 低域（カット）、中域（ブースト）、高域（カット） */
    double gain[3] = { -1.0, 1.0, -1.0};        /* 値を書く */

    /* クオリティファクター */
    double quality_factor[3] = {        /* 値を書く */
      1.0 / sqrt(2.0), 1.0 / sqrt(2.0), 1.0 / sqrt(2.0)
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

    /* 3帯域; 低域(i=0), 中域(i=1), 高域(i=2) */
    for (i = 0; i < 3; i++) { /* i がイコライザの通し番号を表す */
        fc = freq[i] / pcm0.fs;         /* 遮断周波数 or 中心周波数 */
        g = gain[i];                    /* 増幅率 */
        Q = quality_factor[i];          /* クオリティファクター */

        /* 帯域に応じたイコライザの選択 ＆ IIRフィルタの設計 */
        switch (i) {
        case LOW: /* 低域 (i = 0) */
            IIR_low_shelving(fc, Q, g, a, b);   /* ローシェルビングフィルタ */
            break;
        case MIDDLE: /* 中域 (i = 1) */
            IIR_peaking(fc, Q, g, a, b);  /* ピーキングフィルタ */
            break;
        case HIGH: /* 高域 (i = 2) */
            IIR_high_shelving(fc, Q, g, a, b);  /* ハイシェルビングフィルタ */
            break;
        default:
            break;
        }

        /****** IIRフィルタの適用 ********/
        for (n = 0; n < pcm1.length; n++) {
            /* 0で初期化 */
            pcm1.s[n] = 0.0;
        }

        /* IIRフィルタの計算 */
        for (n = 0; n < pcm1.length; n++) {
            /* ここを実装 */
            for (m = 1; m <= I; m++) {
              if (n - m >= 0) {
                pcm1.s[n] += -a[m] * pcm1.s[n - m];
              }
            }

            for (m = 0; m <= J; m++) {
              if (n - m >= 0) {
                pcm1.s[n] += b[m] * pcm0.s[n - m];
              }
            }
        }

        /* 重要：重ねがけするため、フィルタリング結果を
           次のイコライザの入力用にセット */
        for (n = 0; n < pcm1.length; n++) {
            pcm0.s[n] = pcm1.s[n];
        }

    }

    /* WAVEファイルにモノラルの音データを出力する */
    mono_wave_write(&pcm1, "equalizer_3band_sample04.wav");

    free(pcm0.s); /* メモリの解放 */
    free(pcm1.s); /* メモリの解放 */

    return 0;
}
