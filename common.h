/*Name:Rakshitha B R
Batch ID:250017E
Date:05/10/2025
Description:LSB image Steganography*/


#ifndef COMMON_H
#define COMMON_H

#include "types.h"

/* Magic string to identify whether stegged or not */
#define MAGIC_STRING "#*"

/* Buffer sizes */
#define MAX_SECRET_BUF_SIZE 1//  1 byte for secret data
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)// 8 bits per pixel
#define MAX_FILE_SUFFIX 4// Maximum file extension length
#define BMP_HEADER_SIZE 54// BMP header size in bytes

/* Function declarations */
uint get_image_size_for_bmp(FILE *fptr_image);//
uint get_file_size(FILE *fptr);

#endif