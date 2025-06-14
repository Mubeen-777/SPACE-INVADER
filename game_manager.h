#pragma once

class GameManager {
private:

    enum GameState;//forward declaration
    player* mainPlayer;
    Dynamic_array<enemy> enemies;
    Dynamic_array<bullet> playerBullets;
    Dynamic_array<bullet> enemyBullets;
    Dynamic_array<explosion> explosions;

    GameState currentState;
    float gameTimer;

    float enemyShootTimer;

public:
    enum GameState {
        MAIN_MENU,
        PLAYING,
        PAUSED,
        GAME_OVER,
        VICTORY
    };

    GameManager(player* p) : mainPlayer(p), currentState(MAIN_MENU), gameTimer(0.0), enemyShootTimer(0.0) {}

    void Update(float dt) {
        if (currentState == PLAYING) {
            gameTimer += dt;
            enemyShootTimer += dt;

            if (mainPlayer && mainPlayer->IsAlive())
                mainPlayer->Update(dt);
            else if (mainPlayer) {
                currentState = GAME_OVER;
            }

            // Update bullets
            UpdateBullets(dt);

            // Update enemies
            UpdateEnemies(dt);

            // Enemy shooting logic
            if (enemyShootTimer >= 1.0) {
                enemyShootTimer = 0.0;
                TryEnemyShoot();
            }

            // Update explosions
            for (int i = 0; i < explosions.size(); i++) {
                explosions[i].Update(dt);
            }

            // Clean up inactive explosions
            for (int i = 0; i < explosions.size(); ) {
                if (!explosions[i].IsActive()) {
                    explosions.delete_at(i);
                }
                else {
                    i++;
                }
            }
        }
    }

    void UpdateBullets(float dt) {
        // Update player bullets
        for (int i = 0; i < playerBullets.size(); ) {
            playerBullets[i].Update(dt);

            if (playerBullets[i].IsOffScreen()) {
                playerBullets.delete_at(i);
            }
            else
            {
                i++;
            }
        }

        // Update enemy bullets
        for (int i = 0; i < enemyBullets.size(); ) {
            enemyBullets[i].Update(dt);

            if (enemyBullets[i].IsOffScreen()) {
                enemyBullets.delete_at(i);
            }
            else {
                i++;
            }
        }
    }

    void UpdateEnemies(float dt) {
        for (int i = 0; i < enemies.size(); ) {
            enemies[i].Update(dt);

            if (enemies[i].GetHealth() <= 0) {
                // Create explosion at enemy position
                AddExplosion(enemies[i].GetPosition());
                // Add score to player
                if (mainPlayer)
                {
                    mainPlayer->AddScore(enemies[i].GetScoreValue());
                }
                enemies.delete_at(i);
            }
            else if (enemies[i].IsOffScreen()) {
                enemies.delete_at(i);
            }
            else {
                i++;
            }
        }
    }

    void TryEnemyShoot() {
        for (int i = 0; i < enemies.size(); i++) {
            if (enemies[i].CanShoot() && GetRandomValue(0, 100) < 20) { // 20% chance to shoot
                Vector2 bulletPos = enemies[i].GetPosition();
                bulletPos.y += enemies[i].GetTexture().height;
                Vector2 velocity = { 0, 250 }; // Slower than player bullets
                bullet newBullet(bulletPos, GetEnemyBulletTexture(), velocity, "enemy");
                enemyBullets.push(newBullet);
                enemies[i].ResetShootTimer();
            }
        }
    }
    void SetGameTimer(float timer) { gameTimer = timer; }
    float GetGameTimer() const { return gameTimer; }
    void Draw() {
        if (currentState == MAIN_MENU) {
            DrawMainMenu();
        }
        else if (currentState == PLAYING || currentState == PAUSED) {
            // Draw game objects
            if (mainPlayer)
                mainPlayer->Draw();

            for (int i = 0; i < playerBullets.size(); i++)
                playerBullets[i].Draw();

            for (int i = 0; i < enemyBullets.size(); i++)
                enemyBullets[i].Draw();

            for (int i = 0; i < enemies.size(); i++)
                enemies[i].Draw();

            for (int i = 0; i < explosions.size(); i++)
                explosions[i].Draw();

            if (currentState == PAUSED) {
                DrawPauseScreen();
            }
        }
        else if (currentState == GAME_OVER) {
            DrawGameOverScreen();
        }
        else if (currentState == VICTORY) {
            DrawVictoryScreen();
        }
    }

    void AddPlayerBullet(const bullet& b) {
        playerBullets.push(b);
    }

    void AddEnemyBullet(const bullet& b) {
        enemyBullets.push(b);
    }

    void AddEnemy(const enemy& e) {
        enemies.push(e);
    }

    void AddExplosion(Vector2 pos)
    {
        if (explosionTexture.id != 0)
        {
            explosion newExplosion(pos, explosionTexture, 3, 0.05);
            explosions.push(newExplosion);
        }
    }

    void ClearAll() {
        enemies.clear();
        playerBullets.clear();
        enemyBullets.clear();
        explosions.clear();
    }

    void SetGameState(GameState state) {
        currentState = state;
    }

    GameState GetGameState() const {
        return currentState;
    }

    void StartGame() {
        ClearAll();
        if (mainPlayer) {
            Vector2 startPos = { GetScreenWidth() / 2.0 - mainPlayer->GetTexture().width / 2.0,
                                GetScreenHeight() - mainPlayer->GetTexture().height - 10 };
            mainPlayer->ResetGame();
            mainPlayer->SetPosition(startPos);
        }
        currentState = PLAYING;
        gameTimer = 0.0;
    }

