////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Math.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Size::Size(): width(0.0f), height(0.0f)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Size::Size(float w, float h): width(w), height(h)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Size::Size(const Sizei& size): width(float(size.width)), height(float(size.height))
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Size::operator==(const Size& size) const
{
    return width == size.width && height == size.height;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Size::operator!=(const Size& size) const
{
    return !(*this == size);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Size Size::operator+(const Size& size) const
{
    return Size(width + size.width, height + size.height);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Size Size::operator-(const Size& size) const
{
    return Size(width - size.width, height - size.height);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Size Size::operator*(float k) const
{
    return Size(width * k, height * k);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Size Size::operator/(float k) const
{
    return Size(width / k, height / k);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Size& Size::operator+=(const Size& size)
{
    width += size.width;
    height += size.height;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Size& Size::operator-=(const Size& size)
{
    width -= size.width;
    height -= size.height;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline Size& Size::operator*=(float k)
{
    width *= k;
    height *= k;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Size& Size::operator/=(float k)
{
    width /= k;
    height /= k;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Size::Expand(const Size& size)
{
    width = Max(width, size.width);
    height = Max(height, size.height);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Size::Scale(float scaleX, float scaleY)
{
    width *= scaleX;
    height *= scaleY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Size Size::Zero()
{
    return Size(0.0f, 0.0f);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Size Size::Infinite()
{
    return Size(FLT_INF, FLT_INF);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Sizei::Sizei(): width(0), height(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Sizei::Sizei(uint32_t w, uint32_t h): width(w), height(h)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Sizei::Sizei(const Size& size): width(uint32_t(size.width)), height(uint32_t(size.height))
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Sizei::operator==(const Sizei& size) const
{
    return width == size.width && height == size.height;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Sizei::operator!=(const Sizei& size) const
{
    return !(*this == size);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Sizei Sizei::operator+(const Sizei& size) const
{
    return Sizei(width + size.width, height + size.height);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Sizei Sizei::operator-(const Sizei& size) const
{
    return Sizei(width - size.width, height - size.height);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Sizei Sizei::operator*(uint32_t k) const
{
    return Sizei(width * k, height * k);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Sizei Sizei::operator/(uint32_t k) const
{
    return Sizei(width / k, height / k);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Sizei& Sizei::operator+=(const Sizei& size)
{
    width += size.width;
    height += size.height;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Sizei& Sizei::operator-=(const Sizei& size)
{
    width -= size.width;
    height -= size.height;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline Sizei& Sizei::operator*=(uint32_t k)
{
    width *= k;
    height *= k;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Sizei& Sizei::operator/=(uint32_t k)
{
    width /= k;
    height /= k;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Sizei::Expand(const Sizei& size)
{
    width = Max(width, size.width);
    height = Max(height, size.height);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Sizei::Scale(uint32_t scaleX, uint32_t scaleY)
{
    width *= scaleX;
    height *= scaleY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Sizei Sizei::Zero()
{
    return Sizei(0, 0);
}

}
