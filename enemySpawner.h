#pragma once
class EnemySpawner
{
private:
    int maxEnemies;
    Texture2D enemy_lvl1;
    Texture2D enemy_lvl2;
    Texture2D enemy_lvl3;

    float minX, maxX;
    float spawnY;

    int waveNumber;
    int enemiesPerWave;
    float difficultyScaling;


    float enemy_1_SpawnTimer;
    float enemy_1_SpawnInterval;

    float enemy_2_SpawnTimer;
    float enemy_2_SpawnInterval;

    float enemy_3_SpawnTimer;
    float enemy_3_SpawnInterval;



public:
    EnemySpawner(Texture2D tex1, Texture2D tex2, Texture2D tex3, float interval = 2.0, int max = 10, float minSpawnX = 0, float maxSpawnX = 800, float spawnHeight = -50.0f)

        : enemy_1_SpawnTimer(0.0), enemy_1_SpawnInterval(interval), maxEnemies(max),
        enemy_lvl1(tex1), enemy_lvl2(tex2), enemy_lvl3(tex3), minX(minSpawnX), maxX(maxSpawnX), spawnY(spawnHeight),
        waveNumber(1), enemiesPerWave(5), difficultyScaling(1.0), enemy_2_SpawnTimer(0.0), enemy_2_SpawnInterval(60.0) {
    }

    void Update(float dt, Dynamic_array<enemy>& enemies)
    {
        enemy_1_SpawnTimer += dt;
        enemy_2_SpawnTimer += dt;
        enemy_3_SpawnTimer += dt;

        if (enemy_1_SpawnTimer >= enemy_1_SpawnInterval && enemies.size() < maxEnemies && waveNumber == 1)
        {
            enemy_1_SpawnTimer = 0.0;

            float randX = GetRandomValue(int(minX), int(maxX) - enemy_lvl1.width);

            Vector2 spawnPos = { randX, spawnY };//horizontal axis per kahi bhi specific y per enemy spawn ho jye gaw 

            float speed = GetRandomValue(100, 200) * difficultyScaling;  //different enemies have diff speed 
            int health = 100;
            int scoreValue = 100;

            bool canShoot = true;

            //add this newly created enemy in dynamic enemy array 
            enemy newEnemy(spawnPos, enemy_lvl1, speed, health, scoreValue, canShoot);
            enemies.push(newEnemy);
        }
        if (enemy_2_SpawnTimer >= enemy_2_SpawnInterval && enemies.size() < maxEnemies && waveNumber == 2)
        {
            enemy_2_SpawnTimer = 0.0;

            float randX = GetRandomValue(int(minX), int(maxX) - enemy_lvl2.width);

            Vector2 spawnPos = { randX, spawnY };//horizontal axis per kahi bhi specific y per enemy spawn ho jye gaw 

            float speed = GetRandomValue(100, 200) * difficultyScaling;  //different enemies have diff speed 
            int health = 100;
            int scoreValue = 100;

            bool canShoot = true;

            //add this newly created enemy in dynamic enemy array 
            enemy newEnemy(spawnPos, enemy_lvl2, speed, health, scoreValue, canShoot);
            enemies.push(newEnemy);
        }
        if (enemy_3_SpawnTimer >= enemy_3_SpawnInterval && enemies.size() < maxEnemies && waveNumber == 3)
        {
            enemy_3_SpawnTimer = 0.0;

            float randX = GetRandomValue(int(minX), int(maxX) - enemy_lvl3.width);

            Vector2 spawnPos = { randX, spawnY };//horizontal axis per kahi bhi specific y per enemy spawn ho jye gaw 

            float speed = GetRandomValue(100, 200) * difficultyScaling;  //different enemies have diff speed 
            int health = 100;
            int scoreValue = 100;
            bool canShoot = true;

            //add this newly created enemy in dynamic enemy array 
            enemy newEnemy(spawnPos, enemy_lvl3, speed, health, scoreValue, canShoot);
            enemies.push(newEnemy);
        }
    }


    void NextWave() {
        waveNumber++;
        enemiesPerWave += 2;
        difficultyScaling += 0.1;
        enemy_1_SpawnInterval *= 0.9;
        enemy_2_SpawnInterval *= 0.9;
        enemy_3_SpawnInterval *= 0.9;

    }

    void ResetSpawner() {
        waveNumber = 1;
        enemiesPerWave = 5;
        difficultyScaling = 1.0;
        enemy_1_SpawnInterval = 2.0;
        enemy_2_SpawnTimer = 0.0;
    }

    int GetWaveNumber() const
    {
        return waveNumber;
    }
};