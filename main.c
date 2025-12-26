/*Name:Rakshitha B R
Batch ID:250017E
Date:05/10/2025
Description: LSB image Steganography*/
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "common.h"
#include "encode.h"
#include "decode.h"  // Make sure this is included


void print_usage()
{
    printf("Usage:\n");
    printf("Encoding: ./stegano -e <source_image.bmp> <secret_file> <stego_image.bmp>\n");
    printf("Decoding: ./stegano -d <stego_image.bmp>\n");
}
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        print_usage();
        return 1;
    }

    OperationType operation = check_operation_type(argv);

    if (operation == e_encode)
    {
        printf("INFO: Encoding selected\n");
        EncodeInfo encInfo;

        if (argc != 5)
        {
            printf("ERROR: Encoding requires 4 arguments\n");
            print_usage();
            return 1;
        }

        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            printf("INFO: Encoding arguments validated\n");

            if (do_encoding(&encInfo) == e_success)
            {
                printf("INFO: Encoding completed successfully\n");
                return 0;
            }
            else
            {
                printf("ERROR: Encoding failed\n");
                return 1;
            }
        }
        else
        {
            printf("ERROR: Validation failed for encoding arguments\n");
            return 1;
        }
    }
    else if (operation == e_decode)
    {
        printf("INFO: Decoding selected\n");
        DecodeInfo decInfo;

        if (argc != 3)
        {
            printf("ERROR: Decoding requires 2 arguments\n");
            print_usage();
            return 1;
        }

        if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            printf("INFO: Decoding arguments validated\n");

            if (do_decoding(&decInfo) == e_success)
            {
                printf("INFO: Decoding completed successfully\n");
                return 0;
            }
            else
            {
                printf("ERROR: Decoding failed\n");
                return 1;
            }
        }
        else
        {
            printf("ERROR: Validation failed for decoding arguments\n");
            return 1;
        }
    }
    else
    {
        printf("ERROR: Unsupported operation\n");
        print_usage();
        return 1;
    }

    return 0;
}

