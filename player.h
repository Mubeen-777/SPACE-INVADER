#pragma once
class player : public GameObject
{
private:
    float speed;
    int health;
    float shootCooldown;
    float shootTimer;
    int score;
    int lives;
    bool isInvulnerable;
    float invulnerabilityTimer;
    float invulnerabilityDuration;
    bool hasShield;
    float shieldTimer;
    float shieldDuration;
public:
    player()
        : GameObject({ 0, 0 }, Texture2D{}), speed(1000.0), health(100),
        shootCooldown(0.05), shootTimer(0.0), score(0), lives(3),
        isInvulnerable(false), invulnerabilityTimer(0.0), invulnerabilityDuration(1),
        hasShield(false), shieldTimer(0.0), shieldDuration(0.0) {
    }
    player(Vector2 pos, Texture2D tex, float moveSpeed = 1000.0, int hp = 100)
        : GameObject(pos, tex), speed(moveSpeed), health(hp),
        shootCooldown(0.05), shootTimer(0.0), score(0), lives(3),
        isInvulnerable(false), invulnerabilityTimer(0.0), invulnerabilityDuration(1),
        hasShield(false), shieldTimer(0.0), shieldDuration(0.0) {
    }
    void Update(float DeltaTime) override
    {
        Vector2 pos = GetPosition();
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        Texture2D tex = GetTexture();

        // Update shoot timer
        shootTimer += DeltaTime;

        // Update invulnerability timer
        if (isInvulnerable) {
            invulnerabilityTimer += DeltaTime;
            if (invulnerabilityTimer >= invulnerabilityDuration) {
                isInvulnerable = false;
                invulnerabilityTimer = 0.0;
            }
        }

        // Update shield timer
        if (hasShield) {
            shieldTimer += DeltaTime;
            if (shieldTimer >= shieldDuration) {
                hasShield = false;
                shieldTimer = 0.0f;
            }
        }

        // Movement
        if (IsKeyDown(KEY_W)) pos.y -= speed * DeltaTime;
        if (IsKeyDown(KEY_S)) pos.y += speed * DeltaTime;
        if (IsKeyDown(KEY_A)) pos.x -= speed * DeltaTime;
        if (IsKeyDown(KEY_D)) pos.x += speed * DeltaTime;

        // Keep player within screen bounds
        if (pos.x < 0) pos.x = 0;
        if (pos.y < 0) pos.y = 0;
        if (pos.x > screenWidth - tex.width) pos.x = screenWidth - tex.width;
        if (pos.y > screenHeight - tex.height) pos.y = screenHeight - tex.height;
        SetPosition(pos);
    }

    void SetInvulnerable(bool invul, float duration = 1.0f) {
        if (invul) {
            hasShield = true;
            shieldDuration = duration;
            shieldTimer = 0.0;
        }
        else {
            hasShield = false;
        }
    }

    bool HasShield() const {
        return hasShield;
    }

    void Draw() override
    {
        Color playerColor = WHITE;

        // Shield effect - blue tint
        if (hasShield) {
            playerColor = BLUE;
            DrawTextureV(GetTexture(), GetPosition(), playerColor);
        }
        // Damage invulnerability - flashing effect
        else if (isInvulnerable) {
            if ((int)(invulnerabilityTimer * 10) % 2 == 0) {
                DrawTextureV(GetTexture(), GetPosition(), WHITE);
            }
        }
        else {
            DrawTextureV(GetTexture(), GetPosition(), WHITE);
        }
    }

    Rectangle GetBounds() const override
    {
        Vector2 pos = GetPosition();
        Texture2D tex = GetTexture();
        return { pos.x, pos.y, float(tex.width), float(tex.height) };
    }

    int GetHealth() const
    {
        return health;
    }

    void TakeDamage(int damage)
    {
        if (!isInvulnerable && !hasShield) {
            health -= damage;
            isInvulnerable = true;
            invulnerabilityTimer = 0.0f;

            if (health <= 0) {
                lives--;
                if (lives > 0) {
                    health = 100; // Reset health when losing a life
                }
            }
        }
    }

    void Heal(int amount)
    {
        health += amount;
        if (health > 100) health = 100;
    }

    void AddScore(int points)
    {
        score += points;
    }

    int GetScore() const {
        return score;
    }

    int GetLives() const {
        return lives;
    }

    void AddLife() {
        lives++;
    }

    bool CanShoot() const {
        return shootTimer >= shootCooldown;
    }

    void ResetShootTimer() {
        shootTimer = 0.0;
    }

    void SetShootCooldown(float cooldown) {
        shootCooldown = cooldown;
    }

    bool IsAlive() const {
        return lives > 0;
    }

    void Reset(Vector2 startPos) {
        SetPosition(startPos);
        health = 100;
        isInvulnerable = true;
        invulnerabilityTimer = 0.0;
        hasShield = false;
        shieldTimer = 0.0;
    }

    void ResetGame() {
        health = 100;
        lives = 3;
        score = 0;
        isInvulnerable = false;
        invulnerabilityTimer = 0.0;
        hasShield = false;
        shieldTimer = 0.0;
    }
    void SetHealth(int hp) { health = hp; }
    void SetScore(int s) { score = s; }
    void SetLives(int l) { lives = l; }
    void SetInvulnerabilityState(bool invul, float timer) {
        isInvulnerable = invul;
        invulnerabilityTimer = timer;
    }
    void SetShieldState(bool shield, float timer) {
        hasShield = shield;
        shieldTimer = timer;
    }
    bool IsInvulnerable()
    {
        return isInvulnerable;
    }
};
