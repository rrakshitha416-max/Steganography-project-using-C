#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types
#include "common.h"
/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */



typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} EncodeInfo;


/* Encoding function prototype */

OperationType check_operation_type(char *argv[]);
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);
Status do_encoding(EncodeInfo *encInfo);
Status open_files(EncodeInfo *encInfo);
Status check_capacity(EncodeInfo *encInfo);
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);
Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo);
Status encode_int_to_lsb(int data, char *img_buffer);
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);
Status encode_secret_file_data(EncodeInfo *encInfo);
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);
Status encode_byte_to_lsb(char data, char *image_buffer);
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif
