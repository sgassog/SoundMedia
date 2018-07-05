#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm;
    int n;
    double ac, fc, am, fm, ratio, gain;

    pcm.fs = 44100; /* 標本化周波数 */
    pcm.bits = 16; /* 量子化精度 */
    pcm.length = pcm.fs * 1; /* 音データの長さ */
    pcm.s = calloc(pcm.length, sizeof(double)); /* 音データ */

    /* ac : キャリア振幅 */
    /* fc : キャリア周波数 */
    /* am : モジュレータ振幅 */
    /* fm : モジュレータ周波数 */
    /* ratio : 周波数比 */

    /* 講義資料を参考にして書く↓ */
    /* ac = ... */
    ac = 1.0;
    /* fc = ... */
    fc = 500;
    /* am = ... */
    am =1.0;
    /* ratio = ... */
    ratio = 3.5;
    /* fm = ... */
    fm = fc * ratio;

    /* FM音源 */
    for (n = 0; n < pcm.length; n++) {
        /* ここに書く */
        /* pcm.s[n] = ... */
        pcm.s[n] = ac * sin((2*3.14*fc*n/pcm.fs)+am*sin(2*3.14*fm*n/pcm.fs));
    }

    gain = 0.1; /* ゲイン */
    for (n = 0; n < pcm.length; n++) {
        pcm.s[n] *= gain;
    }

    mono_wave_write(&pcm, "fm_ratio35.wav");

    free(pcm.s);

    return 0;
}
