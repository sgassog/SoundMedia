#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

/* A ... アタックタイム */
/* D ... ディケイタイム */
/* S ... サスティンレベル */
/* R ... リリースタイム */
/* gate ... ゲート；鍵盤を押している時間 */
/* duration ... 音が鳴っている時間 */
void ADSR(double e[], int A, int D, double S, int R, int gate, int duration)
{
    int n;

    if (A != 0) {
        /* ここに書く */
        for (n = 0; n < A; n++) {
          e[n]=1-exp(-5*n/A);
        }
    }

    if (D != 0) {
        /* ここに書く */
        for (n = A; n < gate; n++) {
          e[n]=S+((1-S)*exp(-5*(n-A)/D));
        }
    } else {
        for (n = A; n < gate; n++) {
            e[n] = S;
        }
    }

    if (R != 0) {
        /* ここに書く */
        for (n = gate; n < duration; n++) {
          e[n]=e[gate-1]*exp(-5*(n-gate+1)/R);
        }
    }
}

int main(void)
{
    MONO_PCM pcm0, pcm1;
    int n, m, t0, I, J, A, D, R, gate, duration;
    double *vco, *vcf, *vca, gain, offset, depth, S, Q, a[3], b[3];

    /************** VCO：三角波 **************/
    pcm0.fs = 44100; /* 標本化周波数 */
    pcm0.bits = 16; /* 量子化精度 */
    pcm0.length = pcm0.fs * 1; /* 音データの長さ */
    pcm0.s = calloc(pcm0.length, sizeof(double)); /* 音データ */

    vco = calloc(pcm0.length, sizeof(double)); /* 基本周波数 */
    /* ここに書く */
    /* gate = ... */
    gate = pcm0.fs * 1;
    /* ADSR(...) */
    duration = pcm0.fs * 1;
    A = 0;
    D = pcm0.fs *0.4;
    S = 0;
    R = pcm0.fs *0.4;
    ADSR(vco, A, D, S, R, gate, duration);
    /* 時間エンベロープの設定 */
    /* 講義資料を参考にして書く */
    /* offset = ... */
    offset = 40.0;
    /* depth = ... */
    depth = 120.0;
    for (n = 0; n < pcm0.length; n++) {
        /* vco[n] = */
        vco[n] = offset + vco[n] * depth;
    }

    /* 三角波 */
    t0 = pcm0.fs / vco[0]; /* 基本周期 */
    m = 0;
    /* ここに三角波を生成する処理を書く (vco_triangle.cを参考に)*/
    /*********************************************/
    for (n = 0; n < pcm0.length; n++) {
        /* ここに書く */
        if (m < t0 / 2.0) {
          pcm0.s[n] = -1+((4.0*m)/t0);
        }else{
          pcm0.s[n] = 3-((4.0*m)/t0);
        }
        m++;
        if (m >= t0) {
          t0=pcm0.fs/vco[n];//基本周期
          m = 0;
        }
    }
    /************* VCF：IIRフィルタによるローパスフィルタ *************/
    vcf = calloc(pcm0.length, sizeof(double));  /* 遮断周波数 */
    /* ここに書く */
    /* gate = ... */
    gate = pcm0.fs * 1;
    /* ADSR(...) */
    duration = pcm0.fs * 1;
    A = 0;
    D = pcm0.fs *0.4;
    S = 0;
    R = pcm0.fs *0.4;
    ADSR(vcf, A, D, S, R, gate, duration);
    /* 時間エンベロープの設定 */
    /* 講義資料を参考にして書く */
    /* offset = ... */
    offset = 80.0;
    /* depth = ... */
    depth = 240.0;
    for (n = 0; n < pcm0.length; n++) {
        /* vcf[n] = */
        vcf[n] = offset + vcf[n] * depth;
    }

    Q = 5.0; /* レゾナンス */
    I = 2; /* 遅延器の数 */
    J = 2; /* 遅延器の数 */

    pcm1.fs = pcm0.fs; /* 標本化周波数 */
    pcm1.bits = pcm0.bits; /* 量子化精度 */
    pcm1.length = pcm0.length; /* 音データの長さ */
    pcm1.s = calloc(pcm1.length, sizeof(double)); /* 音データ */

    /* フィルタリング */
    for (n = 0; n < pcm1.length; n++) {
        IIR_LPF(vcf[n] / pcm1.fs, Q, a, b); /* IIRフィルタの設計 */

        for (m = 0; m <= J; m++) {
            if (n - m >= 0) {
                pcm1.s[n] += b[m] * pcm0.s[n - m];
            }
        }
        for (m = 1; m <= I; m++) {
            if (n - m >= 0) {
                pcm1.s[n] += -a[m] * pcm1.s[n - m];
            }
        }
    }

    /************** VCA：振幅のADSR *********************/
    vca = calloc(pcm1.length, sizeof(double)); /* 振幅 */
    /* ここに書く */
    /* gate = ... */
    /* ADSR(...) */
    gate = pcm0.fs * 1;
    duration = pcm0.fs * 1;
    A = 0;
    D = pcm0.fs * 0.4;
    S = 0;
    R = pcm0.fs * 0.4;
    ADSR(vca, A, D, S, R, gate, duration);
    gain = 0.9; /* ゲイン */
    for (n = 0; n < pcm1.length; n++) {
        pcm1.s[n] *= vca[n] * gain;
    }
    /***************************************************/

    /* フェード処理 */
    for (n = 0; n < pcm1.fs * 0.01; n++) {
        pcm1.s[n] *= (double)n / (pcm1.fs * 0.01);
        pcm1.s[pcm1.length - n - 1] *= (double)n / (pcm1.fs * 0.01);
    }

    mono_wave_write(&pcm1, "./output_wav/drum.wav");

    free(pcm0.s);
    free(pcm1.s);
    free(vco);
    free(vca);

    return 0;
}
