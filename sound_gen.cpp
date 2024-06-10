#include "sound_gen.h"
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>

//the std out of play command through system() messed up the screen so
//added > /dev/null 2>&1 to put the std output and std err of sox to black hole null
// but still sth messed up user input for values, showed invalid input right after
// enter was pressed.
// running the command in the bg (& at the end) solved this issue (why?)
// due to the '&' at the end, need to wait ( duration + gap ) before repeating

void gen_tone(int freq, int dur, int gap, int repeat){

    for (int i = 0; i < repeat; i++) {

        std::string command = "play -n synth " + std::to_string(dur) + " sine " + std::to_string(freq) + " > /dev/null 2>&1 &";

        system(command.c_str());
        std::this_thread::sleep_for(std::chrono::seconds(dur+gap));

    }

}

void gen_sweep(int freq_start, int freq_stop, int sweep_t, int gap, int repeat){

    /* play -V -r 44100 -n synth 30 sin 20+20000 */
    for (int i = 0; i < repeat; i++) {

        std::string command = "play -n synth " + std::to_string(sweep_t) + " sine " + std::to_string(freq_start) + "+" + std::to_string(freq_stop) + " > /dev/null 2>&1 &" ;

        system(command.c_str());
        std::this_thread::sleep_for(std::chrono::seconds(sweep_t+gap));

    }

}
