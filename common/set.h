/*****************************************************************************
 * set.h: quantization init
 *****************************************************************************
 * Copyright (C) 2003-2019 x264 project
 *
 * Authors: Loren Merritt <lorenm@u.washington.edu>
 *          Laurent Aimar <fenrir@via.ecp.fr>
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

#ifndef X264_SET_H
#define X264_SET_H

enum cqm4_e
{
    CQM_4IY = 0,
    CQM_4PY = 1,
    CQM_4IC = 2,
    CQM_4PC = 3
};
enum cqm8_e
{
    CQM_8IY = 0,
    CQM_8PY = 1,
    CQM_8IC = 2,
    CQM_8PC = 3,
};

//sps结构定义
typedef struct
{
    int i_id; //SPS的ID号

    int i_profile_idc; //视频编码的Profile级别
    int i_level_idc;   //视频编码的Level级别。

    int b_constraint_set0; //约束集标志位，表示是否启用了约束集0。
    int b_constraint_set1;
    int b_constraint_set2;
    int b_constraint_set3;
    int b_constraint_set4;
    int b_constraint_set5;

    int i_log2_max_frame_num;  //最大帧号的对数值

    int i_poc_type;  //POC（Picture Order Count）类型
    /* poc 0 */
    int i_log2_max_poc_lsb;

    int i_num_ref_frames;  //参考帧的数量。
    int b_gaps_in_frame_num_value_allowed; //是否允许帧号间跳跃。
    int i_mb_width; //宏块的宽度（以像素为单位）
    int i_mb_height;
    int b_frame_mbs_only;  // 是否只有帧内宏块。
    int b_mb_adaptive_frame_field; //是否支持宏块自适应场帧模式。
    int b_direct8x8_inference; //是否支持直接8x8推导模式。

    int b_crop; //是否进行裁剪
    struct
    {
        int i_left;
        int i_right;
        int i_top;
        int i_bottom;
    } crop;

    int b_vui;  //是否存在视频Usability Information（VUI）。

    /*
     * 这个结构体可能是用来描述视频编码中的VUI（Video Usability Information，视频可用性信息）参数。VUI参数是指在H.264/AVC视频编码标准中用来描述视频格式和性能的参数，
     * 包括帧率、分辨率、色度格式、色彩空间、色度采样、宽高比、HRD（Hypothetical Reference Decoder，虚拟参考解码器）等信息。
     * */
    struct
    {
        int b_aspect_ratio_info_present; //表示是否存在宽高比信息
        int i_sar_width; //表示像素宽度的比例因子。
        int i_sar_height;//表示像素高度的比例因子

        int b_overscan_info_present; //表示是否存在过扫描信息
        int b_overscan_info; //表示是否存在过扫描。

        int b_signal_type_present; //表示是否存在信号类型信息。
        int i_vidformat; //表示视频格式的标识符
        int b_fullrange; //表示是否使用全范围（0-255）的色度信息
        int b_color_description_present; //表示是否存在色彩描述信息。
        int i_colorprim; //表示色度的色度原色。
        int i_transfer; //表示色度的传输特性。
        int i_colmatrix; //表示色度的色度矩阵。

        int b_chroma_loc_info_present; //表示是否存在色度位置信息。
        int i_chroma_loc_top; //表示色度位置的上部分。
        int i_chroma_loc_bottom; //表示色度位置的下部分

        int b_timing_info_present; //表示是否存在时序信息。
        uint32_t i_num_units_in_tick; //表示每个时钟周期内的时间单位数量
        uint32_t i_time_scale;  //表示时间刻度
        int b_fixed_frame_rate; //表示是否使用固定帧率。

        int b_nal_hrd_parameters_present; //表示是否存在NAL HRD参数。
        int b_vcl_hrd_parameters_present; //表示是否存在VCL HRD参数。

        struct
        {
            int i_cpb_cnt;  //表示缓冲区数量
            int i_bit_rate_scale; //表示比特率缩放因子
            int i_cpb_size_scale; //表示缓冲区大小缩放因子
            int i_bit_rate_value; //表示比特率值
            int i_cpb_size_value; //表示缓冲区大小值。
            int i_bit_rate_unscaled; //表示未缩放的比特率值
            int i_cpb_size_unscaled; //表示未缩放的缓冲区大小值。
            int b_cbr_hrd;  //表示是否使用CBR（Constant Bit Rate）模式的HRD。

            int i_initial_cpb_removal_delay_length; //表示初始缓冲区移除延迟长度。
            int i_cpb_removal_delay_length; //表示缓冲区移除延迟长度。
            int i_dpb_output_delay_length;  //表示DPB（Decoded Picture Buffer）输出延迟长度。
            int i_time_offset_length;       //表示时间偏移长度
        } hrd; //hrd相关结构

        int b_pic_struct_present; //表示是否存在图像结构信息
        int b_bitstream_restriction; //表示是否存在比特流限制信息。
        int b_motion_vectors_over_pic_boundaries; //表示是否允许运动矢量超出图像边界。
        int i_max_bytes_per_pic_denom; //表示比特流限制中的最大每帧字节数的分母。
        int i_max_bits_per_mb_denom;  //表示比特流限制中的最大每宏块比特数的分母。
        int i_log2_max_mv_length_horizontal; //表示比特流限制中的水平运动矢量最大长度的对数。
        int i_log2_max_mv_length_vertical;  //表示比特流限制中的垂直运动矢量最大长度的对数。
        int i_num_reorder_frames;           //表示可重新排序的帧数的最大值。
        int i_max_dec_frame_buffering;      //表示解码器输出缓冲区的最大帧数。

        /* FIXME to complete */
    } vui;

    //这个标志位用于控制帧内预测和变换（transform）之间的关系，是否允许在变换之前直接使用像素值，而不进行预测。
    // 如果这个标志位被设置为1，表示允许直接使用像素值，否则需要先进行帧内预测。
    int b_qpprime_y_zero_transform_bypass;  //表示是否启用量化参数矩阵中的qpprime_y_zero_transform_bypass标志位
    //表示色度格式的标识符。色度格式是指视频中的色度信息的编码方式，包括YUV422、YUV420、YUV444等格式。
    // 不同的色度格式对视频压缩算法的效率和质量都有影响，需要根据具体的应用场景进行选择
    int i_chroma_format_idc;
    //表示是否启用AVC Intra编码。AVC Intra是一种基于H.264/AVC标准的视频编码格式，它采用了一些额外的编码技术，
    //可以提高编码质量和压缩比，适用于对视频质量要求较高的应用场景。
    int b_avcintra;
    //表示量化参数矩阵的预设值。量化参数矩阵是视频编码中用来控制视频质量和压缩比之间平衡的重要参数，不同的预设值会对编码效果产生不同的影响。
    int i_cqm_preset;
    // 用来存储预定义的量化参数矩阵。量化参数矩阵需要根据具体的应用场景进行选择和优化，预定义的量化矩阵可以作为编码器的默认选项，在某些情况下可以提高编码效率和质量。
    // 这个数组包含了8个指针，每个指针指向一个量化参数矩阵的数据结构。
    // 这个数组也可以扩展到12个指针，但是这个编码器不允许使用单独的Cb/Cr量化矩阵，因此只保留了8个指针。
    const uint8_t *scaling_list[8]; /* could be 12, but we don't allow separate Cb/Cr lists */

} x264_sps_t;

