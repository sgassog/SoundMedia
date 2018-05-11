#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "wave.h"

void mono_wave_read(MONO_PCM *pcm, char *file_name)
{
    FILE *fp;
    int n;
    char riff_chunk_ID[4];
    int riff_chunk_size;
    char riff_form_type[4];
    char fmt_chunk_ID[4];
    int fmt_chunk_size;
    short fmt_wave_format_type;
    short fmt_channel;
    int fmt_samples_per_sec;
    int fmt_bytes_per_sec;
    short fmt_block_size;
    short fmt_bits_per_sample;
    char data_chunk_ID[4];
    int data_chunk_size;
    short data;
    int short_max = -SHRT_MIN;

    fp = fopen(file_name, "rb");
    if (fp == NULL) {
        fprintf(stderr, "error: %s is not found.\n", file_name);
        exit(1);
    }

    fread(riff_chunk_ID, 1, 4, fp);
    fread(&riff_chunk_size, 4, 1, fp);
    fread(riff_form_type, 1, 4, fp);
    fread(fmt_chunk_ID, 1, 4, fp);
    fread(&fmt_chunk_size, 4, 1, fp);
    fread(&fmt_wave_format_type, 2, 1, fp);
    fread(&fmt_channel, 2, 1, fp);
    fread(&fmt_samples_per_sec, 4, 1, fp);
    fread(&fmt_bytes_per_sec, 4, 1, fp);
    fread(&fmt_block_size, 2, 1, fp);
    fread(&fmt_bits_per_sample, 2, 1, fp);
    fread(data_chunk_ID, 1, 4, fp);
    fread(&data_chunk_size, 4, 1, fp);

    pcm->fs = fmt_samples_per_sec; /* 標本化周波数 */
    pcm->bits = fmt_bits_per_sample; /* 量子化精度 */
    pcm->length = data_chunk_size / 2; /* 音データの長さ */
    pcm->s = (double *) calloc(pcm->length, sizeof(double)); /* メモリの確保 */

    for (n = 0; n < pcm->length; n++) {
        fread(&data, 2, 1, fp); /* 音データの読み取り */
        pcm->s[n] = (double)data / short_max; /* 音データを-1以上1未満の範囲に正規化する */
    }

    fclose(fp);
}

void mono_wave_write(MONO_PCM *pcm, char *file_name)
{
    FILE *fp;
    int n;
    char riff_chunk_ID[4];
    int riff_chunk_size;
    char riff_form_type[4];
    char fmt_chunk_ID[4];
    int fmt_chunk_size;
    short fmt_wave_format_type;
    short fmt_channel;
    int fmt_samples_per_sec;
    int fmt_bytes_per_sec;
    short fmt_block_size;
    short fmt_bits_per_sample;
    char data_chunk_ID[4];
    int data_chunk_size;
    short data;
    int short_max = -SHRT_MIN;
    double s;

    riff_chunk_ID[0] = 'R';
    riff_chunk_ID[1] = 'I';
    riff_chunk_ID[2] = 'F';
    riff_chunk_ID[3] = 'F';
    riff_chunk_size = 36 + pcm->length * 2;
    riff_form_type[0] = 'W';
    riff_form_type[1] = 'A';
    riff_form_type[2] = 'V';
    riff_form_type[3] = 'E';

    fmt_chunk_ID[0] = 'f';
    fmt_chunk_ID[1] = 'm';
    fmt_chunk_ID[2] = 't';
    fmt_chunk_ID[3] = ' ';
    fmt_chunk_size = 16;
    fmt_wave_format_type = 1;
    fmt_channel = 1;
    fmt_samples_per_sec = pcm->fs; /* 標本化周波数 */
    fmt_bytes_per_sec = pcm->fs * pcm->bits / 8;
    fmt_block_size = pcm->bits / 8;
    fmt_bits_per_sample = pcm->bits; /* 量子化精度 */

    data_chunk_ID[0] = 'd';
    data_chunk_ID[1] = 'a';
    data_chunk_ID[2] = 't';
    data_chunk_ID[3] = 'a';
    data_chunk_size = pcm->length * 2;

    fp = fopen(file_name, "wb");

    fwrite(riff_chunk_ID, 1, 4, fp);
    fwrite(&riff_chunk_size, 4, 1, fp);
    fwrite(riff_form_type, 1, 4, fp);
    fwrite(fmt_chunk_ID, 1, 4, fp);
    fwrite(&fmt_chunk_size, 4, 1, fp);
    fwrite(&fmt_wave_format_type, 2, 1, fp);
    fwrite(&fmt_channel, 2, 1, fp);
    fwrite(&fmt_samples_per_sec, 4, 1, fp);
    fwrite(&fmt_bytes_per_sec, 4, 1, fp);
    fwrite(&fmt_block_size, 2, 1, fp);
    fwrite(&fmt_bits_per_sample, 2, 1, fp);
    fwrite(data_chunk_ID, 1, 4, fp);
    fwrite(&data_chunk_size, 4, 1, fp);

    for (n = 0; n < pcm->length; n++) {
        s = (pcm->s[n] + 1.0) / 2.0 * (2 * short_max);

        data = (short)(s + 0.5) - short_max; /* 四捨五入とオフセットの調節 */
        fwrite(&data, 2, 1, fp); /* 音データの書き出し */
    }

    fclose(fp);
}
