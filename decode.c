#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "common.h"
/*
Name : Rakshitha B R
Batch ID :250017E    
Date : 05/10/2025
Description : This file contains the implementation of decoding functions for steganography.
*/

Status do_decoding(DecodeInfo *decInfo)
{
    printf("INFO: Decoding selected\n");
    printf("INFO: Decoding arguments validated\n");
    printf("INFO: Decoding Started\n");

    // Open stego image
    if (open_stego_file(decInfo) != e_success)
    {
        printf("ERROR: Failed to open stego image\n");
        return e_failure;
    }

    // Skip BMP header
    fseek(decInfo->fptr_stego_image, BMP_HEADER_SIZE, SEEK_SET);

    // Decode magic string
    if (decode_magic_string(MAGIC_STRING, decInfo) != e_success)
    {
        printf("ERROR: Magic string mismatch\n");
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }
    printf("INFO: Magic string verified\n");

    // Decode secret file extension size
    int extn_size;
    if (decode_secret_file_extn_size(&extn_size, decInfo) != e_success)
    {
        printf("ERROR: Failed to decode secret file extension size\n");
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }
    printf("INFO: Extension size: %d\n", extn_size);

    // Decode secret file extension
    if (decode_secret_file_extn(decInfo, extn_size) != e_success)
    {
        printf("ERROR: Failed to decode secret file extension\n");
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }
    printf("INFO: Decoded extension: .%s\n", decInfo->extn_secret_file);

    // Decode secret file size
    long file_size;
    if (decode_secret_file_size(&file_size, decInfo) != e_success)
    {
        printf("ERROR: Failed to decode secret file size\n");
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }
    decInfo->size_secret_file = file_size;
    printf("INFO: File size: %ld bytes\n", file_size);

    // Create output file
    if (create_output_file(decInfo) != e_success)
    {
        printf("ERROR: Failed to create output file\n");
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }
    printf("INFO: Secret Output file: %s\n", decInfo->output_fname);

    // Decode secret file data
    if (decode_secret_file_data(decInfo) != e_success)
    {
        printf("ERROR: Failed to decode secret file data\n");
        fclose(decInfo->fptr_stego_image);
        fclose(decInfo->fptr_output);
        return e_failure;
    }

    // Cleanup
    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_output);

    printf("INFO: Decoding Completed Successfully\n");
    printf("INFO: Decoding completed successfully\n");
    return e_success;
}

Status open_stego_file(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }
    return e_success;
}

Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)// Function to decode the magic string from the stego image 
{
    int magic_len = strlen(magic_string);
    char decoded_magic[magic_len + 1];
    decoded_magic[magic_len] = '\0';

    if (decode_data_from_image(decoded_magic, magic_len, decInfo->fptr_stego_image) != e_success)
    {
        return e_failure;
    }

    if (strncmp(decoded_magic, magic_string, magic_len) != 0)
    {
        return e_failure;
    }

    return e_success;
}

Status decode_secret_file_extn_size(int *extn_size, DecodeInfo *decInfo)// Function to decode the size of the secret file extension from the stego image
{
    char buffer[32];
    if (fread(buffer, 1, 32, decInfo->fptr_stego_image) != 32)
    {
        return e_failure;
    }

    *extn_size = 0;
    for (int i = 0; i < 32; i++)
    {
        *extn_size = (*extn_size << 1) | (buffer[i] & 1);
    }
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo, int extn_size)// Function to decode the secret file extension from the stego image
{
    if (extn_size > MAX_FILE_SUFFIX)
    {
        return e_failure;
    }

    if (decode_data_from_image(decInfo->extn_secret_file, extn_size, decInfo->fptr_stego_image) != e_success)
    {
        return e_failure;
    }
    decInfo->extn_secret_file[extn_size] = '\0';

    return e_success;
}

Status decode_secret_file_size(long *file_size, DecodeInfo *decInfo)// Function to decode the size of the secret file from the stego image
{
    char buffer[32];
    if (fread(buffer, 1, 32, decInfo->fptr_stego_image) != 32)
    {
        return e_failure;
    }

    *file_size = 0;
    for (int i = 0; i < 32; i++)
    {
        *file_size = (*file_size << 1) | (buffer[i] & 1);
    }
    return e_success;
}

Status create_output_file(DecodeInfo *decInfo)// Function to create the output file for decoded data
{
    char output_path[256];
    snprintf(output_path, sizeof(output_path), "decoded.%s", decInfo->extn_secret_file);
    decInfo->output_fname = strdup(output_path);

    decInfo->fptr_output = fopen(output_path, "wb");
    if (decInfo->fptr_output == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)// Function to decode the secret file data from the stego image
{
    char byte;
    for (long i = 0; i < decInfo->size_secret_file; i++)
    {
        if (decode_data_from_image(&byte, 1, decInfo->fptr_stego_image) != e_success)
        {
            return e_failure;
        }
        fwrite(&byte, 1, 1, decInfo->fptr_output);
    }
    return e_success;
}

Status decode_data_from_image(char *data, int size, FILE *fptr_stego_image)// Function to decode data from the stego image
{
    char image_buffer[8];
    for (int i = 0; i < size; i++)
    {
        if (fread(image_buffer, 1, 8, fptr_stego_image) != 8)
        {
            return e_failure;
        }
        data[i] = 0;
        for (int j = 0; j < 8; j++)
        {
            data[i] = (data[i] << 1) | (image_buffer[j] & 1);
        }
    }
    return e_success;
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)// Function to read and validate command line arguments for decoding
{
    if (strcmp(argv[1], "-d") != 0)
    {
        printf("ERROR: Use -d for decoding\n");
        return e_failure;
    }

    decInfo->stego_image_fname = argv[2];

    // Verify stego image is BMP
    if (strstr(decInfo->stego_image_fname, ".bmp") == NULL)
    {
        printf("ERROR: Stego file must be a BMP image\n");
        return e_failure;
    }

    return e_success;
}