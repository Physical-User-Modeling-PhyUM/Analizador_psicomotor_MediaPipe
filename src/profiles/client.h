#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"
#include "profilerecords.h"


class Client : public User {
public:

    explicit Client(QMap<DataType,QVariant>,QObject *parent = nullptr);
    ~Client() override;
    void assignPlan(int planId, const QDateTime &startDate, const QDateTime &endDate) override;

    int getExpLevel() const;
    void setExpLevel(int newExpLevel);

private:
    int expLevel;
    QHash<BodyRecord,QString> bodyRecords;
    QHash<QString,QString>  LifeStyleRecords;
    QHash<QString,QString>  generalData;
    QHash<QString,QString> MedicalConditions;

};

#endif // CLIENT_H
