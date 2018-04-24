#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm;
    char afplay_command[256];
    char out_file[] = "mario_bgm.wav";

    pcm.fs = 44100; /* 標本化周波数 */
    pcm.bits = 16; /* 量子化精度 */
    pcm.length = 7000 * 16; /* 音データの長さ */
    pcm.s = calloc(pcm.length, sizeof(double)); /* 音データ */

    /* メロディパート */
    square_wave(&pcm, E5, 0.1, 7000 * 0, 6125); /* ミ */
    square_wave(&pcm, E5, 0.1, 7000 * 1, 6125); /* ミ */
    square_wave(&pcm, E5, 0.1, 7000 * 3, 6125); /* ミ */
    square_wave(&pcm, C5, 0.1, 7000 * 5, 6125); /* ド */
    square_wave(&pcm, E5, 0.1, 7000 * 6, 6125); /* ミ */
    square_wave(&pcm, G5, 0.1, 7000 * 8, 6125); /* ソ */
    square_wave(&pcm, G4, 0.1, 7000 * 12, 6125); /* ソ */

    /* ベースパート */
    triangle_wave(&pcm, D3, 0.2, 7000 * 0, 6125); /* レ */
    triangle_wave(&pcm, D3, 0.2, 7000 * 1, 6125); /* レ */
    triangle_wave(&pcm, D3, 0.2, 7000 * 3, 6125); /* レ */
    triangle_wave(&pcm, D3, 0.2, 7000 * 5, 6125); /* レ */
    triangle_wave(&pcm, D3, 0.2, 7000 * 6, 6125); /* レ */
    triangle_wave(&pcm, G3, 0.2, 7000 * 8, 6125); /* ソ */
    triangle_wave(&pcm, G3, 0.2, 7000 * 12, 6125); /* ソ */

    /* パーカッション */
    white_noise(&pcm, 0.1, 7000 * 0, 4000);
    white_noise(&pcm, 0.1, 7000 * 2, 1000);
    white_noise(&pcm, 0.1, 7000 * 3, 4000);
    white_noise(&pcm, 0.1, 7000 * 5, 1000);
    white_noise(&pcm, 0.1, 7000 * 6, 4000);
    white_noise(&pcm, 0.1, 7000 * 8, 4000);
    white_noise(&pcm, 0.1, 7000 * 11, 4000);
    white_noise(&pcm, 0.1, 7000 * 13, 1000);
    white_noise(&pcm, 0.1, 7000 * 14, 1000);
    white_noise(&pcm, 0.1, 7000 * 15, 1000);

    /* 音をWAVEファイルに保存 */
    wave_write_16bit_mono(&pcm, out_file);

    /* 音を再生 */
    sprintf(afplay_command, "afplay %s", out_file);
    pclose(popen(afplay_command, "w"));

    free(pcm.s);

    return 0;
}
