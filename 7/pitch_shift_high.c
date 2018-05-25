#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

/* 相関関数を計算する */
double calc_corr(double *s, int N, int m)
{
    int n;
    double r = 0.0;

    for (n = 0; n < N; n++) {
        r += s[n] * s[n + m];
    }

    return r;
}

/* 相関関数のピーク位置に基づいて周期を計算する */
int get_period(int period_min, int period_max, double *x, int corr_size)
{
    int m, period = period_min;
    double corr, corr_max = 0.0;

    for (m = period_min; m <= period_max; m++) {

        /* 相関関数を計算 */
        corr = calc_corr(x, corr_size, m);

        /* 相関関数のピークを求め、そのピーク位置を周期として取得する */
        if (corr > corr_max) {
            corr_max = corr; /* 相関関数のピーク値を更新 */
            period = m; /* 周期を取得 */
        }
    }

    return period;
}

/* オーバーラップアド */
void overwrap_add(int period, double *s0, double *s1)
{
    int n;

    for (n = 0; n < period; n++) {
        s1[period + n] = s0[period + n] * (period - n) / period; /* 単調減少の重みづけ */
        s1[period + n] += s0[n] * n / period;  /* 単調増加の重みづけ */
    }
}

/* オーバーラップアドされない入力信号を出力信号側にコピーする */
void copy_without_overwrap(int period, int q, int offset0, int offset1,
                           double *s0, double *s1, int s0_length)
{
    int n;

    for (n = period; n < q && offset0 + period + n < s0_length; n++) {
        s1[offset1 + period + n] = s0[offset0 + n];
    }
}

/* 再生時間を変えずに音の高さを高くするプログラム */
int main(void)
{
    MONO_PCM pcm0, pcm1, pcm2;
    int n, m, corr_size, min_period, max_period, period, q, offset0, offset1, offset, J;
    double rate, t, pitch;

    /* WAVEファイルからモノラルの音データを入力する */
    mono_wave_read(&pcm0, "wagahai_neko.wav");

    pitch = 1.5;
    rate = 1 / pitch; /* rateの範囲は 0.5 <= rate < 1.0 */

    pcm1.fs = pcm0.fs; /* 標本化周波数 */
    pcm1.bits = pcm0.bits; /* 量子化精度 */
    pcm1.length = (int)(pcm0.length / rate) + 1; /* 音データの長さ */
    pcm1.s = calloc(pcm1.length, sizeof(double)); /* メモリの確保 */

    /******** 音の長さを短くする タイムストレッチ ここから *********/
    /* 入力: pcm0 */
    corr_size = (int)(pcm0.fs * 0.01); /* 相関関数のサイズ 10ms */
    min_period = (int)(pcm0.fs * 0.005); /* 窓内における音データの周期の最小値 5ms */
    max_period = (int)(pcm0.fs * 0.02);  /* 窓内における音データの周期の最大値 20ms */

    offset0 = 0;                /* オーバーラップアドの開始位置 (入力側) */
    offset1 = 0;                /* オーバーラップアドの開始位置 (出力側) */
    while (offset0 + max_period * 2 < pcm0.length) {

        /* 窓内の音データに対して(窓サイズ corr_size)、
           相関関数を利用することで音の周期を計算する */
        period = get_period(min_period, max_period, pcm0.s + offset0, corr_size);

        /* オーバーラップアドしていない音データをそのままコピーする */
        for (n = 0; n < period; n++) {
            pcm1.s[offset1 + n] = pcm0.s[offset0 + n];
        }

        /* オーバーラップアドにより音データを時間的に伸ばす */
        overwrap_add(period, pcm0.s + offset0, pcm1.s + offset1);

        /* オーバーラップアドしていない音データをそのままコピーする */
        q = (int)(period * rate / (1.0 - rate) + 0.5); // offsetの更新時刻を決めるパラメータ
        for (n = period; n < q && offset0 + period + n < pcm0.length; n++) {
            pcm1.s[offset1 + period + n] = pcm0.s[offset0 + n];
        }

        /* offset0の更新 */
        offset0 += q;

        /* offset1の更新 */
        offset1 += period + q;
    }
    /* 出力: pcm1 */

    /******** タイムストレッチ ここまで *********/

    /********* リサンプリング ここから *********/
    /* 入力: pcm1 */
    /* 出力: pcm2 */

    pcm2.fs = pcm0.fs; /* 標本化周波数 */
    pcm2.bits = pcm0.bits; /* 量子化精度 */
    pcm2.length = pcm0.length; /* 音データの長さ (元の音の長さ) */
    pcm2.s = calloc(pcm2.length, sizeof(double)); /* メモリの確保 */

    J = 24;

    /* リサンプリングの処理 */

    for (n = 0; n < pcm1.length; n++) {
        t = pitch * n;          /* アナログ信号の時刻 t を求める */
        offset = t;             /* t を整数値に変換 */

        /* 標本化定理によるリサンプリング */
        for (m = offset - J / 2; m <= offset + J / 2; m++) {
            if (m >= 0 && m < pcm0.length) {
                pcm2.s[n] += pcm1.s[m] * sinc(M_PI * (t - m));
            }
        }
    }

    /********* リサンプリング ここまで *********/

    /* WAVEファイルにモノラルの音データを出力する */
    mono_wave_write(&pcm2, "pitch_shift_high.wav");

    free(pcm0.s); /* メモリの解放 */
    free(pcm1.s); /* メモリの解放 */
    free(pcm2.s); /* メモリの解放 */

    return 0;
}
