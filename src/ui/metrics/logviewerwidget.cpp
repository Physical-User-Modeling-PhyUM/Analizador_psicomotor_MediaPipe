#include "logviewerwidget.h"

#include "utils/uiutils.h"

LogViewerWidget::LogViewerWidget(QSharedPointer<AppController> controller, QWidget *parent) :
    QWidget(parent),
    ui(new Ui_LogViewerWidget),
    controller(controller)
{
    ui->setupUi(this);

    logModel = new UiTableModel(this);
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(logModel);

    ui->tableView->setModel(proxyModel);
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->resizeColumnsToContents();
    //ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionClicked,
            this, &LogViewerWidget::onHeaderClicked);
}

LogViewerWidget::~LogViewerWidget()
{
    delete ui;
}

void LogViewerWidget::loadSesion(QSharedPointer<TrainingSesion> sesion)
{
    if (!sesion) return;

    currentSesion = sesion;

    auto headers = sesion->getReport().getLogColumnHeaders();
    auto data = sesion->getReport().parseLog();

    logModel->setDataSet(data, headers);
    proxyModel->setSourceModel(logModel);
    ui->tableView->setModel(proxyModel);
}

void LogViewerWidget::onHeaderClicked(int column)
{
    UiUtils::showFilterPopup(this, ui->tableView, proxyModel, column);
}
