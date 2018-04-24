#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm;
    int n, i, num_sum;
    double f0, gain, max_val;
    char afplay_command[256];
    char out_file[] = "saw_wave_cos.wav";  /* 出力ファイル */

    pcm.fs = 44100; /* 標本化周波数 */
    pcm.bits = 16; /* 量子化精度 */
    pcm.length = pcm.fs * 1; /* 音データの長さ */
    pcm.s = calloc(pcm.length, sizeof(double)); /* 音データ */

    f0 = 500.0; /* 基本周波数 */

    /* 和の項数 */
    num_sum = 1;

    /* ノコギリ波 */
    for (i = 1; i <= num_sum; i++) {
        for (n = 0; n < pcm.length; n++) {
            /* 円周率πには定数マクロであるM_PIを用いよ */
            pcm.s[n] += 1.0 / i * cos(2.0 * M_PI * i * f0 * n / pcm.fs);
        }
    }
    max_val = calc_max(pcm.s);
    for (n = 0; n < pcm.length; n++) {
        pcm.s[n] /= max_val;
    }

    gain = 0.3; /* ゲイン */

    for (n = 0; n < pcm.length; n++) {
        pcm.s[n] *= gain;
    }

    /* 音をWAVEファイルに保存 */
    wave_write_16bit_mono(&pcm, out_file);

    /* 音を再生 */
    sprintf(afplay_command, "afplay %s", out_file);
    pclose(popen(afplay_command, "w"));

    free(pcm.s);

    return 0;
}
