#ifndef APPCONFIGWIDGET_H
#define APPCONFIGWIDGET_H

#include <QWidget>
#include "ui_appconfigwidget.h"
#include "core/appcontroller.h"
#include <QSharedPointer>
#include "ui/main/uitablemodel.h"

namespace Ui {
class AppConfigWidget;
}

class AppConfigWidget : public QWidget {
    Q_OBJECT

public:
    explicit AppConfigWidget(QSharedPointer<AppController> controller, QWidget *parent = nullptr);
    ~AppConfigWidget();

private slots:
    void onSaveClicked();

private:
    Ui_AppConfigWidget *ui;
    QSharedPointer<AppController> controller;
    UiTableModel* configModel;

    void loadConfigToForm();
    QHash<QString, QVariant> collectConfigData();
};

#endif // APPCONFIGWIDGET_H
