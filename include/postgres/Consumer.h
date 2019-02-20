#pragma once

#include <memory>
#include <libpq-fe.h>

namespace postgres {

class Client;
class Status;

class Consumer {
    friend class Client;

public:
    Consumer(Consumer const& other) = delete;
    Consumer& operator=(Consumer const& other) = delete;
    Consumer(Consumer&& other) noexcept;
    Consumer& operator=(Consumer&& other) noexcept;
    ~Consumer() noexcept;

    Status consume();
    bool isOk();
    bool isBusy();

protected:
    explicit Consumer(std::shared_ptr<PGconn> handle, int is_ok);

    std::shared_ptr<PGconn> handle_;
    bool                    is_ok_ = false;
};

}  // namespace postgres
