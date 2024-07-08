#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>
#include <sqlpp11/sqlite3/sqlite3.h>
#include <QDebug>

namespace spdlog
{
namespace sinks
{


std::string level_to_str(spdlog::level::level_enum level)
{
    switch (level)
    {
        case level::info: return "info";
        case level::debug: return "debug";
        case level::warn: return "warn";
        case level::err: return "err";
        case level::critical: return "critical";
        default: return "Unknown";
    }
}


enum sql_enum
{
    CreateTable = 0,
    SelectTable,
    FindTable,
    InsertLog,
};

std::unordered_map<int, std::string> sqls
{
    {CreateTable, "CREATE TABLE \"logs\" (\"Id\" INTEGER PRIMARY KEY AUTOINCREMENT, \"TimeStamp\" TEXT NOT NULL, \"Level\" TEXT NOT NULL, \"Message\" TEXT NOT NULL, \"LoggerName\" TEXT NOT NULL, \"ThreadId\" INTEGER NOT NULL)"},
    {SelectTable, "select name from sqlite_master where type=\"table\""},
    {FindTable, "select name from sqlite_master where type=\"table\" and name=\"logs\""},
    {InsertLog, "INSERT INTO logs(TimeStamp,Level,Message,LoggerName,ThreadId) VALUES(CURRENT_TIMESTAMP,?,?,?,?)"},
};

bool createTable(sqlite3* db)
{
    if (!db)
        return false;
    char *msg{nullptr};
    int ret = sqlite3_exec(db, sqls[CreateTable].c_str(), nullptr, nullptr, &msg);
    if (ret != SQLITE_OK)
        spdlog::error(msg);
    if (msg)
        sqlite3_free(msg);
    return ret == SQLITE_OK;
}

std::vector<std::string> getTables(sqlite3* db)
{
    std::vector<std::string> tables;
    if (!db)
        return tables;
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sqls[SelectTable].c_str(), -1, &stmt, NULL);
    if(rc != SQLITE_OK)
        return tables;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string table = (char*)sqlite3_column_text(stmt, 0);
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
    int rc = sqlite3_prepare_v2(db, sqls[FindTable].c_str(), -1, &stmt, NULL);
    if(rc != SQLITE_OK)
        return found;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string table = (char*)sqlite3_column_text(stmt, 0);
        if (table == "logs")
            found = true;
    }
    sqlite3_finalize(stmt);
    return found;
}


template<typename Mutex>
class sqlite_sink : public spdlog::sinks::base_sink<Mutex>
{
public:
    explicit sqlite_sink(const std::string& databaseName)
    {
        if (sqlite3_open_v2(databaseName.c_str(), &database_, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, nullptr)  != SQLITE_OK)
        {
            std::cerr << __FUNCTION__ << " " << __LINE__ << "Error opening database" << std::endl;
            throw spdlog_ex("Error opening database");
        }
        bool found = foundTable(database_);
        std::cout << "found table: " << found << std::endl;
        if (!found)
            createTable(database_);
        if (sqlite3_prepare_v2(database_, sqls[InsertLog].c_str(), -1, &stmt_, nullptr) != SQLITE_OK)
            throw spdlog_ex(sqlite3_errmsg(database_));
    }

    ~sqlite_sink()
    {
        sqlite3_close_v2(database_);
        sqlite3_finalize(stmt_);
        database_ = nullptr;
        stmt_ = nullptr;
    }

protected:
    void sink_it_(const details::log_msg &msg) override
    {
        if (!bind_to_statement(msg))
            throw spdlog_ex(sqlite3_errmsg(database_));
        if (sqlite3_step(stmt_) != SQLITE_DONE)
            throw spdlog_ex(sqlite3_errmsg(database_));
        sqlite3_reset(stmt_);
        sqlite3_clear_bindings(stmt_);
    }

    void flush_() override {}

private:
    sqlite3 *database_{nullptr};
    sqlite3_stmt * stmt_{nullptr};

    bool bind_to_statement(const details::log_msg& msg) const
    {
        if (sqlite3_bind_text(stmt_, 1, level_to_str(msg.level).c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
        {
            std::cerr << __FUNCTION__ << " " << __LINE__  << " 1" << std::endl;
            return false;
        }

        if (sqlite3_bind_text(stmt_, 2,  msg.payload.data(),  msg.payload.size(), SQLITE_STATIC) != SQLITE_OK)
        {
            std::cerr << __FUNCTION__ << " " << __LINE__  << " 2" << std::endl;
            return false;
        }
        if (sqlite3_bind_text(stmt_, 3, msg.logger_name.data(), msg.logger_name.size(), SQLITE_STATIC) != SQLITE_OK)
        {
            std::cerr << __FUNCTION__ << " " << __LINE__  << " 3" << std::endl;
            return false;
        }
        if (sqlite3_bind_int(stmt_, 4, static_cast<int>(msg.thread_id)) != SQLITE_OK)
        {
            std::cerr << __FUNCTION__ << " " << __LINE__  << " 4" << std::endl;
            return false;
        }
        return true;
    }
};


using sqlite_sink_mt = sqlite_sink<std::mutex>;
using sqlite_sink_st = sqlite_sink<spdlog::details::null_mutex>;

}
}

