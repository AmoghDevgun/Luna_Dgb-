#include "TraceTableModel.h"

TraceTableModel::TraceTableModel(QVector<Row> rows, QObject *parent)
    : QAbstractTableModel(parent) {
    m_rows = rows;
}

int TraceTableModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : int(m_rows.size());
}

int TraceTableModel::columnCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : ColumnCount;
}

QVariant TraceTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_rows.size())
        return {};

    if (role != Qt::DisplayRole)
        return {};

    const Row &row = m_rows.at(index.row());

    switch (index.column()) {
    case SeqColumn:
        return row.seq;
    case PcColumn:
        return QString::number(row.pc);
    case OpColumn:
        return row.op;
    case ArgsColumn:
        return row.args;
    default:
        return {};
    }
}

QVariant TraceTableModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const {
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case SeqColumn:
            return QStringLiteral("seq");
        case PcColumn:
            return QStringLiteral("pc");
        case OpColumn:
            return QStringLiteral("op");
        case ArgsColumn:
            return QStringLiteral("args");
        default:
            return {};
        }
    }

    return section + 1;
}

void TraceTableModel::setRows(QVector<Row> rows) {
    beginResetModel();
    m_rows = rows;
    endResetModel();
}
