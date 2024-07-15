#pragma once

#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/spdlog.h>
#include <sqlpp11/sqlite3/sqlite3.h>

namespace spdlog { namespace sinks {

enum sql_enum
{
    CreateTable = 0,
    SelectTable,
    FindTable,
    InsertLog,
};

// clang-format off
std::unordered_map<int, std::string> sqls{
    {CreateTable, "CREATE TABLE \"logs\" (\"Id\" INTEGER PRIMARY KEY AUTOINCREMENT, \"TimeStamp\" TEXT NOT NULL, \"Level\" TEXT NOT NULL, \"Message\" TEXT NOT NULL, \"LoggerName\" TEXT NOT NULL, \"ThreadId\" INTEGER NOT NULL)"},
    {SelectTable, "SELECT name FROM sqlite_master WHERE type=\"table\""},
    {  FindTable, "SELECT name FROM sqlite_master WHERE type=\"table\" and name=\"logs\""},
    {  InsertLog, "INSERT INTO logs(TimeStamp,Level,Message,LoggerName,ThreadId) VALUES(CURRENT_TIMESTAMP,?,?,?,?)"},
};
// clang-format on

bool createTable(sqlite3 *db)
{
    if (!db)
        return false;
    char *msg{nullptr};
    int   ret = sqlite3_exec(db, sqls[CreateTable].c_str(), nullptr, nullptr, &msg);
    if (ret != SQLITE_OK)
        spdlog::error(msg);
    if (msg)
        sqlite3_free(msg);
    return ret == SQLITE_OK;
}

std::vector<std::string> getTables(sqlite3 *db)
{
    std::vector<std::string> tables;
    if (!db)
        return tables;
    sqlite3_stmt *stmt = NULL;
    int           rc   = sqlite3_prepare_v2(db, sqls[SelectTable].c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
        return tables;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string table = (char *)sqlite3_column_text(stmt, 0);
        tables.emplace_back(table);
    }
    sqlite3_finalize(stmt);
    return tables;
}

bool foundTable(sqlite3 *db)
{
    bool found{false};
    if (!db)
        return found;
    sqlite3_stmt *stmt = NULL;
    int           rc   = sqlite3_prepare_v2(db, sqls[FindTable].c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
        return found;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string table = (char *)sqlite3_column_text(stmt, 0);
        if (table == "logs")
            found = true;
    }
    sqlite3_finalize(stmt);
    return found;
}

template<typename Mutex, typename FileNameCalc = daily_filename_calculator>
class sqlite_sink : public spdlog::sinks::base_sink<Mutex>
{
public:
    explicit sqlite_sink(const std::string &base_filename, int rotation_hour, int rotation_minute)
        : base_filename_(base_filename)
        , rotation_h_(rotation_hour)
        , rotation_m_(rotation_minute)
    {
        auto              now      = log_clock::now();
        const std::string filename = FileNameCalc::calc_filename(base_filename, now_tm(now));
        open(filename);
        prepare();
    }

    ~sqlite_sink()
    {
        close();
    }

protected:
    void sink_it_(const details::log_msg &msg) override
    {
        auto time          = msg.time;
        bool should_rotate = time >= rotation_tp_;
        if (should_rotate)
        {
            auto filename = FileNameCalc::calc_filename(base_filename_, now_tm(time));
            reopen(filename);
            prepare();
            rotation_tp_ = next_rotation_tp_();
        }
        if (!bind_to_statement(msg))
            throw spdlog_ex(sqlite3_errmsg(database_));
        if (sqlite3_step(stmt_) != SQLITE_DONE)
            throw spdlog_ex(sqlite3_errmsg(database_));
        sqlite3_reset(stmt_);
        sqlite3_clear_bindings(stmt_);
    }

    void flush_() override {}

private:
    sqlite3              *database_{nullptr};
    sqlite3_stmt         *stmt_{nullptr};
    std::string           base_filename_;
    int                   rotation_h_;
    int                   rotation_m_;
    log_clock::time_point rotation_tp_;

    tm now_tm(log_clock::time_point tp)
    {
        time_t tnow = log_clock::to_time_t(tp);
        return spdlog::details::os::localtime(tnow);
    }

    log_clock::time_point next_rotation_tp_()
    {
        auto now           = log_clock::now();
        tm   date          = now_tm(now);
        date.tm_hour       = rotation_h_;
        date.tm_min        = rotation_m_;
        date.tm_sec        = 0;
        auto rotation_time = log_clock::from_time_t(std::mktime(&date));
        if (rotation_time > now)
        {
            return rotation_time;
        }
        return {rotation_time + std::chrono::hours(24)};
    }

    bool bind_to_statement(const details::log_msg &msg)
    {
        if (sqlite3_bind_text(stmt_, 1, level_to_str(msg.level).c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
            return false;

        // std::cout << __FUNCTION__ << " " << __LINE__ << " " << std::string( msg.payload.data(), msg.payload.size()) << " size: " << msg.payload.size() << std::endl;
        if (sqlite3_bind_text(stmt_, 2, msg.payload.data(), static_cast<int>(msg.payload.size()), SQLITE_STATIC) != SQLITE_OK)
            return false;

        if (sqlite3_bind_text(stmt_, 3, msg.logger_name.data(), static_cast<int>(msg.logger_name.size()), SQLITE_STATIC) != SQLITE_OK)
            return false;

        if (sqlite3_bind_int(stmt_, 4, static_cast<int>(msg.thread_id)) != SQLITE_OK)
            return false;

        return true;
    }

    void close()
    {
        sqlite3_close_v2(database_);
        sqlite3_finalize(stmt_);
        database_ = nullptr;
        stmt_     = nullptr;
    }

    void open(const std::string &filename)
    {
        close();
        spdlog::details::os::create_dir(spdlog::details::os::dir_name(filename));
        if (sqlite3_open_v2(filename.c_str(), &database_, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, nullptr)
            != SQLITE_OK)
            throw spdlog_ex("Error opening database");
    }

    void reopen(const std::string &filename)
    {
        this->open(filename);
    }

    void prepare()
    {
        if (!foundTable(database_))
            createTable(database_);
        if (sqlite3_prepare_v2(database_, sqls[InsertLog].c_str(), -1, &stmt_, nullptr) != SQLITE_OK)
            throw spdlog_ex(sqlite3_errmsg(database_));
    }

    std::string level_to_str(spdlog::level::level_enum level)
    {
        switch (level)
        {
        case level::info:
            return "info";
        case level::debug:
            return "debug";
        case level::warn:
            return "warn";
        case level::err:
            return "err";
        case level::critical:
            return "critical";
        default:
            return "Unknown";
        }
    }
};

using sqlite_sink_mt = sqlite_sink<std::mutex>;
using sqlite_sink_st = sqlite_sink<spdlog::details::null_mutex>;

}} // namespace spdlog::sinks
