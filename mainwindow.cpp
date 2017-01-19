#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream> // для использования std::cout
#include <windows.h>

BOOL printExpFinder(HWND wnd, LPARAM lParam)
{
    if (!IsWindowVisible(wnd))
        return 1;

    int length = GetWindowTextLengthA(wnd);
    if (!length)
        return 1;

    length++;

    // статическая
    // динамическая (heap/куча)
    // стек
    char *buffer = (char*)malloc(length); // new
    GetWindowTextA(wnd, buffer, length);
    std::string caption = buffer;
    free(buffer);

    //caption = "PrintExp_139 v.1.4.2. (Dongle not detected)"
    if (caption.find("PrintExp") != std::string::npos) // если в caption есть подстрока "PrintExp"
    {
        // это значит, что wnd - это хэндл нужного нам окна
        HWND * pHwnd = (HWND *)lParam;
        if (pHwnd)
        {
            *pHwnd = wnd;
            return 1;
        }
    }

    return 1;
}

HWND FindPrintExp() // возвращает хэндл окна PrintExp, если оно найдено, либо 0, если не найдено.
{
    HWND hwnd;
    EnumWindows(&printExpFinder, (LPARAM)&hwnd);
    return hwnd;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void Click(int x, int y) // Эмулирует (создает фиктивный клик) по абсолютным координанатам x, y
{
    const double XSCALEFACTOR = 65535 / (GetSystemMetrics(SM_CXSCREEN) - 1);
    const double YSCALEFACTOR = 65535 / (GetSystemMetrics(SM_CYSCREEN) - 1);

    /*POINT cursorPos;
    GetCursorPos(&cursorPos); // Получаем текущие координаты мыши

    double cx = cursorPos.x * XSCALEFACTOR;
    double cy = cursorPos.y * YSCALEFACTOR;*/

    double nx = x * XSCALEFACTOR;
    double ny = y * YSCALEFACTOR;

    INPUT Input={0};
    Input.type = INPUT_MOUSE;
    Input.mi.dx = (LONG)nx; // <=== SendInput принимает координаты в условных единицах от 0 до 65535
    Input.mi.dy = (LONG)ny;
    Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

    SendInput(1,&Input,sizeof(INPUT)); // !!! симуляция клика

    /*Input.mi.dx = (LONG)cx;
    Input.mi.dy = (LONG)cy;
    Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

    SendInput(1,&Input,sizeof(INPUT)); // !!! возврат мыши к старой позиции*/
}

void SimulateClick(HWND hwnd, int x, int y) // симулирует нажатие кнопки мыши по координатам в окне x,y
{
    RECT rect;
    GetWindowRect(hwnd, &rect); // АБСОЛЮТНЫЕ/ЭКРАННЫЕ

    SetForegroundWindow(hwnd); // окно выводит на первый план

    Click(x, y);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);


    hwnd = FindPrintExp();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    // toPlainText возвращает объект типа QString
    numPrints = ui->textEdit->toPlainText().toInt();

    HWND hwnd_ = FindPrintExp();
    SimulateClick(hwnd_, 100, 100);

    //QTimer
}

void MainWindow::on_pushButton_clicked()
{
    int numCycles = ui->textEdit->toPlainText().toInt();
    numCycles++;

    QString q = QString::number(numCycles);
    ui->textEdit->setText(q);
}
