#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <QList>
#include "condition.h"

class FeedBack
{
public:
   explicit FeedBack(QList<Condition> conds);
    QList<QString> getCriticalMessages() const;

   QList<QString> getInfoMessages() const;

    QList<QString> getAlerts() const;

private:
   QList<QString> criticalMessages;
   QList<QString> infoMessages;
   QList<QString> alerts;

   void processConditions(QList<Condition> &conds);
};

#endif // FEEDBACK_H
