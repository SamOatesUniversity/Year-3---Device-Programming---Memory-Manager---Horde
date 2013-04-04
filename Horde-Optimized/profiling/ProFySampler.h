#ifndef PROFYSAMPLER_HEADER
#define PROFYSAMPLER_HEADER

#include "SDL_thread.h"
#include "../lasagne/lasagne.h"

struct ProcessInfo {
	CLasagne *engine;
};

int SamplingThreadFunction(void *rawPtr);

void OutputToHtml(std::map<std::string, float> &samples);

#endif
