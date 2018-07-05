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
    double vco, vcf, *vca, gain, S, Q, a[3], b[3];

    /************** VCO：ノコギリ波 **************/
    pcm0.fs = 44100; /* 標本化周波数 */
    pcm0.bits = 16; /* 量子化精度 */
    pcm0.length = pcm0.fs * 4; /* 音データの長さ */
    pcm0.s = calloc(pcm0.length, sizeof(double)); /* 音データ */

    vco = 440.0; /* 基本周波数 */

    t0 = pcm0.fs / vco; /* 基本周期 */
    m = 0;
    /* ここにノコギリ波を生成する処理を書く */
    for (n = 0; n < pcm0.length; n++) {
        /* ここに書く */
        if(m < t0){
          pcm0.s[n] = 1-((2.0*m)/t0);
        }
        m++;
        if (m >= t0) {
          m = 0;
        }
    }
    /*********************************************/

    /************* VCF：IIRフィルタによるローパスフィルタ *************/
    /* ここに書く */
    /* vcf = ... */ /* 遮断周波数 */

    vcf = 1500;

    Q = 5.0; /* レゾナンス */
    I = 2; /* 遅延器の数 */
    J = 2; /* 遅延器の数 */

    IIR_LPF(vcf / pcm0.fs, Q, a, b);  /* IIRフィルタの設計 */

    pcm1.fs = pcm0.fs; /* 標本化周波数 */
    pcm1.bits = pcm0.bits; /* 量子化精度 */
    pcm1.length = pcm0.length; /* 音データの長さ */
    pcm1.s = calloc(pcm1.length, sizeof(double)); /* 音データ */

    for (n = 0; n < pcm1.length; n++) {    /* IIR フィルタリング */
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
    /****************************************************************/

    /************** VCA：振幅のADSR *********************/
    /* 時間的に一定のレベル */
    vca = calloc(pcm0.length, sizeof(double)); /* 振幅 */
    /* ここに書く */
    /* gate = ... */
    gate = pcm0.fs * 4;
    /* ADSR(...) */
    duration = pcm0.fs * 4;
    A = 0;
    D = 0;
    S = 1.0;
    R = 0;
    ADSR(vca, A, D, S, R, gate, duration);
    for (n = 0; n < pcm1.length; n++) {
        /* 振幅変調 */
        pcm1.s[n] *= vca[n];
    }
    /***************************************************/

    /* ゲイン（全体の音量）調整 */
    gain = 0.1; /* ゲイン */
    for (n = 0; n < pcm1.length; n++) {
        pcm1.s[n] *= gain;
    }

    /* フェード処理 */
    for (n = 0; n < pcm1.fs * 0.01; n++) {
        pcm1.s[n] *= (double)n / (pcm1.fs * 0.01);
        pcm1.s[pcm1.length - n - 1] *= (double)n / (pcm1.fs * 0.01);
    }

    mono_wave_write(&pcm1, "./output_wav/organ.wav");

    /* メモリの開放 */
    free(pcm0.s);
    free(pcm1.s);
    free(vca);

    return 0;
}
