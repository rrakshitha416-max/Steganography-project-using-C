/*Name:Rakshitha B R
Batch ID:250017E
Date:05/10/2025
Description:LSB image Steganography*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encode.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

 Status do_encoding(EncodeInfo *encInfo)// function to perform encoding
{
    printf("INFO: Encoding Started\n");

    // Open all files
    if (open_files(encInfo) != e_success) 
    {
        printf("ERROR: Failed to open files\n");
        return e_failure;
    }
    printf("INFO: Files opened successfully\n");

    // Check capacity
    if (check_capacity(encInfo) != e_success)
    {
        printf("ERROR: Insufficient image capacity\n");
        return e_failure;
    }
    printf("INFO: Capacity check passed\n");

    // Copy BMP header
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
    {
        printf("ERROR: Failed to copy BMP header\n");
        return e_failure;
    }
    printf("INFO: BMP header copied\n");

    // Encode magic string
    printf("INFO: Encoding magic string '%s'\n", MAGIC_STRING);
    if (encode_magic_string(MAGIC_STRING, encInfo) != e_success)
    {
        printf("ERROR: Failed to encode magic string\n");
        return e_failure;
    }

    // Encode extension size
    int extn_size = strlen(encInfo->extn_secret_file);
    printf("INFO: Encoding extension size: %d\n", extn_size);
    if (encode_secret_file_extn_size(extn_size, encInfo) != e_success)
    {
        printf("ERROR: Failed to encode extension size\n");
        return e_failure;
    }

    // Encode extension
    printf("INFO: Encoding extension: %s\n", encInfo->extn_secret_file);
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) != e_success)
    {
        printf("ERROR: Failed to encode extension\n");
        return e_failure;
    }

    // Encode file size
    printf("INFO: Encoding file size: %ld bytes\n", encInfo->size_secret_file);
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) != e_success) {
        printf("ERROR: Failed to encode file size\n");
        return e_failure;
    }

    // Encode file data
    printf("INFO: Encoding secret data\n");
    if (encode_secret_file_data(encInfo) != e_success) 
    {
        printf("ERROR: Failed to encode secret data\n");
        return e_failure;
    }

    // Copy remaining image data
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success) 
    {
        printf("ERROR: Failed to copy remaining image data\n");
        return e_failure;
    }

    // Cleanup
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);

    printf("INFO: Encoding Completed Successfully\n");
    return e_success;
}
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)// Function to read and validate command line arguments for encoding
{
    if (strcmp(argv[1], "-e") != 0) //
    {
        printf("ERROR: Use -e for encoding\n");
        return e_failure;
    }

    encInfo->src_image_fname = argv[2];//
    encInfo->secret_fname = argv[3];
    encInfo->stego_image_fname = argv[4];

    // Extract extension from secret filename
    char *dot = strrchr(encInfo->secret_fname, '.');// 
    if (dot && strlen(dot) > 1)
    {
        strncpy(encInfo->extn_secret_file, dot+1, MAX_FILE_SUFFIX-1);
        encInfo->extn_secret_file[MAX_FILE_SUFFIX-1] = '\0';//
    } 
    else 
    {
        strcpy(encInfo->extn_secret_file, "txt");
    }

    printf("INFO: Using file extension: .%s\n", encInfo->extn_secret_file);
    return e_success;
}

OperationType check_operation_type(char *argv[])// Function to check the operation type based on command line arguments
{
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    } 
    else if (strcmp(argv[1], "-d") == 0) 
    {
        return e_decode;
    } 
    else 
    {
        printf("ERROR: Invalid operation type. Use -e for encoding or -d for decoding.\n");
        return e_unsupported;
    }
    
}
uint get_image_size_for_bmp(FILE *fptr_image)// Function to get the size of the image based on BMP header
{
    uint width, height;
    fseek(fptr_image, 18, SEEK_SET);//
    fread(&width, sizeof(int), 1, fptr_image);
    fread(&height, sizeof(int), 1, fptr_image);
    return width * height * 3;
}
uint get_file_size(FILE *fptr)// Function to get the size of a file
{
    fseek(fptr, 0, SEEK_END);
    long size = ftell(fptr);
    rewind(fptr);
    return size;
}

Status open_files(EncodeInfo *encInfo)// Funaction to open all files required for encoding
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");// Open source image file in binary read mode
    if (!encInfo->fptr_src_image)
    {
        perror("fopen");
        return e_failure;
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");// Open secret file in binary read mode
    if (!encInfo->fptr_secret)
    {
        perror("fopen");
        return e_failure;
    }

    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");// Open stego image file in binary write mode
    if (!encInfo->fptr_stego_image) 
    {
        perror("fopen");
        return e_failure;
    }

    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)// Function to check if the source image has enough capacity to hold the secret file
{
    fseek(encInfo->fptr_secret, 0, SEEK_END);// Move to the end of the secret file
    encInfo->size_secret_file = ftell(encInfo->fptr_secret);// Get the size of the secret file
    rewind(encInfo->fptr_secret);

    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    int required = 54 + (strlen(MAGIC_STRING) + strlen(encInfo->extn_secret_file) + 32 + 32 + encInfo->size_secret_file) * 8;// Calculate required capacity
    
    if (encInfo->image_capacity < required)
    {
        fprintf(stderr, "ERROR: Need %d bytes, image has %d\n", required, encInfo->image_capacity);
        return e_failure;
    }

    return e_success;
}

Status copy_bmp_header(FILE *fptr_src, FILE *fptr_dest)// Function to copy BMP header from source image to stego image
{
    char header[54];
    rewind(fptr_src);
    if (fread(header, 1, 54, fptr_src) != 54) return e_failure;
    if (fwrite(header, 1, 54, fptr_dest) != 54) return e_failure;
    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)// Function to encode the magic string into the stego image
{
    return encode_data_to_image((char*)magic_string, strlen(magic_string),encInfo->fptr_src_image, encInfo->fptr_stego_image);
}

Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo)// Function to encode the size of the secret file extension into the stego image
{
    char buffer[32];
    if (fread(buffer, 1, 32, encInfo->fptr_src_image) != 32)
    {
        fprintf(stderr, "ERROR: Failed to read from source image\n");
        return e_failure;
    }
    encode_int_to_lsb(extn_size, buffer);
    if (fwrite(buffer, 1, 32, encInfo->fptr_stego_image) != 32) 
    {
        fprintf(stderr, "ERROR: Failed to write to stego image\n");
        return e_failure;
    }
    printf("INFO: Encoded extension size: %d\n", extn_size);
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)// Function to encode the secret file extension into the stego image
{
    return encode_data_to_image((char*)file_extn, strlen(file_extn),encInfo->fptr_src_image, encInfo->fptr_stego_image);
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)// Function to encode the size of the secret file into the stego image
{
    char buffer[32];
    if (fread(buffer, 1, 32, encInfo->fptr_src_image) != 32) return e_failure;
    encode_int_to_lsb((int)file_size, buffer);
    if (fwrite(buffer, 1, 32, encInfo->fptr_stego_image) != 32) return e_failure;
    return e_success;
}

Status encode_int_to_lsb(int data, char *img_buffer)// Function to encode an integer into the least significant bits of the image buffer
{
    for (int i = 0; i < 32; i++) {
        img_buffer[i] = (img_buffer[i] & 0xFE) | ((data >> (31-i)) & 1);
    }
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)// Function to encode the secret file data into the stego image
{
    char ch;
    for (long i = 0; i < encInfo->size_secret_file; i++)
    {
        if (fread(&ch, 1, 1, encInfo->fptr_secret) != 1) return e_failure;
        if (encode_data_to_image(&ch, 1, encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
            return e_failure;
    }
    return e_success;
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src, FILE *fptr_stego)// Function to encode data into the stego image
{
    char image_buffer[8];
    for (int i = 0; i < size; i++) 
    {
        if (fread(image_buffer, 1, 8, fptr_src) != 8) return e_failure;
        encode_byte_to_lsb(data[i], image_buffer);
        if (fwrite(image_buffer, 1, 8, fptr_stego) != 8) return e_failure;
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)// 
{
    for (int i = 0; i < 8; i++) {
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> (7-i)) & 1);
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)// Function to copy remaining image data from source to stego image
{
    char ch;
    while (fread(&ch, 1, 1, fptr_src) == 1) 
    {
        if (fwrite(&ch, 1, 1, fptr_dest) != 1) return e_failure;
    }
    return e_success;
}

