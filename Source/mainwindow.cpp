#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("ProcessMonitor");

    ui->processTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->processTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuTable(QPoint)));

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuWindow(QPoint)));

    ui->totalMemoryCounterLine->setDisabled(true);
    ui->availableMemoryCounterLine->setDisabled(true);
    ui->totalPagingFileCounterLine->setDisabled(true);
    ui->availablePagingFileCounterLine->setDisabled(true);
    ui->totalVirtualMemoryCounterLine->setDisabled(true);
    ui->availableVirtualMemoryCounterLine->setDisabled(true);

    ui->cpuNameLine->setDisabled(true);
    ui->cpuIDLine->setDisabled(true);
    ui->cpuVendorIDLine->setDisabled(true);
    ui->fpuIDLine->setDisabled(true);
    ui->mbVendorLine->setDisabled(true);
    ui->mbModelLine->setDisabled(true);
    ui->biosVersionLine->setDisabled(true);
    ui->biosDateLine->setDisabled(true);

    ui->frequencyCounterLine->setDisabled(true);
    ui->logicalProcessorsLine->setDisabled(true);
    ui->hyperThreadingLine->setDisabled(true);
    ui->apicLine->setDisabled(true);
    ui->acpiLine->setDisabled(true);
    ui->cacheL1InstructionsLine->setDisabled(true);
    ui->cacheL1DataLine->setDisabled(true);
    ui->cacheL2Line->setDisabled(true);
    ui->cacheL3Line->setDisabled(true);

    ui->frequencyCounterLine->setAlignment(Qt::AlignCenter);
    ui->logicalProcessorsLine->setAlignment(Qt::AlignCenter);
    ui->hyperThreadingLine->setAlignment(Qt::AlignCenter);
    ui->apicLine->setAlignment(Qt::AlignCenter);
    ui->acpiLine->setAlignment(Qt::AlignCenter);
    ui->cacheL1InstructionsLine->setAlignment(Qt::AlignCenter);
    ui->cacheL1DataLine->setAlignment(Qt::AlignCenter);
    ui->cacheL2Line->setAlignment(Qt::AlignCenter);
    ui->cacheL3Line->setAlignment(Qt::AlignCenter);

    ui->processTable->setColumnWidth(0, 120);
    ui->processTable->setColumnWidth(1, 60);
    ui->processTable->setColumnWidth(2, 60);
    ui->processTable->setColumnWidth(3, 60);
    ui->processTable->setColumnWidth(4, 70);
    ui->processTable->setColumnWidth(5, 70);
    ui->processTable->setColumnWidth(6, 70);
    ui->processTable->horizontalHeader()->setStretchLastSection(true);
    ui->processTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->processTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->processTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QFont font = ui->totalMemoryLabel->font();

    font.setPixelSize(12);
    font.setFamily("Times");

    ui->processTable->setFont(font);

    thread = new UpdateThread(this);

    connect(thread, SIGNAL(cpuCounterChanged(int)), this, SLOT(onCPUCounterChanged(int)));
    connect(thread, SIGNAL(memoryCounterChanged(int)), this, SLOT(onMemoryCounterChanged(int)));
    connect(thread, SIGNAL(virtualMemoryCounterChanged(int)), this, SLOT(onVirtualMemoryCounterChanged(int)));
    connect(thread, SIGNAL(pagingFileCounterChanged(int)), this, SLOT(onPagingFileCounterChanged(int)));

    connect(thread, SIGNAL(totalMemoryCounterChanged(int)), this, SLOT(onTotalMemoryCounterChanged(int)));
    connect(thread, SIGNAL(totalVirtualMemoryCounterChanged(int)), this, SLOT(onTotalVirtualMemoryCounterChanged(int)));
    connect(thread, SIGNAL(totalPagingFileCounterChanged(int)), this, SLOT(onTotalPagingFileCounterChanged(int)));

    connect(thread, SIGNAL(availableMemoryCounterChanged(int)), this, SLOT(onAvailableMemoryCounterChanged(int)));
    connect(thread, SIGNAL(availableVirtualMemoryCounterChanged(int)), this, SLOT(onAvailableVirtualMemoryCounterChanged(int)));
    connect(thread, SIGNAL(availablePagingFileCounterChanged(int)), this, SLOT(onAvailablePagingFileCounterChanged(int)));

    connect(thread, SIGNAL(rowCountChanged(int)), this, SLOT(onRowCountChanged(int)));
    connect(thread, SIGNAL(processNameChanged(int, QString)), this, SLOT(onProcessNameChanged(int, QString)));
    connect(thread, SIGNAL(processIDChanged(int, int)), this, SLOT(onProcessIDChanged(int, int)));
    connect(thread, SIGNAL(processPIDChanged(int, int)), this, SLOT(onProcessPIDChanged(int, int)));
    connect(thread, SIGNAL(processThreadCountChanged(int, int)), this, SLOT(onProcessThreadCountChanged(int, int)));
    connect(thread, SIGNAL(processPhysicalMemoryUsageChanged(int, int)), this, SLOT(onProcessPhysicalMemoryUsageChanged(int, int)));
    connect(thread, SIGNAL(processPeakPhysicalMemoryUsageChanged(int, int)), this, SLOT(onProcessPeakPhysicalMemoryUsageChanged(int, int)));
    connect(thread, SIGNAL(processPagingFileUsageChanged(int,int)), this, SLOT(onProcessPagingFileUsageChanged(int,int)));
    connect(thread, SIGNAL(processPeakPagingFileUsageChanged(int,int)), this, SLOT(onProcessPeakPagingFileUsageChanged(int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::contextMenuTable(QPoint pos)
{
    QMenu* menu = new QMenu(this);
    QAction* terminateProcess = new QAction("Terminate", this);
    QAction* terminateParent = new QAction("Terminate parent", this);
    QAction* exit = new QAction("Exit", this);

    connect(terminateProcess, SIGNAL(triggered()), this, SLOT(terminateProcessSlot()));
    connect(terminateParent, SIGNAL(triggered()), this, SLOT(terminateParentSlot()));
    connect(exit, SIGNAL(triggered()), this, SLOT(exitProgramSlot()));

    menu->addAction(terminateProcess);
    menu->addAction(terminateParent);
    menu->addSeparator();
    menu->addAction(exit);

    menu->popup(ui->processTable->viewport()->mapToGlobal(pos));
}

void MainWindow::contextMenuWindow(QPoint)
{
    QMenu* menu = new QMenu(this);
    QAction* exit = new QAction("Exit", this);

    connect(exit, SIGNAL(triggered()), this, SLOT(exitProgramSlot()));

    menu->addAction(exit);

    menu->exec(QCursor::pos());
}

void MainWindow::setPrivileges()
{
   HANDLE currentProcess = GetCurrentProcess();

   TOKEN_PRIVILEGES tokenPrivileges;
   LUID sdv;

   HANDLE hToken;
   OpenProcessToken(currentProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_READ | TOKEN_QUERY, &hToken);

   LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sdv);

   tokenPrivileges.PrivilegeCount = 1;
   tokenPrivileges.Privileges[0].Luid = sdv;
   tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

   AdjustTokenPrivileges(hToken, FALSE, &tokenPrivileges, sizeof(tokenPrivileges), (PTOKEN_PRIVILEGES)NULL, 0);
   CloseHandle(hToken);
   CloseHandle(currentProcess);
}

void MainWindow::setSystemInfo()
{
    ui->cpuNameLine->setText(systemInfo.processorName);
    ui->cpuIDLine->setText(systemInfo.processorID);
    ui->cpuVendorIDLine->setText(systemInfo.processorVendorID);
    ui->fpuIDLine->setText(systemInfo.fpuID);

    ui->mbVendorLine->setText(systemInfo.motherBoardVendor);
    ui->mbModelLine->setText(systemInfo.motherBoardModel);
    ui->biosVersionLine->setText(systemInfo.biosVersion);
    ui->biosDateLine->setText(systemInfo.biosReleaseDate);
}

void MainWindow::setAdvancedCPUInfo()
{
    ui->cpuInstructionsTextBrowser->setText(cpuInfo.getCPUFeatures());
    cpuInfo.getCacheInfo(ui->cpuVendorIDLine->text());

    ui->frequencyCounterLine->setText(cpuInfo.getCPUFrequency());
    ui->logicalProcessorsLine->setText(QString::number(cpuInfo.getNumOfProcessors()));
    if(cpuInfo.htt) ui->hyperThreadingLine->setText("YES");
    else ui->hyperThreadingLine->setText("NO");
    if(cpuInfo.apic) ui->apicLine->setText("YES");
    else ui->apicLine->setText("NO");
    if(cpuInfo.acpi) ui->acpiLine->setText("YES");
    else ui->acpiLine->setText("NO");

    ui->cacheL1InstructionsLine->setText(cpuInfo.cacheL1InstructionsInfo);
    ui->cacheL1DataLine->setText(cpuInfo.cacheL1InstructionsInfo);
    ui->cacheL2Line->setText(cpuInfo.cacheL2Info);
    ui->cacheL3Line->setText(cpuInfo.cacheL3Info);
}

void MainWindow::start()
{
    systemInfo.getSystemInfo();
    setSystemInfo();
    setAdvancedCPUInfo();
    thread->start();
}

void MainWindow::closeEvent(QCloseEvent*)
{
    exitProgramSlot();
}

void MainWindow::onCPUCounterChanged(int newCounter)
{
    ui->PB_CPU->setValue(newCounter);
}

void MainWindow::onMemoryCounterChanged(int newCounter)
{
    ui->PB_RAM->setValue(newCounter);
}

void MainWindow::onVirtualMemoryCounterChanged(int newCounter)
{
    ui->PB_VM->setValue(newCounter);
}

void MainWindow::onPagingFileCounterChanged(int newCounter)
{
    ui->PB_PF->setValue(newCounter);
}

void MainWindow::onTotalMemoryCounterChanged(int newCounter)
{
    ui->totalMemoryCounterLine->setText(QString::number(newCounter));
}

void MainWindow::onTotalVirtualMemoryCounterChanged(int newCounter)
{
    ui->totalVirtualMemoryCounterLine->setText(QString::number(newCounter));
}

void MainWindow::onTotalPagingFileCounterChanged(int newCounter)
{
    ui->totalPagingFileCounterLine->setText(QString::number(newCounter));
}

void MainWindow::onAvailableMemoryCounterChanged(int newCounter)
{
    ui->availableMemoryCounterLine->setText(QString::number(newCounter));
}

void MainWindow::onAvailableVirtualMemoryCounterChanged(int newCounter)
{
    ui->availableVirtualMemoryCounterLine->setText(QString::number(newCounter));
}

void MainWindow::onAvailablePagingFileCounterChanged(int newCounter)
{
    ui->availablePagingFileCounterLine->setText(QString::number(newCounter));
}

void MainWindow::onRowCountChanged(int newCount)
{
    ui->processTable->setRowCount(newCount);
}

void MainWindow::onProcessNameChanged(int row, QString newName)
{
    ui->processTable->setItem(row, 0, new QTableWidgetItem(newName));
}

void MainWindow::onProcessIDChanged(int row, int newID)
{
    QTableWidgetItem* item = new QTableWidgetItem(QString::number(newID));
    item->setTextAlignment(Qt::AlignCenter);
    ui->processTable->setItem(row, 1, item);
}

void MainWindow::onProcessPIDChanged(int row, int newPID)
{
    QTableWidgetItem* item = new QTableWidgetItem(QString::number(newPID));
    item->setTextAlignment(Qt::AlignCenter);
    ui->processTable->setItem(row, 2, item);
}

void MainWindow::onProcessThreadCountChanged(int row, int newThreadCount)
{
    QTableWidgetItem* item = new QTableWidgetItem(QString::number(newThreadCount));
    item->setTextAlignment(Qt::AlignCenter);
    ui->processTable->setItem(row, 3, item);
}

void MainWindow::onProcessPhysicalMemoryUsageChanged(int row, int newPMU)
{
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setTextAlignment(Qt::AlignCenter);

    if(newPMU == -1)
    {
        item->setText("N/A");
        ui->processTable->setItem(row, 4, item);
    }
    else
    {
        item->setText(QString::number(newPMU) + " MB");
        ui->processTable->setItem(row, 4, item);
    }
}

void MainWindow::onProcessPeakPhysicalMemoryUsageChanged(int row, int newPPMU)
{
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setTextAlignment(Qt::AlignCenter);

    if(newPPMU == -1)
    {
        item->setText("N/A");
        ui->processTable->setItem(row, 5, item);
    }
    else
    {
        item->setText(QString::number(newPPMU) + " MB");
        ui->processTable->setItem(row, 5, item);
    }
}

void MainWindow::onProcessPagingFileUsageChanged(int row, int newPFU)
{
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setTextAlignment(Qt::AlignCenter);

    if(newPFU == -1)
    {
        item->setText("N/A");
        ui->processTable->setItem(row, 6, item);
    }
    else
    {
        item->setText(QString::number(newPFU) + " MB");
        ui->processTable->setItem(row, 6, item);
    }
}

void MainWindow::onProcessPeakPagingFileUsageChanged(int row, int newPPFU)
{
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setTextAlignment(Qt::AlignCenter);

    if(newPPFU == -1)
    {
        item->setText("N/A");
        ui->processTable->setItem(row, 7, item);
    }
    else
    {
        item->setText(QString::number(newPPFU) + " MB");
        ui->processTable->setItem(row, 7, item);
    }
}

void MainWindow::terminateProcessSlot()
{
    QTableWidgetItem* item = ui->processTable->item(ui->processTable->currentRow(), 1);
    int pid = item->text().toInt();

    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, pid);

    if(TerminateProcess(hProcess, 0) == 0) QMessageBox::information(this, "Error", "Can't terminate this process!");
}

void MainWindow::terminateParentSlot()
{
    QTableWidgetItem* item = ui->processTable->item(ui->processTable->currentRow(), 2);
    int ppid = item->text().toInt();

    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, ppid);

    if(TerminateProcess(hProcess, 0) == 0) QMessageBox::information(this, "Error", "Can't terminate this processes!");
}

void MainWindow::exitProgramSlot()
{
    thread->close = true;
    Sleep(100);
    exit(0);
}
