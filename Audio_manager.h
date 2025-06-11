#pragma once
class AudioManager {
private:
    Sound shootSound;
    Sound explosionSound;
    Sound powerupSound;
    Sound enemyHitSound;
    Sound playerHitSound;
    Music backgroundMusic;
    bool musicPlaying;

public:
    AudioManager() : musicPlaying(false) {}

    void Initialize() {
        InitAudioDevice();
        musicPlaying = false;
    }

    void LoadSounds(const char* shootPath, const char* explosionPath, const char* powerupPath,
        const char* enemyHitPath, const char* playerHitPath, const char* musicPath) {
        shootSound = LoadSound(shootPath);
        explosionSound = LoadSound(explosionPath);
        powerupSound = LoadSound(powerupPath);
        enemyHitSound = LoadSound(enemyHitPath);
        playerHitSound = LoadSound(playerHitPath);
        backgroundMusic = LoadMusicStream(musicPath);
    }

    void PlayShootSound() {
        PlaySound(shootSound);
    }

    void PlayExplosionSound() {
        PlaySound(explosionSound);
    }

    void PlayPowerupSound() {
        PlaySound(powerupSound);
    }

    void PlayEnemyHitSound() {
        PlaySound(enemyHitSound);
    }

    void PlayPlayerHitSound() {
        PlaySound(playerHitSound);
    }

    void StartBackgroundMusic() {
        PlayMusicStream(backgroundMusic);
        musicPlaying = true;
    }

    void StopBackgroundMusic() {
        StopMusicStream(backgroundMusic);
        musicPlaying = false;
    }

    void UpdateMusic() 
    {
        if (musicPlaying) {
            UpdateMusicStream(backgroundMusic);
        }
    }


    void SetGameMusicVolume(float volume)
    {
        SetMusicVolume(backgroundMusic, volume); // Call Raylib's function
    }

    void SetGameSoundVolume(float volume) {
        SetSoundVolume(shootSound, volume);
        SetSoundVolume(explosionSound, volume);
        SetSoundVolume(powerupSound, volume);
        SetSoundVolume(enemyHitSound, volume);
        SetSoundVolume(playerHitSound, volume);
    }

    void CleanUp() {
        UnloadSound(shootSound);
        UnloadSound(explosionSound);
        UnloadSound(powerupSound);
        UnloadSound(enemyHitSound);
        UnloadSound(playerHitSound);
        UnloadMusicStream(backgroundMusic);
        CloseAudioDevice();
    }
};
