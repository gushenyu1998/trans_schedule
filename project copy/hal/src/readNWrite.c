// Source File for readNWrite.c
// Read and write to System Files
// Execute Linux Commands

#include "hal/readNWrite.h"

/**
 * Refer from Assignment guide, write value to file
 * Input:
 *  char *fileanme：path of BeagleBone file
 *  char *value: value to write
*/
void writeToFile(const char* fileName, const char* value)
{
	FILE *pFile = fopen(fileName, "w");
	fprintf(pFile, "%s", value);	
	fclose(pFile);
}

/**
 * Refer from Assignment guide, read value from file
 * Input:
 *  char *fileanme：path of BeagleBone file
 *  char *buff: the buffer store the reading result;
 *  uint *maxLen: max length of the buffer to read value of file
 * Return:
 *  int Bytes readed in length
*/
int readLineFromFile(char* fileName, char* buff, unsigned int maxLength)
{
	FILE *file = fopen(fileName, "r");
	int bytes_read = getline(&buff, &maxLength, file);
	fclose(file);
	return bytes_read;
}

/**
 * run the command of Linux, code is comming from the assignment guide
 */
void runCommand(char *command)
{
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe))
    {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
        // printf("--> %s", buffer); // Uncomment for debugging
    }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0)
    {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}
