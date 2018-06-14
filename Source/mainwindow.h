#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QDebug>

#include <windows.h>

#include "updatethread.h"
#include "systeminfo.h"
#include "cpuinfo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setPrivileges();
    void setSystemInfo();
    void setAdvancedCPUInfo();
    void start();

    void closeEvent(QCloseEvent*);


public slots:
    void contextMenuWindow(QPoint);
    void contextMenuTable(QPoint);

    void onCPUCounterChanged(int);
    void onMemoryCounterChanged(int);
    void onVirtualMemoryCounterChanged(int);
    void onPagingFileCounterChanged(int);

    void onTotalMemoryCounterChanged(int);
    void onTotalVirtualMemoryCounterChanged(int);
    void onTotalPagingFileCounterChanged(int);

    void onAvailableMemoryCounterChanged(int);
    void onAvailableVirtualMemoryCounterChanged(int);
    void onAvailablePagingFileCounterChanged(int);

    void onRowCountChanged(int);
    void onProcessNameChanged(int, QString);
    void onProcessIDChanged(int, int);
    void onProcessPIDChanged(int, int);
    void onProcessThreadCountChanged(int, int);
    void onProcessPhysicalMemoryUsageChanged(int, int);
    void onProcessPeakPhysicalMemoryUsageChanged(int, int);
    void onProcessPagingFileUsageChanged(int, int);
    void onProcessPeakPagingFileUsageChanged(int, int);

private slots:
    void terminateProcessSlot();
    void terminateParentSlot();
    void exitProgramSlot();

private:
    Ui::MainWindow *ui;

    UpdateThread* thread;
    SystemInfo systemInfo;
    CPUInfo cpuInfo;
};

#endif // MAINWINDOW_H
