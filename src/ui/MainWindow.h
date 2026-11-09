#pragma once

#include <QMainWindow>

class TraceTableModel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void openTrace();
    void loadTrace(const QString &path);

    TraceTableModel *m_model;
};
