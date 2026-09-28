#ifndef math_hpp
#define math_hpp

#include <cmath>

struct vector2
{
    float x, y;

    constexpr vector2( ) : x( 0.f ), y( 0.f )
    {
    }
    constexpr vector2( float x, float y ) : x( x ), y( y )
    {
    }

    constexpr vector2 operator+( const vector2& other ) const
    {
        return { x + other.x, y + other.y };
    }

    constexpr vector2 operator-( const vector2& other ) const
    {
        return { x - other.x, y - other.y };
    }

    constexpr vector2 operator*( float scalar ) const
    {
        return { x * scalar, y * scalar };
    }

    constexpr vector2 operator/( float scalar ) const
    {
        return { x / scalar, y / scalar };
    }

    vector2& operator+=( const vector2& other )
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    vector2& operator-=( const vector2& other )
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    vector2& operator*=( float scalar )
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    vector2& operator/=( float scalar )
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    constexpr bool operator==( const vector2& other ) const
    {
        return x == other.x && y == other.y;
    }

    constexpr bool operator!=( const vector2& other ) const
    {
        return !( *this == other );
    }

    constexpr float length_squared( ) const
    {
        return x * x + y * y;
    }

    float length( ) const
    {
        return std::sqrt( length_squared( ) );
    }

    vector2 normalized( ) const
    {
        const float len = length( );

        if ( len == 0.f )
            return {};

        return { x / len, y / len };
    }

    void normalize( )
    {
        const float len = length( );

        if ( len == 0.f )
            return;

        x /= len;
        y /= len;
    }

    constexpr float dot( const vector2& other ) const
    {
        return x * other.x + y * other.y;
    }

    constexpr float distance_squared( const vector2& other ) const
    {
        const float dx = x - other.x;
        const float dy = y - other.y;

        return dx * dx + dy * dy;
    }

    float distance( const vector2& other ) const
    {
        return std::sqrt( distance_squared( other ) );
    }
};

struct vector3
{
    float x, y, z;

    constexpr vector3( ) : x( 0.f ), y( 0.f ), z( 0.f )
    {
    }
    constexpr vector3( float x, float y, float z ) : x( x ), y( y ), z( z )
    {
    }

    constexpr vector3 operator+( const vector3& other ) const
    {
        return { x + other.x, y + other.y, z + other.z };
    }

    constexpr vector3 operator-( const vector3& other ) const
    {
        return { x - other.x, y - other.y, z - other.z };
    }

    constexpr vector3 operator*( float scalar ) const
    {
        return { x * scalar, y * scalar, z * scalar };
    }

    constexpr vector3 operator/( float scalar ) const
    {
        return { x / scalar, y / scalar, z / scalar };
    }

    vector3& operator+=( const vector3& other )
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    vector3& operator-=( const vector3& other )
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    vector3& operator*=( float scalar )
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    vector3& operator/=( float scalar )
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    constexpr bool operator==( const vector3& other ) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    constexpr bool operator!=( const vector3& other ) const
    {
        return !( *this == other );
    }

    constexpr float length_squared( ) const
    {
        return x * x + y * y;
    }

    float length( ) const
    {
        return std::sqrt( length_squared( ) );
    }

    vector2 normalized( ) const
    {
        const float len = length( );

        if ( len == 0.0f )
            return {};

        return { x / len, y / len };
    }

    void normalize( )
    {
        const float len = length( );

        if ( len == 0.0f )
            return;

        x /= len;
        y /= len;
    }

    constexpr float dot( const vector2& other ) const
    {
        return x * other.x + y * other.y;
    }

    constexpr float distance_squared( const vector2& other ) const
    {
        const float dx = x - other.x;
        const float dy = y - other.y;

        return dx * dx + dy * dy;
    }

    float distance( const vector2& other ) const
    {
        return std::sqrt( distance_squared( other ) );
    }
};

struct matrix3
{
    float m[ 3 ][ 3 ];
};

struct matrix4
{
    float m[ 4 ][ 4 ];
};

#endif  // math_hpp