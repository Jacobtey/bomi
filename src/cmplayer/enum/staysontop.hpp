#ifndef STAYSONTOP_HPP
#define STAYSONTOP_HPP

#include "enums.hpp"
#define STAYSONTOP_IS_FLAG 0

enum class StaysOnTop : int {
    None = (int)0,
    Playing = (int)1,
    Always = (int)2
};

Q_DECLARE_METATYPE(StaysOnTop)

inline auto operator == (StaysOnTop e, int i) -> bool { return (int)e == i; }
inline auto operator != (StaysOnTop e, int i) -> bool { return (int)e != i; }
inline auto operator == (int i, StaysOnTop e) -> bool { return (int)e == i; }
inline auto operator != (int i, StaysOnTop e) -> bool { return (int)e != i; }
inline auto operator > (StaysOnTop e, int i) -> bool { return (int)e > i; }
inline auto operator < (StaysOnTop e, int i) -> bool { return (int)e < i; }
inline auto operator >= (StaysOnTop e, int i) -> bool { return (int)e >= i; }
inline auto operator <= (StaysOnTop e, int i) -> bool { return (int)e <= i; }
inline auto operator > (int i, StaysOnTop e) -> bool { return i > (int)e; }
inline auto operator < (int i, StaysOnTop e) -> bool { return i < (int)e; }
inline auto operator >= (int i, StaysOnTop e) -> bool { return i >= (int)e; }
inline auto operator <= (int i, StaysOnTop e) -> bool { return i <= (int)e; }
#if STAYSONTOP_IS_FLAG
Q_DECLARE_FLAGS(, StaysOnTop)
Q_DECLARE_OPERATORS_FOR_FLAGS()
Q_DECLARE_METATYPE()
#else
inline auto operator & (StaysOnTop e, int i) -> int { return (int)e & i; }
inline auto operator & (int i, StaysOnTop e) -> int { return (int)e & i; }
inline auto operator &= (int &i, StaysOnTop e) -> int& { return i &= (int)e; }
inline auto operator ~ (StaysOnTop e) -> int { return ~(int)e; }
inline auto operator | (StaysOnTop e, int i) -> int { return (int)e | i; }
inline auto operator | (int i, StaysOnTop e) -> int { return (int)e | i; }
constexpr inline auto operator | (StaysOnTop e1, StaysOnTop e2) -> int { return (int)e1 | (int)e2; }
inline auto operator |= (int &i, StaysOnTop e) -> int& { return i |= (int)e; }
#endif

template<>
class EnumInfo<StaysOnTop> {
    typedef StaysOnTop Enum;
public:
    typedef StaysOnTop type;
    using Data =  QVariant;
    struct Item {
        Enum value;
        QString name, key;
        QVariant data;
    };
    using ItemList = std::array<Item, 3>;
    static constexpr auto size() -> int
    { return 3; }
    static constexpr auto typeName() -> const char*
    { return "StaysOnTop"; }
    static constexpr auto typeKey() -> const char*
    { return "stays-on-top"; }
    static auto typeDescription() -> QString
    { return qApp->translate("EnumInfo", "Stays on Top"); }
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
        case Enum::Playing: return qApp->translate("EnumInfo", "Playing");
        case Enum::Always: return qApp->translate("EnumInfo", "Always");
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
    { return StaysOnTop::Playing; }
private:
    static const ItemList info;
};

using StaysOnTopInfo = EnumInfo<StaysOnTop>;

#endif
