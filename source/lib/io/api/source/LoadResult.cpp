#include "../include/LoadResult.h"

#include <mutex>
#include <optional>
#include <future>
#include <thread>

namespace io::api
{
    class LoadResult::Internal
    {
        std::mutex mutex;

    public:
        std::vector<InputElement> elements;
        std::optional<std::future<void>> future;

        auto lock() { return std::lock_guard<std::mutex>(mutex); }
    };

    LoadResult::LoadResult(std::vector<InputElement> e)
        : internal(std::make_shared<Internal>())
    {
        internal->elements = std::move(e);
    }

    LoadResult::LoadResult(std::function<void(std::function<void(InputElement &&)>)> supplier)
        : internal(std::make_shared<Internal>())
    {
        internal->future.emplace(std::async(std::launch::async, [this, supplier]() mutable
                                            { supplier([internal = this->internal](auto &&element)
                                                       {
                                                        auto guard = internal->lock();
                                                        internal->elements.emplace_back(std::move(element)); }); }));
    }

    LoadResult::LoadResult(LoadResult &&other)
        : internal(other.internal)
    {
    }

    LoadResult &LoadResult::operator=(LoadResult &&other)
    {
        internal = other.internal;
        return *this;
    }

    bool LoadResult::inProgress() const
    {
        return !hasCompleted();
    }

    bool LoadResult::hasCompleted() const
    {
        auto guard = internal->lock();
        if (!internal->future.has_value())
        {
            return true; // when there is no future anymore, it has completed for sure ;-)
        }
        auto completed = internal->future->wait_for(std::chrono::seconds(0)) == std::future_status::ready;
        if (completed)
        {
            internal->future->get();
            internal->future.reset();
        }
        return completed;
    }

    bool LoadResult::waitForElementOrCompletion() const
    {
        if (size() > 0)
        {
            return true;
        }
        while (inProgress())
        {
            std::this_thread::yield();
            if (size() > 0)
            {
                return true;
            }
        }
        return size() > 0;
    }

    void LoadResult::add(InputElement &&element)
    {
        auto guard = internal->lock();
        internal->elements.emplace_back(std::move(element));
    }

    std::size_t LoadResult::size() const
    {
        auto guard = internal->lock();
        return internal->elements.size();
    }

    InputElement LoadResult::get(std::size_t index) const
    {
        auto guard = internal->lock();
        if (index >= internal->elements.size())
        {
            throw std::runtime_error("Cannot access element " + std::to_string(index) + " of container with size: " + std::to_string(internal->elements.size()));
        }
        return internal->elements[index];
    }
}
