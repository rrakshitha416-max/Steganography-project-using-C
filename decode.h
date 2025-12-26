/*Name:Rakshitha B R
Date:05/10/2025
Batch ID:250017E
Description:LSB image Steganography*/
#ifndef DECODE_H
#define DECODE_H

#include "types.h"
#include "common.h"

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;

    /* Decoded File Info */
    char *output_fname;
    FILE *fptr_output;
} DecodeInfo;

/* Decoding function prototypes */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);
Status do_decoding(DecodeInfo *decInfo);
Status open_stego_file(DecodeInfo *decInfo);
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);
Status decode_secret_file_extn_size(int *extn_size, DecodeInfo *decInfo);
Status decode_secret_file_extn(DecodeInfo *decInfo, int extn_size);
Status decode_int_from_lsb(int *data, char *img_buffer);
Status decode_secret_file_size(long *file_size, DecodeInfo *decInfo);
Status decode_secret_file_data(DecodeInfo *decInfo);
Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image);
Status decode_byte_from_lsb(char *data, char *image_buffer);
Status create_output_file(DecodeInfo *decInfo);

#endif