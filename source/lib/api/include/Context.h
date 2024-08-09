#pragma once

namespace api
{
    class Context
    {
    public:
        Context(Context const &) = delete;
        Context(Context &&) = delete;
        Context &operator=(Context const &) = delete;
        Context &operator=(Context &&) = delete;
    };
}
