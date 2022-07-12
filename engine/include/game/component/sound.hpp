#pragma once

#include <memory>
#include <filesystem>
#include "miniaudio.h"

#include "component_generator.hpp"


namespace Game
{
    KK_COMPONENT(Sound)

    std::string soundPath = "none";
    bool loop = false;
    bool positionAuto = true; // Set the position to the owner's root entity each frame on update.

    /**
     * Set sound file path
     */
    void SetSoundPath(std::filesystem::path i_path);

    /**
     * Set sound position in scene
     */
    void SetSoundPosition(const Vector3& i_pos);

    /**
     * Set main listener in scene
     */
    static void SetMainListener(const Vector3& i_pos, const Vector3& i_forward);

    void OnUpdate() override;

    /**
     * Play a sound
     */
    void Play();

    /**
     * Stop a sound
     */
    void Stop();

    /**
     * Pause a sound
     */
    void Pause();

    /**
     * Set the volume of the sound playing
     */
    void SetVolume(const float i_volume);

    /**
     * Apply the current volume field value. Useful for editor inputs.
     */
    void ApplyVolume();

    /**
     * @brief Unload miniaudio sound resource if 'loaded' is true
     */
    void UnloadSound();

private:
    ma_sound sound;
    bool loaded = false;

    float volume = 1.f;

    /**
     * @brief Load miniaudio sound resource with current 'soundPath' value.
     */
    void LoadSound();

    KK_COMPONENT_END
}

void DropOnSoundComponent(unsigned char* io_component, void* i_dropData);
void OnSoundPathUpdate(unsigned char* io_component);
void OnPlayClicked(unsigned char* io_component);
void OnPauseClicked(unsigned char* io_component);
void OnStopClicked(unsigned char* io_component);
void OnVolumeUpdate(unsigned char* io_component);