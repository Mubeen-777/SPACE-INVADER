#include<iostream>
using namespace std;

#include "raylib.h"

#include "MyString.h"
#include "Dynamic_Array.h"

#include "game_object.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "power_ups.h"
#include "powerUpsManager.h"
#include "Audio_manager.h"
#include "background.h"
#include "enemySpawner.h"
#include "game_manager.h"


int main() {
    // Initialize window
    const int screenWidth = 1920;
    const int screenHeight = 1020;
    InitWindow(screenWidth, screenHeight, "Space Shooter");
    SetTargetFPS(60);

    // Load textures
    Texture2D playerTexture = LoadTexture("player.png");
    Texture2D enemy1 = LoadTexture("enemy1.png");
    Texture2D enemy2 = LoadTexture("enemy2.png");
    Texture2D enemy3 = LoadTexture("enemy3.png");
    Texture2D bulletTexture = LoadTexture("bullet.png");
    Texture2D enemyBulletTexture = LoadTexture("enemy_bullet.png");
    Texture2D explosionTexture = LoadTexture("explosion.png");
    Texture2D backgroundTexture = LoadTexture("background.png");

    // Load power-up textures
    Texture2D healthPowerupTexture = LoadTexture("health_powerup.png");
    Texture2D shieldPowerupTexture = LoadTexture("shield_powerup.png");
    Texture2D rapidFirePowerupTexture = LoadTexture("rapid_fire_powerup.png");
    Texture2D extraLifePowerupTexture = LoadTexture("extra_life_powerup.png");

    // Create player
    Vector2 playerStartPos = { (float)screenWidth / 2 - playerTexture.width / 2,(float)screenHeight - playerTexture.height - 20 };
    player mainPlayer(playerStartPos, playerTexture);

    // Create game systems
    GameManager gameManager(&mainPlayer);

    gameManager.SetExplosionTexture(explosionTexture);
    gameManager.SetEnemyBulletTexture(enemyBulletTexture);

    EnemySpawner enemySpawner(enemy1, enemy2, enemy3, 10, 0, screenWidth);

    PowerUpManager powerupManager;
    powerupManager.Initialize(healthPowerupTexture, shieldPowerupTexture, rapidFirePowerupTexture, extraLifePowerupTexture);

    AudioManager audioManager;
    audioManager.Initialize();
    audioManager.LoadSounds(
        "resources/shoot.wav",
        "resources/explosion.wav",
        "resources/powerup.wav",
        "resources/enemy_hit.wav",
        "resources/player_hit.wav",
        "resources/music.mp3"
    );

    Background background(backgroundTexture, 50.0);

    // Start background music
    audioManager.StartBackgroundMusic();
    audioManager.SetGameMusicVolume(0.3);
    audioManager.SetGameSoundVolume(0.5);


    // Game loop
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Update music
        audioManager.UpdateMusic();

        // Process input
        if (gameManager.GetGameState() == GameManager::MAIN_MENU)
        {
            if (IsKeyPressed(KEY_ENTER)) {
                gameManager.StartGame();
            }
        }
        else if (gameManager.GetGameState() == GameManager::PLAYING) {
            if (IsKeyPressed(KEY_P)) {
                gameManager.SetGameState(GameManager::PAUSED);
            }

            // Player shooting
            if (IsKeyPressed(KEY_SPACE) && mainPlayer.CanShoot())
            {
                Vector2 bulletPos = mainPlayer.GetPosition();
                bulletPos.x += mainPlayer.GetTexture().width / 2 - bulletTexture.width / 2;
                Vector2 velocity = { 0, -400 }; // Up direction
                bullet newBullet(bulletPos, bulletTexture, velocity, "player");
                gameManager.AddPlayerBullet(newBullet);
                mainPlayer.ResetShootTimer();
                audioManager.PlayShootSound();
            }
            if (mainPlayer.GetScore() >= 2000 && enemySpawner.GetWaveNumber() == 1) {
                enemySpawner.NextWave();
            }
            else if (mainPlayer.GetScore() >= 4000 && enemySpawner.GetWaveNumber() == 2) {
                enemySpawner.NextWave();
            }
            else if (mainPlayer.GetScore() >= 8000 && enemySpawner.GetWaveNumber() == 3) {
                enemySpawner.NextWave();
            }
        }
        else if (gameManager.GetGameState() == GameManager::PAUSED)
        {
            if (IsKeyPressed(KEY_P))
            {
                gameManager.SetGameState(GameManager::PLAYING);
            }
        }
        else if (gameManager.GetGameState() == GameManager::GAME_OVER || gameManager.GetGameState() == GameManager::VICTORY)
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                gameManager.StartGame();
                enemySpawner.ResetSpawner();
                powerupManager.Clear();
            }
        }
        // Update game logic
        if (gameManager.GetGameState() == GameManager::PLAYING) {
            
            background.Update(deltaTime);
            
            gameManager.Update(deltaTime);
            
            enemySpawner.Update(deltaTime, gameManager.GetEnemies());
            
            powerupManager.Update(deltaTime, mainPlayer);

            for (int i = 0; i < gameManager.GetEnemies().size(); i++) {
                if (gameManager.GetEnemies()[i].GetHealth() <= 0) {
                    // Chance to spawn a power-up where the enemy died
                    powerupManager.SpawnPowerUpAtPosition(gameManager.GetEnemies()[i].GetPosition());
                }
            }
            gameManager.CheckCollisions();
            // Check for level completion
            if (enemySpawner.GetWaveNumber() > 3)
            {
                gameManager.SetGameState(GameManager::VICTORY);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        background.Draw();
        gameManager.Draw();
        powerupManager.Draw();
        // Draw HUD
        if (gameManager.GetGameState() == GameManager::PLAYING)
        {
            gameManager.DrawHUD();

            // Draw wave number
            char waveText[20];
            sprintf_s(waveText, "WAVE: %d", enemySpawner.GetWaveNumber());
            DrawText(waveText, GetScreenWidth() - 200, 50, 20, WHITE);
        }

        EndDrawing();
    }

    UnloadTexture(playerTexture);
    UnloadTexture(enemy1);
    UnloadTexture(enemy2);
    UnloadTexture(enemy3);
    UnloadTexture(bulletTexture);
    UnloadTexture(enemyBulletTexture);
    UnloadTexture(explosionTexture);
    UnloadTexture(backgroundTexture);
    UnloadTexture(healthPowerupTexture);
    UnloadTexture(shieldPowerupTexture);
    UnloadTexture(rapidFirePowerupTexture);
    UnloadTexture(extraLifePowerupTexture);

    audioManager.CleanUp();
    CloseWindow();
    return 0;
}
