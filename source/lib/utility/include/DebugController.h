#pragma once

#include "lib/infrastructure/include/ParameterSupplier.h"

#include "lib/utility/include/Utility.h"

#include <string>
#include <any>
#include <map>
#include <numeric>
#include <functional>
#include <stdexcept>

namespace utility
{

    /* Simple key value (any type) store to bring various tweaks into different
       implementations of detectors, decoders, whatever...
     */
    class DebugController : public infrastructure::ParameterSupplier
    {
        class Tweak
        {
            std::any minimum;
            std::any value;
            std::any maximum;
            std::string shortIdent;
            mutable unsigned int touchCount = 0;

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
                touchCount++;
                return std::any_cast<T>(value);
            }

            template <typename T>
            T incrementAs()
            {
                touchCount = 0;
                return ::utility::safeIncrement(*std::any_cast<T>(&value), std::any_cast<T>(maximum));
            }

            template <typename T>
            T incrementRotateAs(T incrementValue)
            {
                touchCount = 0;
                return ::utility::rotate(*std::any_cast<T>(&value), incrementValue, std::any_cast<T>(minimum), std::any_cast<T>(maximum));
            }

            template <typename T>
            T decrementAs()
            {
                touchCount = 0;
                return ::utility::safeDecrement(*std::any_cast<T>(&value), std::any_cast<T>(minimum));
            }

            bool toggle()
            {
                touchCount = 0;
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

            bool touched() const { return touchCount < 2; }
        };

        std::map<std::string, Tweak> settings;

    public:
        DebugController() = default;
        DebugController(DebugController &) = delete;
        DebugController(DebugController &&) = default;
        DebugController &operator=(DebugController &) = delete;
        DebugController &operator=(DebugController &&) = default;

        bool touched() const
        {
            return settings.end() != std::find_if(std::begin(settings), std::end(settings), [&](auto const &setting)
                                                  { return setting.second.touched(); });
        }

        DebugController &define(std::string key, Tweak value)
        {
            settings.emplace(key, std::move(value));
            return *this;
        }

        ParameterTypeList supplyParameters() const
        {
            return std::accumulate(std::begin(settings), std::end(settings), ParameterTypeList{}, [](auto &&list, auto const &setting)
                                   { list.emplace_back(std::make_pair("dbg " + setting.second.getShortIdent(), setting.second.toString()));
                                     return list; });
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
        T handleAs(std::string key, std::function<T(Tweak &)> handler)
        {
            auto item = settings.find(key);
            if (item == std::end(settings))
            {
                throw std::runtime_error(std::string("Key not found and no default value provided: ") + key);
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
        T getAs(std::string key)
        {
            return handleAs<T>(key, [](Tweak &tweak)
                               { return tweak.getAs<T>(); });
        }

        template <typename T>
        T incrementAs(std::string key)
        {
            return handleAs<T>(key, [](Tweak &tweak)
                               { return tweak.incrementAs<T>(); });
        }

        template <typename T>
        T incrementRotateAs(std::string key, T incrementValue)
        {
            return handleAs<T>(key, [=](Tweak &tweak)
                               { return tweak.incrementRotateAs<T>(incrementValue); });
        }

        template <typename T>
        T decrementAs(std::string key)
        {
            return handleAs<T>(key, [](Tweak &tweak)
                               { return tweak.decrementAs<T>(); });
        }

        bool toggle(std::string key)
        {
            return handleAs<bool>(key, [](Tweak &tweak)
                                  { return tweak.toggle(); });
        }
    };
}
