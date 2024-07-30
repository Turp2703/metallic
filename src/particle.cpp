#include "particle.h"

#include <algorithm>
#include <cmath>

// Constructors
Particle::Particle(Vector2 p_pos, float p_angle, double p_maxTime)
    : m_alive(true), m_position(p_pos), m_angle(p_angle)
    , m_maxTime(p_maxTime), m_gravity(0.f)
    , m_speed(GetRandomValue(20, 40) / 10.f), m_startTime(GetTime())
{
    /* */
}
Particle::Particle(Vector2 p_pos, float p_angle, double p_maxTime, float p_gravity)
    : m_alive(true), m_position(p_pos), m_angle(p_angle)
    , m_maxTime(p_maxTime), m_gravity(p_gravity)
    , m_speed(GetRandomValue(20, 40) / 10.f), m_startTime(GetTime())
{
    /* */
}
Particle::Particle(Vector2 p_pos, float p_angle, double p_maxTime, float p_gravity, float p_speed)
    : m_alive(true), m_position(p_pos), m_angle(p_angle)
    , m_maxTime(p_maxTime), m_gravity(p_gravity)
    , m_speed(p_speed), m_startTime(GetTime())
{
    /* */
}

void Particle::update(){
    // Calculate life
    m_alive = GetTime() - m_startTime < m_maxTime;
    
    // Calculate movement
    m_position = {
        static_cast<float>(m_position.x + cos(m_angle * DEG2RAD) * -m_speed),
        static_cast<float>(m_position.y + sin(m_angle * DEG2RAD) * m_speed) + m_gravity
    };
}

void Particle::draw(Texture2D &p_particleTexture, Color p_color, bool p_endGlow){
    DrawTexturePro( p_particleTexture
                  , { 0.f, 0.f, (float)p_particleTexture.width, (float)p_particleTexture.height }
                  , { m_position.x, m_position.y, (float)p_particleTexture.width, (float)p_particleTexture.height }
                  , { p_particleTexture.width / 2.f, p_particleTexture.height / 2.f }
                  , GetRandomValue(1, 90)
                  , { p_color.r, p_color.g, p_color.b, mapTimeToAlpha(GetTime() - m_startTime, m_maxTime, p_endGlow) });
}
unsigned char Particle::mapTimeToAlpha(double p_current, double p_max, bool p_endGlow){
    if(p_endGlow)
        return static_cast<unsigned char>((static_cast<double>(p_current - 1) / (p_max - 1)) * 254);
    
    p_current = std::max(0.0, std::min(p_max, p_current));
    double normalizedValue = p_current / p_max;
    return 255 - static_cast<unsigned char>(normalizedValue * 254);
}

bool Particle::isAlive(){
    return m_alive;
}