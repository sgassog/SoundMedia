#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
    MONO_PCM pcm1;
    int n;
    double A, f0;
    char image_filename[] ="ex2_4.png"; /* 出力画像ファイル名 */

    pcm1.fs = 30; /* 標本化周波数 */
    pcm1.bits = 16; /* 量子化ビット数 */
    pcm1.length = 30; /* データの長さ */
    pcm1.s = (double *) calloc(pcm1.length, sizeof(double)); /* メモリの確保 */

    A = 0.5; /* 振幅 */
    f0 = 1.0; /* 基本周波数 */

    /* 正弦波 */
    for (n = 0; n < pcm1.length; n++) {
        pcm1.s[n] = A * sin(2.0 * M_PI * f0 * n / pcm1.fs);
    }

    /* 波形をプロットする */
    /* 中間ファイルの作成 */
    FILE *fp = fopen("temp.dat", "w");
    for (n = 0; n < pcm1.length; n++) {
        fprintf(fp, "%d %f\n", n, pcm1.s[n]);
    }
    fclose(fp);

    /* gnuplotを使う */
    FILE *gp = popen("gnuplot -p", "w"); /* gnuplotの立ち上げ */
    // fprintf(gp, "set terminal x11\n");   /* XQuartz */
    fprintf(gp, "set xrange [0:%d]\n", pcm1.length);
    fprintf(gp, "plot 'temp.dat' with impulse lw 2 lc rgb \"black\" notitle, "
            "'temp.dat' with points pt 6 notitle\n");

    /* 画像（PNG形式）で保存 */
    fprintf(gp, "set terminal png\n");
    fprintf(gp, "set output '%s'\n", image_filename);
    fprintf(gp, "replot\n");

    /* 以下は必須の処理 */
    fflush(gp);
    fprintf(gp, "quit\n"); /* gnuplotの終了 */
    pclose(gp);

    /* 一時ファイルの削除 */
    pclose(popen("rm ./temp.dat", "w"));

    /* メモリの解放 */
    free(pcm1.s);

    return 0;
}
