#pragma once

#include <QString>
#include <QObject>

namespace quicktools::core {

class InputParams;
class OutputParams;

class RuntimeParams : public QObject
{
public:
    RuntimeParams(QObject *parent = nullptr)
        : QObject(parent)
    {

    }

    virtual ~RuntimeParams()
    {

    }

    virtual bool getInput(InputParams *input_params)    = 0;
    virtual bool genOutput(OutputParams *output_params) = 0;

    virtual void reset()
    {

    }

    void setError(const int error, const QString &msg = QString())
    {
        error_ = error;
        msg_ = msg;
    }

    std::tuple<int, QString> returnError() const
    {
        return {error_, msg_};
    }

private:
    int error_{0};
    QString msg_;
};

} // namespace quicktools::core
