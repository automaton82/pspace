// Global sound object for Subspace game - simple stub
// For interim play without complex audio dependencies

// Simple audio interface stub
class SimpleAudioInterface {
public:
    void playSound(int soundId) { }
    void stopSound(int soundId) { }
    void setVolume(float volume) { }
};

// Global sound object for Subspace game
SimpleAudioInterface subspaceSounds;
