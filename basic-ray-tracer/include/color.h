#ifndef _COLOUR_H__
#define _COLOUR_H__

#include <stdlib.h>

/*
 #ifdef max
 #undef max
 #define max(a,b)(a>b?a:b)
 #endif

 #ifdef min
 #undef min
 #define min(a,b)(a>b?b:a)
 #endif
 */
inline int min(int a, int b)
{
    return (a < b ? a : b);
}
;
inline int max(int a, int b)
{
    return (a > b ? a : b);
}
;

#define clamp(a,mn,mx) (min(mx, max(mn, a)))

class CRGB
{
public:

    CRGB()
        : m_Red(0), m_Green(0), m_Blue(0)
    {
    }

    CRGB(unsigned char R, unsigned char G, unsigned char B)
        : m_Red(R), m_Green(G), m_Blue(B)
    {
    }

    CRGB(int R, int G, int B)
    {
        m_Red = clamp(R, 0, 255);
        m_Green = clamp(G, 0, 255);
        m_Blue = clamp(B, 0, 255);
    }

    CRGB(const CRGB &C)
        : m_Red(C.m_Red), m_Green(C.m_Green), m_Blue(C.m_Blue)
    {
    }

    unsigned char Red()
    {
        return m_Red;
    }
    ;
    void Red(unsigned char R)
    {
        m_Red = R;
    }
    ;

    unsigned char Green()
    {
        return m_Green;
    }
    ;
    void Green(unsigned char G)
    {
        m_Green = G;
    }
    ;

    unsigned char Blue()
    {
        return m_Blue;
    }
    ;
    void Blue(unsigned char B)
    {
        m_Blue = B;
    }
    ;

    const CRGB& operator+=(const CRGB &Other)
    {
        if(&Other != this)
        {
            m_Red = clamp(m_Red + Other.m_Red, 0, 255);
            m_Green = clamp(m_Green + Other.m_Green, 0, 255);
            m_Blue = clamp(m_Blue + Other.m_Blue, 0, 255);
        }
        return *this;
    }

    const CRGB& operator*=(float S)
    {
        m_Red = (unsigned char)clamp(((float )m_Red) * S, 0.0f, 255.0f);
        m_Green = (unsigned char)clamp(((float )m_Green) * S, 0.0f, 255.0f);
        m_Blue = (unsigned char)clamp(((float )m_Blue) * S, 0.0f, 255.0f);

        return *this;
    }

    friend CRGB operator+(const CRGB &C1, const CRGB &C2)
    {
        unsigned char R = clamp(C1.m_Red + C2.m_Red, 0, 255);
        unsigned char G = clamp(C1.m_Green + C2.m_Green, 0, 255);
        unsigned char B = clamp(C1.m_Blue + C2.m_Blue, 0, 255);

        return CRGB(R, G, B);
    }

    friend CRGB operator*(const CRGB &C, float S)
    {
        unsigned char R = (unsigned char)clamp(((float )C.m_Red) * S,
                                               0.0f,
                                               255.0f);
        unsigned char G = (unsigned char)clamp(((float )C.m_Green) * S,
                                               0.0f,
                                               255.0f);
        unsigned char B = (unsigned char)clamp(((float )C.m_Blue) * S,
                                               0.0f,
                                               255.0f);

        return CRGB(R, G, B);
    }

protected:

    unsigned char m_Red;
    unsigned char m_Green;
    unsigned char m_Blue;
};

#endif // _COLOUR_H__
