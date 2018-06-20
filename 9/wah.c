#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

/* 共振フィルタ */
void IIR_resonator(double fe, double Q, double a[], double b[])
{
    double denom;

    /* 中心周波数 */
    double fc = tan(M_PI * fe) / (2.0 * M_PI);

    /* フィルタ係数の分母 (denominator) */
    denom = 1.0 + 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc;

    /* ここに書く */

    a[1] = (8.0 * M_PI * M_PI * fc * fc - 2.0) / denom;
    a[2] = 1.0 - 2.0 * M_PI * fc / Q + 4.0 * M_PI * M_PI * fc * fc / denom;
    /* b[0], b[1], b[2]について講義資料と上記を参考に書く */
    b[0] = 2.0 * M_PI * fc / Q / denom;
    b[1] = 0;
    b[2] = -2.0 * M_PI * fc  / denom;
}

int main(void)
{
    MONO_PCM pcm0, pcm1;
    int n, m, I, J;
    double fc = 0.0, Q, depth, rate, a[3] = {0.0}, b[3] = {0.0};

    /* WAVEファイルからモノラルの音データを入力する */
    mono_wave_read(&pcm0, "sample04.wav");

    pcm1.fs = pcm0.fs; /* 標本化周波数 */
    pcm1.bits = pcm0.bits; /* 量子化精度 */
    pcm1.length = pcm0.length; /* 音データの長さ */
    pcm1.s = calloc(pcm1.length, sizeof(double)); /* メモリの確保 */

    I = 2; /* 遅延器の数 */
    J = 2; /* 遅延器の数 */

    depth = 800.0;
    rate = 2.0;

    /* ワウ */
    for (n = 0; n < pcm1.length; n++) {
        /* 中心周波数 (pcm1.fsで割るのを忘れないように！) */
        /* こんな感じで書く → fc = (...) / pcm1.fs */

        fc = (1000 + depth * sin(2*3.14*rate*n/pcm1.fs))/pcm1.fs;

        /* クオリティファクタ */
        Q = 2.0;

        /* 共振フィルタの設計 */
        IIR_resonator(fc, Q, a, b);

        /* IIRフィルタの計算 */
        /* ここに書く */
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

    /* WAVEファイルにモノラルの音データを出力する */
    mono_wave_write(&pcm1, "wah.wav");

    free(pcm0.s); /* メモリの解放 */
    free(pcm1.s); /* メモリの解放 */

    return 0;
}
