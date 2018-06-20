#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

/* 窓関数法に基づくFIRフィルタの設計（ハイパスフィルタ） */
void FIR_HPF(double fe, int J, double b[], double w[])
{
  int m, offset;
  offset = J / 2;
  for (m = 0; m <= J; m++) {
    b[m] = sinc(3.14*(m-offset))-2*fe*sinc(2*3.14*fe*(m-offset));
  }
  for (m = 0; m <= J; m++) {
    b[m] *= w[m];
  }
}

int main(void)
{
    MONO_PCM pcm0, pcm1;
    int n, m, J;
    double fe, delta, *b, *w;

    /* WAVEファイルからモノラルの音データを入力する */
    mono_wave_read(&pcm0, "sample_wav/sample10.wav");

    pcm1.fs = pcm0.fs; /* 標本化周波数 */
    pcm1.bits = pcm0.bits; /* 量子化精度 */
    pcm1.length = pcm0.length; /* 音データの長さ */
    pcm1.s = calloc(pcm1.length, sizeof(double)); /* メモリの確保 */

    fe = 3600.0 / pcm0.fs; /* エッジ周波数 */

    /* 遷移帯域幅 : 小さいほど遅延器の数Jが増えるので計算に時間がかかる */
    delta = 100.0 / pcm0.fs;

    J = (int)(3.1 / delta + 0.5) - 1; /* 遅延器の数 */
    if (J % 2 == 1) {
        J++; /* J+1が奇数になるように調整する */
    }

    b = calloc((J + 1), sizeof(double)); /* メモリの確保 */
    w = calloc((J + 1), sizeof(double)); /* メモリの確保 */

    Hanning_window(w, (J + 1)); /* ハニング窓の作成 */

    FIR_HPF(fe, J, b, w); /* ハイパスフィルタの設計 */

    /* ハイパスフィルタの計算 */
    for (n = 0; n < pcm1.length; n++) {
        /* ここに追記 */
        for (m = 0; m < J; m++) {
          if (n - m >= 0) {
            pcm1.s[n] += b[m] * pcm0.s[n - m];
          }
        }
    }

    /* WAVEファイルにモノラルの音データを出力する */
    mono_wave_write(&pcm1, "fir_high_pass.wav");

    free(pcm0.s); /* メモリの解放 */
    free(pcm1.s); /* メモリの解放 */
    free(b); /* メモリの解放 */
    free(w); /* メモリの解放 */

    return 0;
}
