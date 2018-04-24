#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

void wave_read_8bit_mono(MONO_PCM *pcm, char *file_name);
void wave_write_8bit_mono(MONO_PCM *pcm, char *file_name);
void wave_read_8bit_stereo(STEREO_PCM *pcm, char *file_name);
void wave_write_8bit_stereo(STEREO_PCM *pcm, char *file_name);
void wave_read_16bit_mono(MONO_PCM *pcm, char *file_name);
void wave_write_16bit_mono(MONO_PCM *pcm, char *file_name);
void wave_read_16bit_stereo(STEREO_PCM *pcm, char *file_name);
void wave_write_16bit_stereo(STEREO_PCM *pcm, char *file_name);
void wave_read_PCMU_mono(MONO_PCM *pcm, char *file_name);
void wave_write_PCMU_mono(MONO_PCM *pcm, char *file_name);
void wave_read_PCMA_mono(MONO_PCM *pcm, char *file_name);
void wave_write_PCMA_mono(MONO_PCM *pcm, char *file_name);
void wave_read_IMA_ADPCM_mono(MONO_PCM *pcm, char *file_name);
void wave_write_IMA_ADPCM_mono(MONO_PCM *pcm, char *file_name);

// コード進行のための各種定義
#define C4 261.63               /* ド */
#define CSHARP4 277.18          /* ド♯ */
#define D4 293.66               /* レ */
#define DSHARP4 311.13          /* レ♯ */
#define E4 329.63               /* ミ */
#define F4 349.23               /* ファ */
#define FSHARP4 369.99          /* ファ♯ */
#define G4 392.00               /* ソ */
#define GSHARP4 415.30          /* ソ♯ */
#define A4 440.00               /* ラ */
#define ASHARP4 466.16          /* ラ♯ */
#define B4 493.88               /* シ */

#define C3 (C4/2)               /* ド */
#define CSHARP3 (CSHARP4/2)     /* ド♯ */
#define D3 (D4/2)               /* レ */
#define DSHARP3 (DSHARP4/2)     /* レ♯ */
#define E3 (E4/2)               /* ミ */
#define F3 (F4/2)               /* ファ */
#define FSHARP3 (FSHARP4/2)     /* ファ♯ */
#define G3 (G4/2)               /* ソ */
#define GSHARP3 (GSHARP4/2)     /* ソ♯ */
#define A3 (A4/2)               /* ラ */
#define ASHARP3 (ASHARP4/2)     /* ラ♯ */
#define B3 (B4/2)               /* シ */

#define C5 (C4*2)               /* ド */
#define CSHARP5 (CSHARP4*2)      /* ド♯ */
#define D5 (D4*2)               /* レ */
#define DSHARP5 (DSHARP4*2)     /* レ♯ */
#define E5 (E4*2)               /* ミ */
#define F5 (F4*2)               /* ファ */
#define FSHARP5 (FSHARP4*2)     /* ファ♯ */
#define G5 (G4*2)               /* ソ */
#define GSHARP5 (GSHARP4*2)     /* ソ♯ */
#define A5 (A4*2)               /* ラ */
#define ASHARP5 (ASHARP4*2)     /* ラ♯ */
#define B5 (B4*2)               /* シ */

#define TRIANGLE 0
#define RECTANGLE 1
#define SAW 2

typedef enum _WAVE_TYPE {
    triangle,
    rectangle,
    saw
} WAVE_TYPE;

// 三角波生成
void triangle_wave(MONO_PCM *pcm, double f0, double gain, int offset, int duration);
void triangle_wave_sin(MONO_PCM *pcm, double f0, int offset, int duration);

// 矩形波生成
void square_wave(MONO_PCM *pcm, double f0, double gain, int offset, int duration);
void square_wave_sin(MONO_PCM *pcm, double f0, int offset, int duration);

// ノコギリ波生成
void saw_wave(MONO_PCM *pcm, double f0, double gain, int offset, int duration);
void saw_wave_sin(MONO_PCM *pcm, double f0, int offset, int duration);

// 白色雑音
void white_noise(MONO_PCM *pcm, double gain, int offset, int duration);

// 波形生成（三角波、矩形波、ノコギリ波）
// WAVE_TYPE ... 波形を指定
// f0 ... 基本周波数
// offset ... 開始位置
// duration ... 波形の長さ（データ数）
void make_wave(WAVE_TYPE wave_type, MONO_PCM *pcm, double f0, double gain,
               int offset, int duration);

// 和音生成
// WAVE_TYPE ... 波形を指定
// f0 ... 基本周波数
// chord ... 和音を表す文字列（"C", "Dm", "Em", "F", "G", "Am"のいずれか）
// offset ... 開始位置
// duration ... 波形の長さ（データ数）
void make_chord(WAVE_TYPE wave_type, MONO_PCM *pcm, char *chord,
                double gain, int offset, int duration);

void make_chord_test(WAVE_TYPE wave_type, MONO_PCM *pcm, char *chord,
                     double gain, int offset, int duration);

double calc_max(double *all);

#endif
