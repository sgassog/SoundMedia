#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm0, pcm1;
    int n;
    double gain, level;

    mono_wave_read(&pcm0, "sample02.wav"); /* WAVEファイルからモノラルの音データを入力する */

    pcm1.fs = pcm0.fs; /* 標本化周波数 */
    pcm1.bits = pcm0.bits; /* 量子化精度 */
    pcm1.length = pcm0.length; /* 音データの長さ */
    pcm1.s = calloc(pcm1.length, sizeof(double)); /* メモリの確保 */

    gain = 2000.0; /* 増幅率 */
    level = 0.5; /* レベル */

    /* プログラムを完成させよ */

    for (n = 0; n <= pcm1.length-1; n++) {
      pcm1.s[n] = gain * pcm0.s[n];
      if(pcm1.s[n]>1){
        pcm1.s[n] = 1;
      }else if(pcm1.s[n]<-1){
        pcm1.s[n] = -1;
      }
      pcm1.s[n] = level * pcm1.s[n];
    }

    mono_wave_write(&pcm1, "hard_distortion.wav");

    free(pcm0.s); /* メモリの解放 */
    free(pcm1.s); /* メモリの解放 */

    return 0;
}
