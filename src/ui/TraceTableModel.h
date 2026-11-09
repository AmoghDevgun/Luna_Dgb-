#pragma once

#include <QAbstractTableModel>
#include <QString>
#include <QVector>

class TraceTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    struct Row {
        int seq;
        quint64 pc;
        QString op;
        QString args;
    };

    enum Column { SeqColumn, PcColumn, OpColumn, ArgsColumn, ColumnCount };

    explicit TraceTableModel(QVector<Row> rows, QObject *parent = nullptr);

    void setRows(QVector<Row> rows);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
        
private:
    QVector<Row> m_rows;
};