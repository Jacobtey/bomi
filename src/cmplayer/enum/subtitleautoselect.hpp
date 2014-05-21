#ifndef SUBTITLEAUTOSELECT_HPP
#define SUBTITLEAUTOSELECT_HPP

#include "enums.hpp"
#define SUBTITLEAUTOSELECT_IS_FLAG 0

enum class SubtitleAutoselect : int {
    Matched = (int)0,
    First = (int)1,
    All = (int)2,
    EachLanguage = (int)3
};

Q_DECLARE_METATYPE(SubtitleAutoselect)

inline auto operator == (SubtitleAutoselect e, int i) -> bool { return (int)e == i; }
inline auto operator != (SubtitleAutoselect e, int i) -> bool { return (int)e != i; }
inline auto operator == (int i, SubtitleAutoselect e) -> bool { return (int)e == i; }
inline auto operator != (int i, SubtitleAutoselect e) -> bool { return (int)e != i; }
inline auto operator > (SubtitleAutoselect e, int i) -> bool { return (int)e > i; }
inline auto operator < (SubtitleAutoselect e, int i) -> bool { return (int)e < i; }
inline auto operator >= (SubtitleAutoselect e, int i) -> bool { return (int)e >= i; }
inline auto operator <= (SubtitleAutoselect e, int i) -> bool { return (int)e <= i; }
inline auto operator > (int i, SubtitleAutoselect e) -> bool { return i > (int)e; }
inline auto operator < (int i, SubtitleAutoselect e) -> bool { return i < (int)e; }
inline auto operator >= (int i, SubtitleAutoselect e) -> bool { return i >= (int)e; }
inline auto operator <= (int i, SubtitleAutoselect e) -> bool { return i <= (int)e; }
#if SUBTITLEAUTOSELECT_IS_FLAG
Q_DECLARE_FLAGS(, SubtitleAutoselect)
Q_DECLARE_OPERATORS_FOR_FLAGS()
Q_DECLARE_METATYPE()
#else
inline auto operator & (SubtitleAutoselect e, int i) -> int { return (int)e & i; }
inline auto operator & (int i, SubtitleAutoselect e) -> int { return (int)e & i; }
inline auto operator &= (int &i, SubtitleAutoselect e) -> int& { return i &= (int)e; }
inline auto operator ~ (SubtitleAutoselect e) -> int { return ~(int)e; }
inline auto operator | (SubtitleAutoselect e, int i) -> int { return (int)e | i; }
inline auto operator | (int i, SubtitleAutoselect e) -> int { return (int)e | i; }
constexpr inline auto operator | (SubtitleAutoselect e1, SubtitleAutoselect e2) -> int { return (int)e1 | (int)e2; }
inline auto operator |= (int &i, SubtitleAutoselect e) -> int& { return i |= (int)e; }
#endif

template<>
class EnumInfo<SubtitleAutoselect> {
    typedef SubtitleAutoselect Enum;
public:
    typedef SubtitleAutoselect type;
    using Data =  QVariant;
    struct Item {
        Enum value;
        QString name, key;
        QVariant data;
    };
    using ItemList = std::array<Item, 4>;
    static constexpr auto size() -> int
    { return 4; }
    static constexpr auto typeName() -> const char*
    { return "SubtitleAutoselect"; }
    static constexpr auto typeKey() -> const char*
    { return ""; }
    static auto typeDescription() -> QString
    { return qApp->translate("EnumInfo", ""); }
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
        case Enum::Matched: return qApp->translate("EnumInfo", "Subtitle which has the same name as that of playing file");
        case Enum::First: return qApp->translate("EnumInfo", "First subtitle from loaded ones");
        case Enum::All: return qApp->translate("EnumInfo", "All loaded subtitles");
        case Enum::EachLanguage: return qApp->translate("EnumInfo", "Each language subtitle");
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
    { return SubtitleAutoselect::Matched; }
private:
    static const ItemList info;
};

using SubtitleAutoselectInfo = EnumInfo<SubtitleAutoselect>;

#endif
