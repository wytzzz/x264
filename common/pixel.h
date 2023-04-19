/*****************************************************************************
 * pixel.c: pixel metrics
 *****************************************************************************
 * Copyright (C) 2004-2019 x264 project
 *
 * Authors: Loren Merritt <lorenm@u.washington.edu>
 *          Fiona Glaser <fiona@x264.com>
            Henrik Gramner <henrik@gramner.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at licensing@x264.com.
 *****************************************************************************/

#ifndef X264_PIXEL_H
#define X264_PIXEL_H

// SSD assumes all args aligned
// other cmp functions assume first arg aligned
typedef int  (*x264_pixel_cmp_t)( pixel *, intptr_t, pixel *, intptr_t );
typedef void (*x264_pixel_cmp_x3_t)( pixel *, pixel *, pixel *, pixel *, intptr_t, int[3] );
typedef void (*x264_pixel_cmp_x4_t)( pixel *, pixel *, pixel *, pixel *, pixel *, intptr_t, int[4] );

//像素类型
//色度类型
enum
{
    PIXEL_16x16 = 0,
    PIXEL_16x8  = 1,
    PIXEL_8x16  = 2,
    PIXEL_8x8   = 3,
    PIXEL_8x4   = 4,
    PIXEL_4x8   = 5,
    PIXEL_4x4   = 6,

    /* Subsampled chroma only */
    PIXEL_4x16  = 7,  /* 4:2:2 */
    PIXEL_4x2   = 8,
    PIXEL_2x8   = 9,  /* 4:2:2 */
    PIXEL_2x4   = 10,
    PIXEL_2x2   = 11,
};

static const struct { uint8_t w, h; } x264_pixel_size[12] =
{
    { 16, 16 }, { 16, 8 }, { 8, 16 }, { 8, 8 }, { 8, 4 }, { 4, 8 }, { 4, 4 },
    {  4, 16 }, {  4, 2 }, { 2,  8 }, { 2, 4 }, { 2, 2 },
};

static const uint8_t x264_size2pixel[5][5] =
{
    { 0, },
    { 0, PIXEL_4x4, PIXEL_8x4, 0, 0 },
    { 0, PIXEL_4x8, PIXEL_8x8, 0, PIXEL_16x8 },
    { 0, },
    { 0, 0,        PIXEL_8x16, 0, PIXEL_16x16 }
};

static const uint8_t x264_luma2chroma_pixel[4][7] =
{
    { 0 },
    { PIXEL_8x8,   PIXEL_8x4,  PIXEL_4x8,  PIXEL_4x4, PIXEL_4x2, PIXEL_2x4, PIXEL_2x2 }, /* 4:2:0 */
    { PIXEL_8x16,  PIXEL_8x8,  PIXEL_4x16, PIXEL_4x8, PIXEL_4x4, PIXEL_2x8, PIXEL_2x4 }, /* 4:2:2 */
    { PIXEL_16x16, PIXEL_16x8, PIXEL_8x16, PIXEL_8x8, PIXEL_8x4, PIXEL_4x8, PIXEL_4x4 }, /* 4:4:4 */
};