    Texture2D explosionTexture;
    Texture2D enemyBulletTexture;

    void SetExplosionTexture(Texture2D tex) {
        explosionTexture = tex;
    }

    void SetEnemyBulletTexture(Texture2D tex) {
        enemyBulletTexture = tex;
    }

    Texture2D GetEnemyBulletTexture() const {
        return enemyBulletTexture;
    }

    Dynamic_array<enemy>& GetEnemies() { return enemies; }
    Dynamic_array<bullet>& GetPlayerBullets() { return playerBullets; }
    Dynamic_array<bullet>& GetEnemyBullets() { return enemyBullets; }

    // Check collisions between game objects
    void CheckCollisions()
    {
        if (!mainPlayer || !mainPlayer->IsAlive()) return;

        // Player bullets vs enemies
        for (int i = 0; i < playerBullets.size(); i++) {
            for (int j = 0; j < enemies.size(); j++) {
                if (CheckCollisionRecs(playerBullets[i].GetBounds(), enemies[j].GetBounds())) {
                    enemies[j].TakeDamage(playerBullets[i].GetDamage());
                    playerBullets.delete_at(i);
                    // Break because the bullet is gone
                    break;
                }
            }
        }

        // Enemy bullets vs player
        for (int i = 0; i < enemyBullets.size(); i++) {
            if (CheckCollisionRecs(enemyBullets[i].GetBounds(), mainPlayer->GetBounds())) {
                mainPlayer->TakeDamage(enemyBullets[i].GetDamage());
                enemyBullets.delete_at(i);
                // Add small explosion at hit point
                AddExplosion(mainPlayer->GetPosition());
                break;
            }
        }

        //collision damage
        for (int i = 0; i < enemies.size(); i++) {
            if (CheckCollisionRecs(enemies[i].GetBounds(), mainPlayer->GetBounds())) {
                mainPlayer->TakeDamage(25);
                enemies[i].TakeDamage(50);
                AddExplosion(enemies[i].GetPosition());
            }
        }
    }

    void DrawHUD() {
        if (!mainPlayer) return;

        // Draw health bar
        int healthBarWidth = 200;
        int healthBarHeight = 20;
        int healthAmount = (mainPlayer->GetHealth() * healthBarWidth) / 100;

        DrawRectangle(20, 20, healthBarWidth, healthBarHeight, GRAY);
        DrawRectangle(20, 20, healthAmount, healthBarHeight, RED);
        DrawRectangleLines(20, 20, healthBarWidth, healthBarHeight, WHITE);
        DrawText("HEALTH", 25, 22, 15, WHITE);

        // Draw lives
        for (int i = 0; i < mainPlayer->GetLives(); i++) {
            DrawRectangle(20 + i * 30, 50, 20, 20, GREEN);
        }
        DrawText("LIVES", 25, 52, 15, WHITE);

        // Draw score
        char scoreText[50];
        sprintf_s(scoreText, "SCORE: %d", mainPlayer->GetScore());
        DrawText(scoreText, GetScreenWidth() - 200, 20, 20, WHITE);
    }

    void DrawMainMenu() {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        DrawText("SPACE SHOOTER", screenWidth / 2 - 150, screenHeight / 3, 40, WHITE);
        DrawText("Press ENTER to Start New Game", screenWidth / 2 - 150, screenHeight / 2 - 20, 20, GREEN);
        DrawText("Press L to Load Game", screenWidth / 2 - 120, screenHeight / 2 + 20, 20, YELLOW);
        DrawText("Press ESC to Quit", screenWidth / 2 - 100, screenHeight / 2 + 60, 20, RED);
    }

    void DrawPauseScreen() {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
        DrawText("GAME PAUSED", screenWidth / 2 - 120, screenHeight / 2 - 60, 40, WHITE);
        DrawText("Press P to Resume", screenWidth / 2 - 100, screenHeight / 2, 20, GREEN);
        DrawText("Press S to Save Game", screenWidth / 2 - 100, screenHeight / 2 + 40, 20, YELLOW);
        DrawText("Press ESC to Quit", screenWidth / 2 - 100, screenHeight / 2 + 80, 20, RED);
    }

    void DrawGameOverScreen() {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
        DrawText("GAME OVER", screenWidth / 2 - 120, screenHeight / 3, 40, RED);

        if (mainPlayer) {
            char scoreText[50];
            sprintf_s(scoreText, "Final Score: %d", mainPlayer->GetScore());
            DrawText(scoreText, screenWidth / 2 - 100, screenHeight / 2, 20, WHITE);
        }

        DrawText("Press ENTER to Try Again", screenWidth / 2 - 140, screenHeight / 2 + 40, 20, GREEN);
        DrawText("Press ESC to Quit", screenWidth / 2 - 100, screenHeight / 2 + 80, 20, RED);
    }

    void DrawVictoryScreen() {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
        DrawText("VICTORY!", screenWidth / 2 - 100, screenHeight / 3, 40, GREEN);

        if (mainPlayer) {
            char scoreText[50];
            sprintf_s(scoreText, "Final Score: %d", mainPlayer->GetScore());
            DrawText(scoreText, screenWidth / 2 - 100, screenHeight / 2, 20, WHITE);
        }

        DrawText("Press ENTER to Play Again", screenWidth / 2 - 140, screenHeight / 2 + 40, 20, GREEN);
        DrawText("Press ESC to Quit", screenWidth / 2 - 100, screenHeight / 2 + 80, 20, RED);
    }
};
