#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

/* 疑似ステレオ化 */
int main(void)
{
    MONO_PCM pcm0; /* モノラルの音データ */
    STEREO_PCM pcm1; /* ステレオの音データ */
    int n, m;
    double d;

    mono_wave_read(&pcm0, "sample09.wav"); /* WAVEファイルからモノラルの音データを入力する */

    pcm1.fs = pcm0.fs; /* 標本化周波数 */
    pcm1.bits = pcm0.bits; /* 量子化精度 */
    pcm1.length = pcm0.length; /* 音データの長さ */
    pcm1.sL = calloc(pcm1.length, sizeof(double)); /* メモリの確保 (左チャンネル) */
    pcm1.sR = calloc(pcm1.length, sizeof(double)); /* メモリの確保 (右チャンネル) */

    /* ディレイ時間 (単位はサンプル数) */
    d = pcm1.fs * 0.01;

    /* 疑似ステレオ化 */
    for (n = 0; n < pcm1.length; n++) {
        m = n - d;
        if (m>=0) {
          pcm1.sL[n] =pcm0.s[n]+pcm0.s[m];
          pcm1.sR[n] =pcm0.s[n]-pcm0.s[m];
        }
    }

    stereo_wave_write(&pcm1, "pseudo_stereo_0.01.wav"); /* WAVEファイルにステレオの音データを出力する */

    free(pcm0.s); /* メモリの解放 */
    free(pcm1.sL); /* メモリの解放 */
    free(pcm1.sR); /* メモリの解放 */

    return 0;
}
