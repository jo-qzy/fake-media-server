//
// Created by quzhenyu on 2021/12/6.
// Copyright (c) 2021 quzhenyu. All rights reserved.
//

#include "mpeg4_avc.h"

#include <assert.h>

/*
ISO/IEC 14496-15:2010(E)

aligned(8) class AVCDecoderConfigurationRecord {
    unsigned int(8) configurationVersion = 1;
    unsigned int(8) AVCProfileIndication;
    unsigned int(8) profile_compatibility;
    unsigned int(8) AVCLevelIndication;
    bit(6) reserved = ‘111111’b;
    unsigned int(2) lengthSizeMinusOne;
    bit(3) reserved = ‘111’b;
    unsigned int(5) numOfSequenceParameterSets;

    for (i=0; i< numOfSequenceParameterSets; i++) {
        unsigned int(16) sequenceParameterSetLength ;
        bit(8*sequenceParameterSetLength) sequenceParameterSetNALUnit;
    }

    unsigned int(8) numOfPictureParameterSets;
    for (i=0; i< numOfPictureParameterSets; i++) {
        unsigned int(16) pictureParameterSetLength;
        bit(8*pictureParameterSetLength) pictureParameterSetNALUnit;
    }

    if( profile_idc == 100 || profile_idc == 110 ||
    profile_idc == 122 || profile_idc == 144 )
    {
        bit(6) reserved = ‘111111’b;
        unsigned int(2) chroma_format;
        bit(5) reserved = ‘11111’b;
        unsigned int(3) bit_depth_luma_minus8;
        bit(5) reserved = ‘11111’b;
        unsigned int(3) bit_depth_chroma_minus8;
        unsigned int(8) numOfSequenceParameterSetExt;
        for (i=0; i< numOfSequenceParameterSetExt; i++) {
            unsigned int(16) sequenceParameterSetExtLength;
            bit(8*sequenceParameterSetExtLength) sequenceParameterSetExtNALUnit;
        }
    }
}
*/

int mpeg4_decode_avc_decoder_configuration_record(mpeg4_avc_t *avc, const uint8_t *data, uint32_t bytes)
{
    int i;
    const uint8_t *cur, *end;

    if (bytes < 7)
        return -1;

    cur = data;
    end = data + bytes;

    assert(cur[0] == 1);

    avc->configuration_version = cur[0];
    avc->avc_profile_indication = cur[1];
    avc->profile_compatibility = cur[2];
    avc->avc_level_indication = cur[3];

    // lengthSizeMinusOne: indicates the length in bytes of the NALUnitLength field in an AVC video
    // sample or AVC parameter set sample of the associated stream minus one. For example, a size of
    // one byte is indicated with a value of 0. The value of this field shall be one of 0, 1, or 3
    // corresponding to alength encoded with 1, 2, or 4 bytes, respectively.
    avc->length_size_minus_one = cur[4] & 0x03 + 1;

    cur += 5;

    // Sequence Parameter Sets
    avc->num_of_sequence_parameter_sets = *cur++ & 0x1F;
    for (i = 0; i < avc->num_of_sequence_parameter_sets; i++) {
        if (cur + 2 > end)
            return -1;

        avc->sps[i].bytes = (cur[0] << 8) | cur[1];
        avc->sps[i].data = cur + 2;
        if (avc->sps[i].data + avc->sps[i].bytes > end)
            return -1;

        cur += avc->sps[i].bytes + 2;
    }

    // Picture Parameter Sets
    avc->num_of_picture_parameter_sets = *cur++;
    for (i = 0; i < avc->num_of_picture_parameter_sets; i++) {
        if (cur + 2 > end)
            return -1;

        avc->pps[i].bytes = (cur[0] << 8) | cur[1];
        avc->pps[i].data = cur + 2;
        if (avc->pps[i].data + avc->pps[i].bytes > end)
            return -1;

        cur += avc->pps[i].bytes + 2;
    }

    // Extension
    if (100 == avc->avc_profile_indication || 110 == avc->avc_profile_indication ||
        122 == avc->avc_profile_indication || 144 == avc->avc_profile_indication)
    {
        avc->chroma_format = cur[0] & 0x03;
        avc->bit_depth_luma_minus8 = cur[1] & 0x07;
        avc->bit_depth_chroma_minus8 = cur[2] & 0x07;

        cur += 3;

        // Sequence Parameter Set Extension NAL Unit
        avc->num_of_sequence_parameter_set_ext_length = *cur++;
        for (i = 0; i < avc->num_of_sequence_parameter_set_ext_length; i++) {
            if (cur + 2 > end)
                return -1;

            avc->sps_ext[i].bytes = (cur[0] << 8) | cur[1];
            avc->sps_ext[i].data = cur + 2;
            if (avc->sps_ext[i].data + avc->sps_ext[i].bytes > end)
                return -1;

            cur += avc->sps_ext[i].bytes + 2;
        }
    }

    return (int) (cur - data);
}