//像素相关的结构
typedef struct
{
    x264_pixel_cmp_t  sad[8]; //函数指针数组，用于计算像素块之间的绝对差值和
    x264_pixel_cmp_t  ssd[8]; //函数指针数组，用于计算像素块之间的平方差值和
    x264_pixel_cmp_t satd[8]; //函数指针数组，用于计算像素块在 DCT 变换域中的绝对差值和。
    x264_pixel_cmp_t ssim[7]; //SSIM（Structural Similarity Index）函数指针数组，用于计算像素块之间的结构相似性指数。
    x264_pixel_cmp_t sa8d[4]; //SA8D（Sum of Absolute 8x8 Differences）函数指针数组，用于计算像素块内 8x8 子块之间的绝对差值和。
    x264_pixel_cmp_t mbcmp[8]; /* 宏块比较函数指针数组，用于宏块模式决策和子像素精细化运动估计 either satd or sad for subpel refine and mode decision */
    x264_pixel_cmp_t mbcmp_unaligned[8]; /* 未对齐的宏块比较函数指针数组，用于子像素的运动估计。 unaligned mbcmp for subpel */
    x264_pixel_cmp_t fpelcmp[8]; /*全像素块运动搜索时使用的函数指针数组，可以是 SATD 或 SAD。  either satd or sad for fullpel motion search */
    x264_pixel_cmp_x3_t fpelcmp_x3[7]; //多个全像素块运动搜索时使用的函数指针数组，其中 x3 表示每次可以处理三个像素块
    x264_pixel_cmp_x4_t fpelcmp_x4[7]; //多个全像素块运动搜索时使用的函数指针数组，其中 x4 表示每次可以处理四个像素块。
    x264_pixel_cmp_t sad_aligned[8]; /* 对齐的 SAD 函数指针数组，用于宏块比较。 Aligned SAD for mbcmp */
    int (*vsad)( pixel *, intptr_t, int ); //垂直 SAD 函数指针。
    int (*asd8)( pixel *pix1, intptr_t stride1, pixel *pix2, intptr_t stride2, int height ); //AS8D 函数指针，用于计算两个像素块内相邻 8x8 子块之间的绝对差值和。
    uint64_t (*sa8d_satd[1])( pixel *pix1, intptr_t stride1, pixel *pix2, intptr_t stride2 ); //SA8D 或 SATD 函数指针，用于计算两个像素块内相邻 8x8 子块之间的绝对差值和或在 DCT 变换域中的绝对差值和。

    uint64_t (*var[4])( pixel *pix, intptr_t stride ); //方差函数指针数组，用于计算像素块内的方差。
    int (*var2[4])( pixel *fenc, pixel *fdec, int ssd[2] ); //方差函数指针数组，用于计算两个像素块内的方差。
    uint64_t (*hadamard_ac[4])( pixel *pix, intptr_t stride ); //Hadamard 变换函数指针数组，用于计算像素块的 AC 系数。

    void (*ssd_nv12_core)( pixel *pixuv1, intptr_t stride1,
                           pixel *pixuv2, intptr_t stride2, int width, int height,
                           uint64_t *ssd_u, uint64_t *ssd_v ); // NV12 格式下的 SSD 函数指针，用于计算亮度和色度分量的平方差值和
    void (*ssim_4x4x2_core)( const pixel *pix1, intptr_t stride1,
                             const pixel *pix2, intptr_t stride2, int sums[2][4] ); //用于 SSIM 计算中的函数指针，计算 4x4 块内两幅图像的像素值和
    float (*ssim_end4)( int sum0[5][4], int sum1[5][4], int width ); //用于 SSIM 计算中的函数指针，计算像素块的结构相似性指数

    /* multiple parallel calls to cmp. */
    x264_pixel_cmp_x3_t sad_x3[7]; //多个像素块之间的 SAD 函数指针数组，其中 x3 表示每次可以处理三个像素块。
    x264_pixel_cmp_x4_t sad_x4[7];
    x264_pixel_cmp_x3_t satd_x3[7];
    x264_pixel_cmp_x4_t satd_x4[7];

    /* abs-diff-sum for successive elimination.
     * may round width up to a multiple of 16. */
    int (*ads[7])( int enc_dc[4], uint16_t *sums, int delta,
                   uint16_t *cost_mvx, int16_t *mvs, int width, int thresh );

    /* calculate satd or sad of V, H, and DC modes. */
    void (*intra_mbcmp_x3_16x16)( pixel *fenc, pixel *fdec, int res[3] ); //计算 16x16 帧内预测模式的像素块的 SAD（和值）。
    void (*intra_satd_x3_16x16) ( pixel *fenc, pixel *fdec, int res[3] ); //计算 16x16 帧内预测模式的像素块的 SATD（经过变换的和值）
    void (*intra_sad_x3_16x16)  ( pixel *fenc, pixel *fdec, int res[3] ); //计算 16x16 帧内预测模式的像素块的 SAD。
    void (*intra_mbcmp_x3_4x4)  ( pixel *fenc, pixel *fdec, int res[3] ); //计算 4x4 帧内预测模式的像素块的 SAD。
    void (*intra_satd_x3_4x4)   ( pixel *fenc, pixel *fdec, int res[3] );
    void (*intra_sad_x3_4x4)    ( pixel *fenc, pixel *fdec, int res[3] );
    void (*intra_mbcmp_x3_chroma)( pixel *fenc, pixel *fdec, int res[3] ); //计算色度分量的帧内预测模式的像素块的 SAD。
    void (*intra_satd_x3_chroma) ( pixel *fenc, pixel *fdec, int res[3] );
    void (*intra_sad_x3_chroma)  ( pixel *fenc, pixel *fdec, int res[3] );
    void (*intra_mbcmp_x3_8x16c) ( pixel *fenc, pixel *fdec, int res[3] );
    void (*intra_satd_x3_8x16c)  ( pixel *fenc, pixel *fdec, int res[3] );
    void (*intra_sad_x3_8x16c)   ( pixel *fenc, pixel *fdec, int res[3] );
    void (*intra_mbcmp_x3_8x8c)  ( pixel *fenc, pixel *fdec, int res[3] );
    void (*intra_satd_x3_8x8c)   ( pixel *fenc, pixel *fdec, int res[3] );
    void (*intra_sad_x3_8x8c)    ( pixel *fenc, pixel *fdec, int res[3] );
    void (*intra_mbcmp_x3_8x8)  ( pixel *fenc, pixel edge[36], int res[3] );
    void (*intra_sa8d_x3_8x8)   ( pixel *fenc, pixel edge[36], int res[3] );
    void (*intra_sad_x3_8x8)    ( pixel *fenc, pixel edge[36], int res[3] );
    /* find minimum satd or sad of all modes, and set fdec.
     * may be NULL, in which case just use pred+satd instead. */
    int (*intra_mbcmp_x9_4x4)( pixel *fenc, pixel *fdec, uint16_t *bitcosts );
    int (*intra_satd_x9_4x4) ( pixel *fenc, pixel *fdec, uint16_t *bitcosts );
    int (*intra_sad_x9_4x4)  ( pixel *fenc, pixel *fdec, uint16_t *bitcosts );
    int (*intra_mbcmp_x9_8x8)( pixel *fenc, pixel *fdec, pixel edge[36], uint16_t *bitcosts, uint16_t *satds );
    int (*intra_sa8d_x9_8x8) ( pixel *fenc, pixel *fdec, pixel edge[36], uint16_t *bitcosts, uint16_t *satds );
    int (*intra_sad_x9_8x8)  ( pixel *fenc, pixel *fdec, pixel edge[36], uint16_t *bitcosts, uint16_t *satds );
} x264_pixel_function_t;

#define x264_pixel_init x264_template(pixel_init)
void x264_pixel_init( int cpu, x264_pixel_function_t *pixf );
#define x264_pixel_ssd_nv12 x264_template(pixel_ssd_nv12)
void x264_pixel_ssd_nv12   ( x264_pixel_function_t *pf, pixel *pix1, intptr_t i_pix1, pixel *pix2, intptr_t i_pix2,
                             int i_width, int i_height, uint64_t *ssd_u, uint64_t *ssd_v );
#define x264_pixel_ssd_wxh x264_template(pixel_ssd_wxh)
uint64_t x264_pixel_ssd_wxh( x264_pixel_function_t *pf, pixel *pix1, intptr_t i_pix1, pixel *pix2, intptr_t i_pix2,
                             int i_width, int i_height );
#define x264_pixel_ssim_wxh x264_template(pixel_ssim_wxh)
float x264_pixel_ssim_wxh  ( x264_pixel_function_t *pf, pixel *pix1, intptr_t i_pix1, pixel *pix2, intptr_t i_pix2,
                             int i_width, int i_height, void *buf, int *cnt );
#define x264_field_vsad x264_template(field_vsad)
int x264_field_vsad( x264_t *h, int mb_x, int mb_y );

#endif
