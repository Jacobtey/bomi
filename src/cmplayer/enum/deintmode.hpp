#ifndef DEINTMODE_HPP
#define DEINTMODE_HPP

#include "enums.hpp"
#define DEINTMODE_IS_FLAG 0

enum class DeintMode : int {
    None = (int)0,
    Auto = (int)1
};

Q_DECLARE_METATYPE(DeintMode)

inline auto operator == (DeintMode e, int i) -> bool { return (int)e == i; }
inline auto operator != (DeintMode e, int i) -> bool { return (int)e != i; }
inline auto operator == (int i, DeintMode e) -> bool { return (int)e == i; }
inline auto operator != (int i, DeintMode e) -> bool { return (int)e != i; }
inline auto operator > (DeintMode e, int i) -> bool { return (int)e > i; }
inline auto operator < (DeintMode e, int i) -> bool { return (int)e < i; }
inline auto operator >= (DeintMode e, int i) -> bool { return (int)e >= i; }
inline auto operator <= (DeintMode e, int i) -> bool { return (int)e <= i; }
inline auto operator > (int i, DeintMode e) -> bool { return i > (int)e; }
inline auto operator < (int i, DeintMode e) -> bool { return i < (int)e; }
inline auto operator >= (int i, DeintMode e) -> bool { return i >= (int)e; }
inline auto operator <= (int i, DeintMode e) -> bool { return i <= (int)e; }
#if DEINTMODE_IS_FLAG
Q_DECLARE_FLAGS(, DeintMode)
Q_DECLARE_OPERATORS_FOR_FLAGS()
Q_DECLARE_METATYPE()
#else
inline auto operator & (DeintMode e, int i) -> int { return (int)e & i; }
inline auto operator & (int i, DeintMode e) -> int { return (int)e & i; }
inline auto operator &= (int &i, DeintMode e) -> int& { return i &= (int)e; }
inline auto operator ~ (DeintMode e) -> int { return ~(int)e; }
inline auto operator | (DeintMode e, int i) -> int { return (int)e | i; }
inline auto operator | (int i, DeintMode e) -> int { return (int)e | i; }
constexpr inline auto operator | (DeintMode e1, DeintMode e2) -> int { return (int)e1 | (int)e2; }
inline auto operator |= (int &i, DeintMode e) -> int& { return i |= (int)e; }
#endif

template<>
class EnumInfo<DeintMode> {
    typedef DeintMode Enum;
public:
    typedef DeintMode type;
    using Data =  QVariant;
    struct Item {
        Enum value;
        QString name, key;
        QVariant data;
    };
    using ItemList = std::array<Item, 2>;
    static constexpr auto size() -> int
    { return 2; }
    static constexpr auto typeName() -> const char*
    { return "DeintMode"; }
    static constexpr auto typeKey() -> const char*
    { return "deinterlacing"; }
    static auto typeDescription() -> QString
    { return qApp->translate("EnumInfo", "Deinterlacing"); }
    static auto item(Enum e) -> const Item*
    { return 0 <= e && e < size() ? &info[(int)e] : nullptr; }
    static auto name(Enum e) -> QString
    { auto i = item(e); return i ? i->name : QString(); }
    static auto key(Enum e) -> QString
    { auto i = item(e); return i ? i->key : QString(); }
    static auto data(Enum e) -> QVariant
    { auto i = item(e); return i ? i->data : QVariant(); }
    static auto description(int e) -> QString
    { return description((Enum)e); }
    static auto description(Enum e) -> QString
    {
        switch (e) {
        case Enum::None: return qApp->translate("EnumInfo", "Off");
        case Enum::Auto: return qApp->translate("EnumInfo", "Auto");
        default: return "";
        }
    }
    static constexpr auto items() -> const ItemList&
    { return info; }
    static auto from(int id, Enum def = default_()) -> Enum
    {
        auto it = std::find_if(info.cbegin(), info.cend(),
                               [id] (const Item &item)
                               { return item.value == id; });
        return it != info.cend() ? it->value : def;
    }
    static auto from(const QString &name, Enum def = default_()) -> Enum
    {
        auto it = std::find_if(info.cbegin(), info.cend(),
                               [&name] (const Item &item)
                               { return !name.compare(item.name); });
        return it != info.cend() ? it->value : def;
    }
    static auto fromData(const QVariant &data,
                         Enum def = default_()) -> Enum
    {
        auto it = std::find_if(info.cbegin(), info.cend(),
                               [&data] (const Item &item)
                               { return item.data == data; });
        return it != info.cend() ? it->value : def;
    }
    static constexpr auto default_() -> Enum
    { return DeintMode::Auto; }
private:
    static const ItemList info;
};

using DeintModeInfo = EnumInfo<DeintMode>;

#endif
