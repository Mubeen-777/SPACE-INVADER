#pragma once

class explosion : public GameObject {
private:
    int frameWidth, frameHeight;
    int totalFrames;
    int currentFrame;
    float frameTime;     // Time per frame
    float frameTimer;
    bool active;
public:
    explosion()
        : GameObject({ 0, 0 }, Texture2D{}),
        frameWidth(0), frameHeight(0), totalFrames(0),
        currentFrame(0), frameTime(0.1), frameTimer(0.0), active(false) {
    }
    explosion(Vector2 pos, Texture2D sprite, int frames, float speed = 0.1)
        : GameObject(pos, sprite), totalFrames(frames),
        currentFrame(0), frameTime(speed), frameTimer(0.0), active(true)
    {
        frameWidth = sprite.width;
        frameHeight = sprite.height;
    }
    void Update(float deltaTime) override
    {
        if (!active) return;
        frameTimer += deltaTime;
        if (frameTimer >= frameTime) {
            frameTimer = 0.0;
            currentFrame++;
            if (currentFrame >= totalFrames) {
                active = false;
            }
        }
    }
    void Draw() override
    {
        if (!active) return;
        Vector2 position = GetPosition();
        Texture2D spriteSheet = GetTexture();
        Rectangle source = { (float)(currentFrame * frameWidth), 0, (float)frameWidth, (float)frameHeight };
        Rectangle dest = { position.x, position.y, (float)frameWidth, (float)frameHeight };
        DrawTexturePro(spriteSheet, source, dest, { 0, 0 }, 0.0, WHITE);
    }
    Rectangle GetBounds() const override
    {
        Vector2 pos = GetPosition();
        return { pos.x, pos.y, (float)frameWidth, (float)frameHeight };
    }
    bool IsActive() const {
        return active;
    }
    void Activate(Vector2 pos) {
        SetPosition(pos);
        currentFrame = 0;
        frameTimer = 0.0;
        active = true;
    }
    void Deactivate() {
        active = false;
    }
    int GetCurrentFrame() const {
        return currentFrame;
    }
    int getTotalFrames() const {
        return totalFrames;
    }
    void SetFrameTime(float time) {
        frameTime = time;
    }
    float GetFrameTime() const {
        return frameTime;
    }
};