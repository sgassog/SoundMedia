#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void)
{
    STEREO_PCM pcm0; /* ステレオの音データ */
    MONO_PCM pcm1; /* モノラルの音データ */
    int n;

    stereo_wave_read(&pcm0, "test.wav"); /* WAVEファイルからステレオの音データを入力する */

    pcm1.fs = pcm0.fs; /* 標本化周波数 */
    pcm1.bits = pcm0.bits; /* 量子化精度 */
    pcm1.length = pcm0.length; /* 音データの長さ */
    pcm1.s = calloc(pcm1.length, sizeof(double)); /* メモリの確保 */

    /* ボーカルキャンセラ */
    for (n = 0; n < pcm1.length; n++) {
      pcm1.s[n] = pcm0.sL[n]-pcm0.sR[n];
    }

    mono_wave_write(&pcm1, "vocal_cancel.wav"); /* WAVEファイルにモノラルの音データを出力する */

    free(pcm0.sL); /* メモリの解放 */
    free(pcm0.sR); /* メモリの解放 */
    free(pcm1.s); /* メモリの解放 */

    return 0;
}