//pps结构定义
typedef struct
{
    int i_id;  //PPS的ID，用于标识不同的PPS
    int i_sps_id; //相关的SPS（Sequence Parameter Set）的ID，表示该PPS所依赖的SPS参数集。

    int b_cabac; //是否使用CABAC（Context Adaptive Binary Arithmetic Coding）熵编码技术

    int b_pic_order;  //b_pic_order表示当前编码的视频是否是基于帧（frame）的编码还是基于场（field）的编码。如果b_pic_order为1，则表示当前编码的视频是基于场的编码，否则为基于帧的编码。
    int i_num_slice_groups;  //切片组数量，用于多线程并行编码

    int i_num_ref_idx_l0_default_active; //L0参考帧列表的默认活跃参考帧数量。
    int i_num_ref_idx_l1_default_active; //L1参考帧列表的默认活跃参考帧数量。

    int b_weighted_pred; //是否启用加权预测。
    int b_weighted_bipred; //是否启用加权双向预测。

    int i_pic_init_qp; //图像的初始QP（Quantization Parameter）值。
    int i_pic_init_qs; //图像的初始QS（Quantization Scaling）值。

    int i_chroma_qp_index_offset; //色度分量的量化参数的偏移量

    int b_deblocking_filter_control; //是否启用去块滤波器
    int b_constrained_intra_pred; //是否启用约束内部预测。
    int b_redundant_pic_cnt; //是否启用冗余帧计数。

    int b_transform_8x8_mode; //是否启用8x8变换模式。

} x264_pps_t;

#define x264_cqm_init x264_template(cqm_init)
int  x264_cqm_init( x264_t *h );
#define x264_cqm_delete x264_template(cqm_delete)
void x264_cqm_delete( x264_t *h );
#define x264_cqm_parse_file x264_template(cqm_parse_file)
int  x264_cqm_parse_file( x264_t *h, const char *filename );

#endif
