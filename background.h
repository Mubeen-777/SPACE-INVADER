#pragma once

class Background : public GameObject {
private:
    Vector2 position2;  // Second background position for infinite scrolling
    float scrollSpeed;
public:
    Background() : GameObject({ 0, 0 }, Texture2D{}), scrollSpeed(100.0)
    {
        position2 = { 0, 0 };
    }
    Background(Texture2D tex, float speed = 100.0) : GameObject({ 0, 0 }, tex), scrollSpeed(speed)
    {
        Vector2 pos = GetPosition();
        position2 = { 0, float(-1 * tex.height) };
    }
    Background(Vector2 pos, Texture2D tex, float speed = 100.0) : GameObject(pos, tex), scrollSpeed(speed)
    {
        position2 = { 0, float(-1 * tex.height) };
    }

    void Update(float deltaTime) override
    {
        Vector2 position = GetPosition();
        Texture2D texture = GetTexture();
        // Scroll both backgrounds
        position.y += scrollSpeed * deltaTime;
        position2.y += scrollSpeed * deltaTime;
        // If the first background has scrolled off screen, reset it to the top
        if (position.y >= GetScreenHeight())
        {
            position.y = position2.y - texture.height;
        }
        // If the second background has scrolled off screen, reset it to the top
        if (position2.y >= GetScreenHeight()) 
        {
            position2.y = position.y - texture.height;
        }
        // Update the position in the parent class
        SetPosition(position);
    }

    void Draw() override
    {
        Vector2 position = GetPosition();
        Texture2D texture = GetTexture();
        DrawTextureV(texture, position, WHITE);
        DrawTextureV(texture, position2, WHITE);
    }
    Rectangle GetBounds() const override 
    {
        Vector2 pos = GetPosition();
        Texture2D tex = GetTexture();
        return { pos.x, pos.y, (float)tex.width, (float)tex.height };
    }
    void SetScrollSpeed(float speed) {
        scrollSpeed = speed;
    }

    float GetScrollSpeed() const {
        return scrollSpeed;
    }

    void ResetPosition() 
    {
        SetPosition({ 0, 0 });
        position2 = { 0, float(-1 * GetTexture().height) };
    }

    void PauseScrolling() {
        scrollSpeed = 0.0;
    }

    void ResumeScrolling(float speed) {
        scrollSpeed = speed;
    }

    Vector2 GetSecondPosition() const {
        return position2;
    }

    void SetSecondPosition(Vector2 pos) {
        position2 = pos;
    }

};
