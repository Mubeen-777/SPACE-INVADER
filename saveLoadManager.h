#pragma once

struct GameSaveData
{
    Vector2 playerPosition;
    int playerHealth;
    int playerScore;
    int playerLives;
    bool playerIsInvulnerable;
    float playerInvulnerabilityTimer;
    bool playerHasShield;
    float playerShieldTimer;

    float gameTimer;
    int waveNumber;
    float difficultyScaling;

    float enemy1SpawnTimer;
    float enemy2SpawnTimer;
    float enemy3SpawnTimer;
    float enemy1SpawnInterval;
    float enemy2SpawnInterval;
    float enemy3SpawnInterval;


    GameSaveData() {
        playerPosition = { 0, 0 };
        playerHealth = 100;
        playerScore = 0;
        playerLives = 3;
        playerIsInvulnerable = false;
        playerInvulnerabilityTimer = 0.0;
        playerHasShield = false;
        playerShieldTimer = 0.0;
        gameTimer = 0.0;
        waveNumber = 1;
        difficultyScaling = 1.0;
        enemy1SpawnTimer = 0.0;
        enemy2SpawnTimer = 0.0;
        enemy3SpawnTimer = 0.0;
        enemy1SpawnInterval = 2.0;
        enemy2SpawnInterval = 10.0;
        enemy3SpawnInterval = 15.0;

    }
};
class SaveLoadManager {
private:
    const char* saveFilePath = "save.bin";

    // Helper functions
    template<typename T>
    void WriteData(ofstream& file, const T& data) {
        file.write(reinterpret_cast<const char*>(&data), sizeof(T));
    }
    //helper functions
    void WriteVector2(ofstream& file, const Vector2& vec) {
        WriteData(file, vec.x);
        WriteData(file, vec.y);
    }
    // Helper functions
    template<typename T>
    void ReadData(ifstream& file, T& data) {
        file.read(reinterpret_cast<char*>(&data), sizeof(T));
    }
    //helper function
    void ReadVector2(ifstream& file, Vector2& vec) {
        ReadData(file, vec.x);
        ReadData(file, vec.y);
    }

public:
    bool SaveGame(player& mainPlayer, GameManager& gameManager,
        EnemySpawner& spawner, float gameTimer) {
        ofstream file(saveFilePath, ios::binary);

        // Save player data
        WriteVector2(file, mainPlayer.GetPosition());
        WriteData(file, mainPlayer.GetHealth());
        WriteData(file, mainPlayer.GetScore());
        WriteData(file, mainPlayer.GetLives());
        WriteData(file, mainPlayer.HasShield());
        WriteData(file, mainPlayer.IsInvulnerable());

        // Save game state data
        WriteData(file, gameTimer);
        WriteData(file, spawner.GetWaveNumber());
        WriteData(file, spawner.GetDifficultyScaling());

        // Save enemy spawner timers
        float timer1, timer2, timer3, interval1, interval2, interval3;
        spawner.GetSpawnTimers(timer1, timer2, timer3);
        spawner.GetSpawnIntervals(interval1, interval2, interval3);
        WriteData(file, timer1);
        WriteData(file, timer2);
        WriteData(file, timer3);
        WriteData(file, interval1);
        WriteData(file, interval2);
        WriteData(file, interval3);


        file.close();
        return true;
    }

    bool LoadGame(player& mainPlayer, GameManager& gameManager, EnemySpawner& spawner, float gameTimer)
    {
        ifstream file(saveFilePath, ios::binary);
        // Load player data
        Vector2 playerPos;
        ReadVector2(file, playerPos);
        mainPlayer.SetPosition(playerPos);

        int health, score, lives;
        ReadData(file, health);
        ReadData(file, score);
        ReadData(file, lives);
        mainPlayer.SetHealth(health);
        mainPlayer.SetScore(score);
        mainPlayer.SetLives(lives);

        bool hasShield, isInvulnerable;
        ReadData(file, hasShield);
        ReadData(file, isInvulnerable);
        mainPlayer.SetShieldState(hasShield, 0.0);
        mainPlayer.SetInvulnerabilityState(isInvulnerable, 0.0);
        // Load game state data
        ReadData(file, gameTimer);

        int waveNumber;
        float difficultyScaling;
        ReadData(file, waveNumber);
        ReadData(file, difficultyScaling);
        spawner.SetWaveNumber(waveNumber);
        spawner.SetDifficultyScaling(difficultyScaling);
        // Load enemy spawner timers
        float timer1, timer2, timer3, interval1, interval2, interval3;
        ReadData(file, timer1);
        ReadData(file, timer2);
        ReadData(file, timer3);
        ReadData(file, interval1);
        ReadData(file, interval2);
        ReadData(file, interval3);
        spawner.SetSpawnTimers(timer1, timer2, timer3);
        spawner.SetSpawnIntervals(interval1, interval2, interval3);

        file.close();
        return true;
    }

    bool SaveFileExists() {
        ifstream file(saveFilePath);
        return file.good();
    }
};