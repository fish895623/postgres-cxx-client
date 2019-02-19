#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <type_traits>
#include <postgres/internal/Classifier.h>
#include <postgres/Result.h>
#include <postgres/Transaction.h>
#include <postgres/PrepareData.h>
#include <postgres/Connection.h>
#include <postgres/Statement.h>

namespace postgres {

class Config;
class Command;
class PreparedCommand;

class Client {
public:
    explicit Client();
    explicit Client(Config const& config);
    Client(Client const&) = delete;
    Client(Client&&);
    Client& operator=(Client const&) = delete;
    Client& operator=(Client&&);
    ~Client();

    Transaction begin();

    template <typename... Ts>
    std::enable_if_t<(sizeof... (Ts) > 1), Result> execute(Ts const& ... statements) {
        return completeTransaction(exec("BEGIN", statements...));
    }

    template <typename T>
    Result execute(T const& statement) {
        return exec(statement);
    }

    Connection& connection();

private:
    template <typename T, typename... Ts>
    std::enable_if_t<(sizeof... (Ts) > 0), Result>
    exec(T const& statement, Ts const& ... statements) {
        auto res = exec(statement);
        if (res.isOk()) {
            return exec(statements...);
        }
        return res;
    };

    Result exec(PreparedCommand const& cmd);
    Result exec(Command const& cmd);
    Result completeTransaction(Result res);

    Connection conn_;
};

}  // namespace postgres
