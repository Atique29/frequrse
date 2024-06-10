#ifndef SOUND_GEN_H
#define SOUND_GEN_H

// generate a single freq tone
void gen_tone(int freq, int dur, int gap, int repeat);

// generate a sine wave sweep
void gen_sweep(int freq_start, int freq_stop, int sweep_t, int gap, int repeat);

#endif // SOUND_GEN_H
