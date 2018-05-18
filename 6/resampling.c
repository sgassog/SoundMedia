#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

/* sinc関数を計算し、値を返す関数 */
double sinc(double x)
{
  if(x==0){
    x=1;
  }else{
    x = sin(x)/x;
  }
  return x;
};

int main(void)
{
  MONO_PCM pcm0, pcm1;
  int n, m, J, offset;
  double t, pitch;

  mono_wave_read(&pcm0, "sample12.wav"); /* WAVEファイルからモノラルの音データを入力する */

  pitch = 0.5; /* 音の高さをpitch倍にする */

  pcm1.fs = pcm0.fs; /* 標本化周波数の変更 */
  pcm1.bits = pcm0.bits; /* 量子化精度 */
  pcm1.length = (int)(pcm0.length / pitch); /* 音データの長さ */
  pcm1.s = calloc(pcm1.length, sizeof(double)); /* メモリの確保 */

  /* 和の項数は J + 1 */
  J = 24;

  for (n = 0; n < pcm1.length; n++) {
    t = pitch * n;          /* アナログ信号の時刻 t を求める */
    offset = (int) t;             /* t を整数値に変換 */

    /* 標本化定理によるリサンプリング */
    for (m = offset-(J/2); m < offset+(J/2); m++) {
      if (m>=0 && m<pcm0.length) {
        pcm1.s[n] += pcm0.s[m]*sinc(3.14*(t-m));
      }
    }
  }

  mono_wave_write(&pcm1, "resampling.wav"); /* WAVEファイルにモノラルの音データを出力する */

  free(pcm0.s); /* メモリの解放 */
  free(pcm1.s); /* メモリの解放 */

  return 0;
}
