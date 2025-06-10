#pragma once

class bullet : public GameObject
{
private:
    Vector2 velocity;
    String ownerType; // enemy or player
    int damage;

public:

    bullet()
        : GameObject({ 0, 0 }, Texture2D{}), velocity({ 0, 0 }), ownerType("unknown"), damage(100) {}
    bullet(Vector2 pos, Texture2D tex, Vector2 vel, const String& owner, int dmg = 100)
        : GameObject(pos, tex), velocity(vel), ownerType(owner), damage(dmg) {}
    bullet(const bullet& other)
        : GameObject(other.GetPosition(), other.GetTexture()),
        velocity(other.velocity), ownerType(other.ownerType), damage(other.damage)  {}
    bullet& operator=(const bullet& other) {
        if (this != &other) {
            SetPosition(other.GetPosition());
            SetTexture(other.GetTexture());
            velocity = other.velocity;
            ownerType = other.ownerType;
            damage = other.damage;
        }
        return *this;
    }
    void Update(float DeltaTime) override
    {
        Vector2 pos = GetPosition();
        pos.x += velocity.x * DeltaTime;
        pos.y += velocity.y * DeltaTime;
        SetPosition(pos);
    }
    void Draw() override
    {
        DrawTextureV(GetTexture(), GetPosition(), WHITE);
    }
    Rectangle GetBounds() const override
    {
        Vector2 pos = GetPosition();
        Texture2D tex = GetTexture();
        return { pos.x, pos.y, float(tex.width), float(tex.height) };
    }
    String GetOwnerType() const
    {
        return ownerType;
    }
    int GetDamage() const
    {
        return damage;
    }
    bool IsOffScreen() const
    {
        Vector2 pos = GetPosition();
        return pos.y < -50 || pos.y > GetScreenHeight() + 50 ||
               pos.x < -50 || pos.x > GetScreenWidth() + 50;
    }
};