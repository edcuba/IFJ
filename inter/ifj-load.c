/* ifj-load.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

#include "ifj-inter.h"

/**
 * Function loads file from 1st command line parameter and saves pointer to
 * ifjInter structure.
 *
 * If wrong number of parameters were passed or file is unopenable then
 * function returns error code 99
 *
 * @param argc Number of commandline parameters
 * @param argv Array of parameters
 * @param inter Pointer to ifjInter structure
 * @return Function returns 99 if error occurred, otherwise returns 0
 */
int ifj_load(int argc, char **argv, ifjInter* inter) {

    if (argc != 2) {
        fprintf(stderr, "Error: No input file!\n");
        return 99; //no file set
    }

    inter->inputFile = fopen(argv[1], "r");
    if (inter->inputFile == NULL) {
        fprintf(stderr, "Error: Cannot open input file. \n");
        return 99;
    }

    return 0;
}
