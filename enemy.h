#pragma once

class enemy : public GameObject
{
private:
    float speed;
    int health;
    int scoreValue;
    float shootCooldown;
    float shootTimer;
    bool canShoot;
public:
    enemy()
        : GameObject({ 0, 0 }, Texture2D{}), speed(0.0), health(100),
        scoreValue(100), shootCooldown(2.0), shootTimer(0.0), canShoot(false) {
    }
    enemy(Vector2 pos, Texture2D tex, float spd, int hp = 100, int score = 100, bool shoots = false)
        : GameObject(pos, tex), speed(spd), health(hp), scoreValue(score),
        shootCooldown(2.0), shootTimer(0.0), canShoot(shoots) {}
    void Update(float DeltaTime) override
    {
        Vector2 pos = GetPosition();
        pos.y += speed * DeltaTime;
        SetPosition(pos);
        // Update shoot timer
        if (canShoot) 
        {
            shootTimer += DeltaTime;
        }
    }
    void Draw() override 
    {
        DrawTextureV(GetTexture(), GetPosition(), WHITE);
    }
    Rectangle GetBounds() const override
    {
        Vector2 pos = GetPosition();
        Texture2D tex = GetTexture();
        return { pos.x, pos.y, (float)tex.width, (float)tex.height };
    }
    void TakeDamage(int amount) {
        health -= amount;
    }
    int GetHealth() const {
        return health;
    }
    int GetScoreValue() const
    {
        return scoreValue;
    }
    bool CanShoot() const
    {
        return canShoot && shootTimer >= shootCooldown;
    }
    void ResetShootTimer() 
    {
        shootTimer = 0.0;
    }
    bool IsOffScreen() const
    {
        return GetPosition().y > GetScreenHeight() + GetTexture().height;
    }
};