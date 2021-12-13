#include <stdlib.h>
#include <string.h>

template <typename T>
class Bools
{
public:
    enum
    {
        len = 8 * sizeof(T)
    };
    typedef T bit_field;

    operator int()
    {
        return (int)get();
    }

    operator bit_field()
    {
        return get();
    }

    Bools &operator=(bool reg[len])
    {
        set(reg);
        return *this;
    }

    void set(bool reg[len])
    {
        memcpy(_reg, reg, len);
    }

    void set(bit_field reg)
    {
        _regist_uppack(reg);
    }

    void get(bool reg[len])
    {
        memcpy(reg, _reg, len);
    }

    bit_field get()
    {
        return _regist_pack();
    }

    bool get(int bit)
    {
        return _reg[bit];
    }

private:
    bool _reg[len];
    bit_field _regist_pack()
    {
        bit_field ret = 0;
        for (int i = 0; i < len; i++)
            ret |= _reg[i] << i;
        return ret;
    }

    void _regist_uppack(bit_field reg)
    {
        for (int i = 0; i < len; i++)
            _reg[i] = (reg >> i) & 0x01;
    }
};

typedef Bools<uint8_t> bools8_t;
typedef Bools<uint16_t> bools16_t;