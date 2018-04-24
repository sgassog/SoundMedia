#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm;
    int n;
    double gain;
    char afplay_command[256];
    char out_file[] = "white_noise.wav";

    pcm.fs = 44100; /* 標本化周波数 */
    pcm.bits = 16; /* 量子化精度 */
    pcm.length = pcm.fs * 1; /* 音データの長さ */
    pcm.s = calloc(pcm.length, sizeof(double)); /* 音データ */

    /* 白色雑音 */
    for (n = 0; n < pcm.length; n++) {
        pcm.s[n] = (double)rand() / RAND_MAX * 2.0 - 1.0;
    }

    gain = 0.1; /* ゲイン */
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
