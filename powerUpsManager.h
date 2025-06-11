#pragma once

class PowerUpManager
{
private:
    Dynamic_array<PowerUp> powerUps;
    Texture2D healthTexture;
    Texture2D shieldTexture;
    Texture2D rapidFireTexture;
    Texture2D extraLifeTexture;

    float spawnTimer;
    float spawnInterval;
    float powerupChance;

public:
    PowerUpManager() : spawnTimer(0.0), spawnInterval(10.0), powerupChance(1) {}

    void Initialize(Texture2D health, Texture2D shield, Texture2D rapidFire, Texture2D extraLife) {
        healthTexture = health;
        shieldTexture = shield;
        rapidFireTexture = rapidFire;
        extraLifeTexture = extraLife;
    }
    void Update(float dt, player& playerRef)
    {
        spawnTimer += dt;
        for (int i = 0; i < powerUps.size(); )
        {
            PowerUp& p = powerUps[i];
            p.Update(dt);

            // Check if powerup is collected by player
            if (p.IsActive() && CheckCollisionRecs(p.GetBounds(), playerRef.GetBounds())) {
                p.Apply(playerRef);
                powerUps.delete_at(i);
            }
            // Remove powerups that are off screen
            else if (p.IsOffScreen()) {
                powerUps.delete_at(i);
            }
            else {
                i++;
            }
        }
        // Spawn new powerup if time has passed
        if (spawnTimer >= spawnInterval) {
            spawnTimer = 0.0f;
            TrySpawnPowerUp();
        }
    }

    void Draw() {
        for (int i = 0; i < powerUps.size(); i++) {
            powerUps[i].Draw();
        }
    }

    void TrySpawnPowerUp() 
    {
        if (GetRandomValue(0, 100) < powerupChance * 100) {
            SpawnRandomPowerUp();
        }
    }

    void SpawnPowerUpAtPosition(Vector2 pos) {
        if (GetRandomValue(0, 100) < powerupChance * 100) 
        {
            PowerUp::Type type = static_cast<PowerUp::Type>(GetRandomValue(0, 4));
            Texture2D tex;

            switch (type)
            {
            case PowerUp::HEALTH:
                tex = healthTexture;
                break;
            case PowerUp::SHIELD:
                tex = shieldTexture;
                break;
            case PowerUp::RAPID_FIRE:
                tex = rapidFireTexture;
                break;
            case PowerUp::EXTRA_LIFE:
                tex = extraLifeTexture;
                break;
            default:
                tex = healthTexture;
                break;
            }
            PowerUp newPowerUp(pos, tex, type);
            powerUps.push(newPowerUp);
        }
    }

    void SpawnRandomPowerUp() {
        int screenWidth = GetScreenWidth();
        float randX = GetRandomValue(50, screenWidth - 50);
        Vector2 spawnPos = { randX, -50 };

        PowerUp::Type type = static_cast<PowerUp::Type>(GetRandomValue(0, 4));
        Texture2D tex;

        switch (type) {
        case PowerUp::HEALTH:
            tex = healthTexture;
            break;
        case PowerUp::SHIELD:
            tex = shieldTexture;
            break;
        case PowerUp::RAPID_FIRE:
            tex = rapidFireTexture;
            break;
        case PowerUp::EXTRA_LIFE:
            tex = extraLifeTexture;
            break;
        default:
            tex = healthTexture;
            break;
        }

        PowerUp newPowerUp(spawnPos, tex, type);
        powerUps.push(newPowerUp);
    }

    void Clear() {
        powerUps.clear();
    }

    void SetSpawnInterval(float interval) {
        spawnInterval = interval;
    }

    void SetPowerUpChance(float chance) {
        powerupChance = chance;
    }
};