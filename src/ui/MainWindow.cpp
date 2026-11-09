#include "MainWindow.h"

#include "TraceTableModel.h"

#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QTableView>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("lunadbg");
    resize(1200, 800);

    m_model = new TraceTableModel(QVector<TraceTableModel::Row>(), this);

    auto *table = new QTableView(this);
    table->setModel(m_model);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    setCentralWidget(table);

    connect(table->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
            [this](const QModelIndex &current) {
                if (!current.isValid())
                    return;
                statusBar()->showMessage(
                    QStringLiteral("seq %1").arg(
                        m_model->index(current.row(), TraceTableModel::SeqColumn).data().toInt()));
            });

    auto *openAction = new QAction(QStringLiteral("&Open trace..."), this);
    connect(openAction, &QAction::triggered, this, &MainWindow::openTrace);
    menuBar()->addMenu(QStringLiteral("&File"))->addAction(openAction);

    statusBar()->showMessage(QStringLiteral("no trace loaded"));
}

void MainWindow::openTrace() {
    const QString path = QFileDialog::getOpenFileName(
        this, QStringLiteral("Open trace"), QString(),
        QStringLiteral("JSONL (*.jsonl);;All files (*)"));
    if (path.isEmpty())
        return;

    loadTrace(path);
}

void MainWindow::loadTrace(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, QStringLiteral("lunadbg"),
                             QStringLiteral("Cannot open %1").arg(path));
        return;
    }

    QVector<TraceTableModel::Row> rows;
    QTextStream in(&file);

    while (!in.atEnd()) {
        const QByteArray line = in.readLine().toUtf8();
        if (line.trimmed().isEmpty())
            continue;

        const QJsonObject obj = QJsonDocument::fromJson(line).object();
        if (obj.value(QStringLiteral("t")).toString() != QStringLiteral("ins"))
            continue;

        QStringList args;
        const QJsonArray argArray = obj.value(QStringLiteral("args")).toArray();
        for (const QJsonValue &value : argArray)
            args.append(QString::number(value.toInt()));

        rows.append({obj.value(QStringLiteral("seq")).toInt(),
                     quint64(obj.value(QStringLiteral("pc")).toInt()),
                     QString::number(obj.value(QStringLiteral("op")).toInt()),
                     QStringLiteral("[%1]").arg(args.join(QStringLiteral(", ")))});
    }

    m_model->setRows(rows);
    setWindowTitle(QStringLiteral("lunadbg - %1").arg(QFileInfo(path).fileName()));
    statusBar()->showMessage(QStringLiteral("%1 instructions").arg(int(rows.size())));
}
