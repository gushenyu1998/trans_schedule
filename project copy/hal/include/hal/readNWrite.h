// Header File for readNWrite.c
// Read and write to System Files
// Execute Linux Commands

#ifndef _READNWRITE_H_
#define _READNWRITE_H_
#include <stdio.h>
#include <stdlib.h>
#include <errno.h> // Errors
#include <string.h>

/**
 * Refer from Assignment guide, write value to file
 * Input:
 *  char *fileanme：path of BeagleBone file
 *  char *value: value to write
*/
void writeToFile(const char* fileName, const char* value);
/**
 * Refer from Assignment guide, read value from file
 * Input:
 *  char *fileanme：path of BeagleBone file
 *  uint *maxLen: max length of the buffer to read value of file
 *  Return:
 *  int Bytes readed
*/
int readLineFromFile(char* fileName, char* buff, unsigned int maxLength);

/**
 * run the command of Linux, code is comming from the assignment guide
 */
void runCommand(char *command);

#endif