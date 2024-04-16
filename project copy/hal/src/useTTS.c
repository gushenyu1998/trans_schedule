// Header File to use Flite Text to Speech
// Setup and Play the Text to Speech

#include "hal/useTTS.h"
#include <flite/flite.h>
#include <stdlib.h>

// Initialize Flite and register voices
void initialize_flite() {
    flite_init();
}

// Setup voice features
cst_voice *setup_voice() {
    cst_voice *voice = register_cmu_us_kal(NULL); // Use a different voice as needed
    cst_features *features = new_features();
    flite_feat_set_float(features, "duration_stretch", 1.5);
    flite_feat_set_float(features, "int_f0_target_mean", 150); // Corrected to a more typical pitch
    flite_feat_set_float(features, "int_f0_target_stddev", 9);
    return voice;
}

void synthesize_text_to_file(const char *text, cst_voice *voice, const char *output_file) {
    flite_text_to_speech(text, voice, output_file);
}