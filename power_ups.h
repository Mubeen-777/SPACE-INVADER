#pragma once
class PowerUp : public GameObject {

    enum Type;//forward declartion
private:
    Type type;
    float speed;
    float duration;  // For time-limited powerups
    bool active;
public:

    enum Type {
        HEALTH,
        SHIELD,
        RAPID_FIRE,
        TRIPLE_SHOT,
        EXTRA_LIFE
    };

    PowerUp()
        : GameObject({ 0, 0 }, Texture2D{}), type(HEALTH), speed(100.0), duration(10.0), active(true) {
    }

    PowerUp(Vector2 pos, Texture2D tex, Type powerupType, float moveSpeed = 100.0, float powerupDuration = 10.0)
        : GameObject(pos, tex), type(powerupType), speed(moveSpeed), duration(powerupDuration), active(true) {
    }

    void Update(float DeltaTime) override {
        if (!active) return;

        // Move downward
        Vector2 pos = GetPosition();
        pos.y += speed * DeltaTime;
        SetPosition(pos);
    }

    void Draw() override {
        if (!active) return;

        DrawTextureV(GetTexture(), GetPosition(), WHITE);
    }

    Rectangle GetBounds() const override {
        Vector2 pos = GetPosition();
        Texture2D tex = GetTexture();
        return { pos.x, pos.y, (float)tex.width, (float)tex.height };
    }

    Type GetPowerupType() const {
        return type;
    }

    float GetDuration() const {
        return duration;
    }

    void Deactivate() {
        active = false;
    }

    bool IsActive() const {
        return active;
    }

    bool IsOffScreen() const {
        return GetPosition().y > GetScreenHeight() + GetTexture().height;
    }
    // Apply the powerup effect to the player
    void Apply(player& playerRef) {
        switch (type) {
        case HEALTH:
            playerRef.Heal(50);
            break;
        case SHIELD:
            // Make player invulnerable for the shield duration
            playerRef.SetInvulnerable(true, duration);
            break;
        case RAPID_FIRE:
            playerRef.SetShootCooldown(0.1);
            break;
        case EXTRA_LIFE:
            playerRef.AddLife();
            break;
        default:
            break;
        }
    }
};