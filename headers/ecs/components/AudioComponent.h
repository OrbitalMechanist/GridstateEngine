#pragma once
#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H
#include "audio/SoundSource.h"
// Audio Source
struct AudioComponent {
    SoundSource* sourceA = new SoundSource(1.f, 1.f, { 0.0f,0.0f,0.0f }, { 0,0,0 }, false, true);
    SoundSource* sourceB = new SoundSource(1.f, 1.f, { 0.0f,0.0f,0.0f }, { 0,0,0 }, false, true);
};

#endif 