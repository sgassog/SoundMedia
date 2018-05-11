#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm0, pcm1;
    int n, m = 0;
    double d, depth, rate, t, tau = 0.0, delta;

    mono_wave_read(&pcm0, "sample06.wav"); /* WAVEファイルからモノラルの音データを入力する */

    pcm1.fs = pcm0.fs; /* 標本化周波数 */
    pcm1.bits = pcm0.bits; /* 量子化精度 */
    pcm1.length = pcm0.length; /* 音データの長さ */
    pcm1.s = calloc(pcm1.length, sizeof(double)); /* メモリの確保 */

    d = pcm1.fs * 0.025;
    depth = pcm1.fs * 0.010;
    rate = 0.1;

    /* コーラス */
    for (n = 0; n < pcm1.length; n++) {
        pcm1.s[n] = pcm0.s[n];
        /* ビブラートを参考にtauを実装 */
        tau = d+depth*sin(6.28*rate*n/pcm1.fs);
        t =  n - tau;
        m = t;
        delta = t - m;
        if (m >= 0 && m + 1 < pcm1.length) {
            /* ビブラートとブロック図を参考に実装 */
            pcm1.s[n] = delta * pcm0.s[m+1]+(1-delta)*pcm0.s[m];
            pcm1.s[n] = pcm1.s[n] + pcm0.s[n];
        }
    }

    mono_wave_write(&pcm1, "chorus.wav"); /* WAVEファイルにモノラルの音データを出力する */

    free(pcm0.s); /* メモリの解放 */
    free(pcm1.s); /* メモリの解放 */

    return 0;
}
