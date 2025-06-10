#pragma once
class GameObject
{
private:
    Vector2 position;
    Texture2D texture;
public:
    GameObject(Vector2 pos, Texture2D tex) : position(pos), texture(tex) {}
    virtual void Update(float DeltaTime) = 0;
    virtual void Draw() = 0;
    virtual Rectangle GetBounds() const = 0;
    Vector2 GetPosition() const
    {
        return position;
    }
    void SetPosition(Vector2 newPos)
    {
        position = newPos;
    }
    Texture2D GetTexture() const
    {
        return texture;
    }
    void SetTexture(Texture2D newTexture)
    {
        texture = newTexture;
    }
    virtual ~GameObject() {}
};