#pragma once

#include "InputElement.h"

#include <vector>
#include <functional>
#include <memory>

namespace io::api
{
    class LoadResult
    {
        struct Internal;
        std::shared_ptr<Internal> internal;

    public:
        LoadResult(std::vector<InputElement> elements);
        LoadResult(std::function<void(std::function<void(InputElement &&)>)>);
        LoadResult(LoadResult const &) = delete;
        LoadResult(LoadResult &&other);
        LoadResult &operator=(LoadResult const &) = delete;
        LoadResult &operator=(LoadResult &&other);

        bool inProgress() const;
        bool hasCompleted() const;

        /* Waits for at least one result element and returns true when there is at least one.
           Returns false when load process has been completed and no element has been found.
         */
        bool waitForElementOrCompletion() const;

        void add(InputElement &&element);

        /* Returns the current count of elements, this might not be the final size of the result in
           case it has been created with loadAsync().
         */
        std::size_t size() const;

        InputElement get(std::size_t index) const;
    };
}
