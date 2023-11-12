#ifndef SHUTTLE_SOUND_H
#define SHUTTLE_SOUND_H

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <iostream>
#include <filesystem>
#include <vector>

using namespace std;

// additional flags of Sound
const int IS_LOOPING = 0x01;  // 00000001
const int INIT_ONLY = 0x02;   // 00000010

class Sound {
public:
    ma_decoder decoder;
    ma_device device;
    bool looping = false;
    bool initOnly = false;
    bool forceStop = false;
    string audioFilename;
    int flags;
    ma_uint64 totalFramesPlayed;  // New member to track frames played for this sound.

    Sound() {};
    Sound(string name, int initflags = 0) : audioFilename(name) {
        if (initflags != 0) {
            flags = initflags;
            if (flags & IS_LOOPING) { looping = true; }
            if (flags & INIT_ONLY) { initOnly = true; }
        }
        everySound.push_back(this);
    }

    // void stopLoop() { // moved down for consistency
    //     forceStop = true;
    // }

    static vector<Sound*> everySound;
};

vector<Sound*> Sound::everySound;

extern string scriptDir;
ma_result result;

void uninitMiniAudio(Sound* pSound) {
    if (pSound == nullptr) {
        return;
    }
    ma_device_uninit(&pSound->device);
    ma_decoder_uninit(&pSound->decoder);
}

void uninitMiniAudio(Sound& sound) {
    uninitMiniAudio(&sound); // Call the pointer version of uninitMiniAudio.
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    Sound* pSound = (Sound*)pDevice->pUserData;
    if (pSound == nullptr) {
        return;
    }

    ma_decoder* pDecoder = &pSound->decoder;
    if (pDecoder == nullptr) {
        return;
    }

    ma_uint64 audioLengthInFrames;
    ma_decoder_get_length_in_pcm_frames(pDecoder, &audioLengthInFrames);

    if (pSound->initOnly == true) {
        pSound->initOnly = false;
        frameCount = 0;
    }
    else {
        while (frameCount > 0) {
            ma_uint64 framesRemaining = audioLengthInFrames - pSound->totalFramesPlayed;
            if (pSound->forceStop) {
                framesRemaining = 0;
                break;
            }
            if (pSound->initOnly) {
                pSound->initOnly = false;
                break;
            }
            if (framesRemaining == 0) {
                if (pSound->looping) {
                    ma_decoder_seek_to_pcm_frame(pDecoder, 0);
                    pSound->totalFramesPlayed = 0;
                }
                else {
                    break;
                }
            }

            ma_uint64 framesToRead = (frameCount < framesRemaining) ? frameCount : framesRemaining;
            ma_decoder_read_pcm_frames(pDecoder, pOutput, framesToRead, NULL);
            pSound->totalFramesPlayed += framesToRead;

            frameCount -= (ma_uint32)framesToRead;
        }
        // if (playDone) { uninitMiniAudio(pSound); }
    }

}

int initConfigMiniAudio(string filename, Sound& sound, bool isStartOnly = false) {
    string audioFilePath = scriptDir + "\\" + filename;  // Construct the relative path

    if (isStartOnly == true) {
        result = ma_device_start(&sound.device);
        if (result != MA_SUCCESS) {
            cerr << "Failed to start the audio device for " << audioFilePath << endl;
            return -1;
        }
        else {
            return 0;
        }
    }

    result = ma_decoder_init_file(audioFilePath.c_str(), nullptr, &sound.decoder);
    if (result != MA_SUCCESS) {
        cerr << "Failed to initialize the decoder for " << audioFilePath << endl;
        return -1;
    }

    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);

    deviceConfig.playback.format = sound.decoder.outputFormat;
    deviceConfig.playback.channels = sound.decoder.outputChannels;
    deviceConfig.sampleRate = sound.decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &sound.decoder;

    result = ma_device_init(nullptr, &deviceConfig, &sound.device);
    if (result != MA_SUCCESS) {
        cerr << "Failed to initialize the audio device for " << audioFilePath << endl;
        return -1;
    }

    if (sound.initOnly == false) {
        result = ma_device_start(&sound.device);
        if (result != MA_SUCCESS) {
            cerr << "Failed to start the audio device for " << audioFilePath << endl;
            return -1;
        }
    }

    return 0;
}

void initializeEveryAudio() {
    for (Sound* sound : Sound::everySound) {
        initConfigMiniAudio(sound->audioFilename, *sound);
    }
}

void uninitEveryAudio() {
    for (Sound* sound : Sound::everySound) {
        uninitMiniAudio(sound);
    }
}

void playAudioOnClick(Sound& sound) {
    if (sound.initOnly == true) {
        sound.initOnly = false;
        initConfigMiniAudio(sound.audioFilename, sound, true);
    }
    else {
        sound.totalFramesPlayed = 0;
        ma_decoder_seek_to_pcm_frame(&sound.decoder, 0);
    }
}

void stopLoop(Sound& sound) {
    sound.forceStop = true;
}

void restoreLoop(Sound& sound, string resuming = "") {
    sound.forceStop = false;
    if (resuming == "resume") {
        ma_decoder_seek_to_pcm_frame(&sound.decoder, sound.totalFramesPlayed);
    }
    else { // start over
        sound.totalFramesPlayed = 0;
        ma_decoder_seek_to_pcm_frame(&sound.decoder, 0);
    }
}

void pauseLoop(Sound& sound) {
    sound.forceStop = true;
}

#endif
