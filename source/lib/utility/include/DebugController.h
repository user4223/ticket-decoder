#pragma once

#include "lib/utility/include/Utility.h"

#include <string>
#include <any>
#include <map>
#include <algorithm>

namespace utility
{

    /* Simple key value (any type) store to bring various tweaks into different
       implementations of detectors, decoders, whatever...
     */
    class DebugController
    {
        class Tweak
        {
            std::any minimum;
            std::any value;
            std::any maximum;
            std::string shortIdent;

        public:
            template <typename T>
            Tweak(T min, T v, T max, std::string si)
                : minimum(std::move(min)), value(std::move(v)), maximum(std::move(max)), shortIdent(si)
            {
            }

            Tweak(bool v, std::string si)
                : minimum(false), value(std::move(v)), maximum(true), shortIdent(si)
            {
            }

            std::string getShortIdent() const
            {
                return shortIdent;
            }

            template <typename T>
            T getAs() const
            {
                return std::any_cast<T>(value);
            }

            template <typename T>
            T incrementAs()
            {
                return ::utility::safeIncrement(*std::any_cast<T>(&value), std::any_cast<T>(maximum));
            }

            template <typename T>
            T incrementRotateAs(T incrementValue)
            {
                return ::utility::rotate(*std::any_cast<T>(&value), incrementValue, std::any_cast<T>(minimum), std::any_cast<T>(maximum));
            }

            template <typename T>
            T decrementAs()
            {
                return ::utility::safeDecrement(*std::any_cast<T>(&value), std::any_cast<T>(minimum));
            }

            bool toggle()
            {
                bool &v = *std::any_cast<bool>(&value);
                v = !v;
                return v;
            }

            std::string toString() const
            {
                if (value.type() == typeid(int))
                {
                    return std::to_string(std::any_cast<int>(value));
                }
                else if (value.type() == typeid(unsigned int))
                {
                    return std::to_string(std::any_cast<unsigned int>(value));
                }
                else if (value.type() == typeid(bool))
                {
                    return std::to_string(std::any_cast<bool>(value));
                }
                return "fix type handling";
            }
        };

        std::map<std::string, Tweak> settings;

    public:
        DebugController()
        {
        }

        DebugController &define(std::string key, Tweak value)
        {
            settings.emplace(key, std::move(value));
            return *this;
        }

        template <typename IteratorT>
        void toString(IteratorT inserter) const
        {
            std::for_each(std::begin(settings), std::end(settings), [&](auto const &setting)
                          { *(inserter++) = std::make_pair("dbg " + setting.second.getShortIdent(), setting.second.toString()); });
        }

        template <typename T>
        T handleAs(std::string key, T defaultValue, std::function<T(Tweak &)> handler)
        {
            auto item = settings.find(key);
            if (item == std::end(settings))
            {
                return defaultValue;
            }
            return handler(item->second);
        }

        template <typename T>
        T getAs(std::string key, T defaultValue)
        {
            return handleAs<T>(key, defaultValue, [](Tweak &tweak)
                               { return tweak.getAs<T>(); });
        }

        template <typename T>
        T incrementAs(std::string key, T defaultValue)
        {
            return handleAs<T>(key, defaultValue, [](Tweak &tweak)
                               { return tweak.incrementAs<T>(); });
        }

        template <typename T>
        T incrementRotateAs(std::string key, T defaultValue, T incrementValue)
        {
            return handleAs<T>(key, defaultValue, [=](Tweak &tweak)
                               { return tweak.incrementRotateAs<T>(incrementValue); });
        }

        template <typename T>
        T decrementAs(std::string key, T defaultValue)
        {
            return handleAs<T>(key, defaultValue, [](Tweak &tweak)
                               { return tweak.decrementAs<T>(); });
        }

        bool toggle(std::string key, bool defaultValue)
        {
            return handleAs<bool>(key, defaultValue, [](Tweak &tweak)
                                  { return tweak.toggle(); });
        }
    };
}
