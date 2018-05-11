#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#ifndef WAVE_H
#define WAVE_H

typedef struct
{
    int fs; /* 標本化周波数 */
    int bits; /* 量子化精度 */
    int length; /* 音データの長さ */
    double *s; /* 音データ */
} MONO_PCM;

typedef struct
{
    int fs; /* 標本化周波数 */
    int bits; /* 量子化精度 */
    int length; /* 音データの長さ */
    double *sL; /* 音データ（Lチャンネル） */
    double *sR; /* 音データ（Rチャンネル） */
} STEREO_PCM;

void mono_wave_read(MONO_PCM *pcm, char *file_name);
void mono_wave_write(MONO_PCM *pcm, char *file_name);

#endif
