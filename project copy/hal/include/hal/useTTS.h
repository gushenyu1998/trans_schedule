// Header File to use Flite Text to Speech
// Setup and Play the Text to Speech

#include <flite/flite.h>

#ifndef ASSIGNMENT3_USETTS_H
#define ASSIGNMENT3_USETTS_H

#endif //ASSIGNMENT3_USETTS_H

void initialize_flite();
cst_voice *setup_voice();
void synthesize_text_to_file(const char *text, cst_voice *voice, const char *output_file);
