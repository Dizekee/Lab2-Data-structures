#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "asciidialog.h"

#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QListWidget>
#include <QLabel>
#include <QMessageBox>
#include <QTime>
#include <cstdlib>

// ============================================================================
// КОНСТРУКТОР / ДЕСТРУКТОР
// ============================================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentType(ARRAY)
{
    ui->setupUi(this);

    this->setStyleSheet(
        // Главное окно
        "QMainWindow {"
        "    background-color: #0a0a0a;"
        "}"

        // Центральный виджет
        "QWidget#centralwidget {"
        "    background-color: #141414;"
        "    border-radius: 12px;"
        "    margin: 3px;"
        "}"

        // Списки (QListWidget)
        "QListWidget {"
        "    background-color: #1a1a1a;"
        "    color: #e0e0e0;"
        "    border: 1px solid #2a2a2a;"
        "    border-radius: 8px;"
        "    padding: 4px;"
        "    font: 10pt 'Segoe UI';"
        "    outline: none;"
        "}"
        "QListWidget::item {"
        "    padding: 6px;"
        "    border-bottom: 1px solid #222222;"
        "}"
        "QListWidget::item:selected {"
        "    background-color: rgba(255, 140, 0, 0.2);"
        "    color: #ffaa55;"
        "    border-left: 3px solid #ff8c00;"
        "}"
        "QListWidget::item:hover:!selected {"
        "    background-color: #202020;"
        "}"

        // Поля ввода (QLineEdit)
        "QLineEdit {"
        "    background-color: #1e1e1e;"
        "    color: #e0e0e0;"
        "    border: 1px solid #333333;"
        "    border-radius: 6px;"
        "    padding: 6px;"
        "    font: 9pt 'Segoe UI';"
        "    selection-background-color: #ff8c00;"
        "}"
        "QLineEdit:focus {"
        "    border: 1px solid #ff8c00;"
        "    background-color: #252525;"
        "}"

        // Кнопки (QPushButton)
        "QPushButton {"
        "    background-color: #1e1e1e;"
        "    color: #cccccc;"
        "    border: 1px solid #333333;"
        "    border-radius: 6px;"
        "    padding: 6px 12px;"
        "    font: 9pt 'Segoe UI';"
        "}"
        "QPushButton:hover {"
        "    background-color: #2a2a2a;"
        "    border-color: #ff8c00;"
        "    color: #ffaa55;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #111111;"
        "    border-color: #ff8c00;"
        "}"

        // Выпадающий список (QComboBox)
        "QComboBox {"
        "    background-color: #1e1e1e;"
        "    color: #e0e0e0;"
        "    border: 1px solid #333333;"
        "    border-radius: 6px;"
        "    padding: 5px;"
        "    font: 9pt 'Segoe UI';"
        "}"
        "QComboBox:hover {"
        "    border-color: #ff8c00;"
        "}"
        "QComboBox::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 20px;"
        "    border-left: 1px solid #333333;"
        "}"
        "QComboBox::down-arrow {"
        "    image: none;"
        "    border-left: 4px solid transparent;"
        "    border-right: 4px solid transparent;"
        "    border-top: 5px solid #ff8c00;"
        "    margin-right: 6px;"
        "}"
        "QComboBox QAbstractItemView {"
        "    background-color: #1a1a1a;"
        "    color: #e0e0e0;"
        "    selection-background-color: #2a2a2a;"
        "    border: 1px solid #ff8c00;"
        "    border-radius: 4px;"
        "}"
        "QComboBox QAbstractItemView::item {"
        "    padding: 5px;"
        "}"
        "QComboBox QAbstractItemView::item:selected {"
        "    background-color: rgba(255, 140, 0, 0.3);"
        "    color: #ffaa55;"
        "}"

        // Метка (QLabel)
        "QLabel {"
        "    color: #aaaaaa;"
        "    font: 9pt 'Segoe UI';"
        "}"
        "QLabel#searchResultLabel {"
        "    color: #ffaa55;"
        "    font-weight: bold;"
        "}"

        // Полоса прокрутки
        "QScrollBar:vertical {"
        "    background: #141414;"
        "    width: 10px;"
        "    border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #333333;"
        "    border-radius: 4px;"
        "    min-height: 20px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #ff8c00;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        );


    ui->structureCombo->addItem("Array", ARRAY);
    ui->structureCombo->addItem("Vector", VECTOR);
    ui->structureCombo->addItem("Stack", STACK);
    ui->structureCombo->addItem("Queue", QUEUE);

    ui->valueLineEdit->setPlaceholderText("Введите число для вставки");
    ui->indexLineEdit->setPlaceholderText("Введите индекс элемента");
    ui->newValueLineEdit->setPlaceholderText("Новое значение");
    ui->searchValueLineEdit->setPlaceholderText("Число для поиска");
    ui->generateCountEdit->setPlaceholderText("Кол-во элементов");

    connect(ui->structureCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onStructureChanged);
    connect(ui->insertButton, &QPushButton::clicked, this, &MainWindow::onInsertClicked);
    connect(ui->removeLastButton, &QPushButton::clicked, this, &MainWindow::onRemoveLastClicked);
    connect(ui->replaceButton, &QPushButton::clicked, this, &MainWindow::onReplaceClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(ui->sortButton, &QPushButton::clicked, this, &MainWindow::onSortClicked);
    connect(ui->asciiButton, &QPushButton::clicked, this, &MainWindow::onAsciiConvertClicked);
    connect(ui->generateButton, &QPushButton::clicked, this, &MainWindow::onGenerateDataClicked);
    connect(ui->clearStructButton, &QPushButton::clicked, this, &MainWindow::onClearStructClicked);

    onStructureChanged(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ============================================================================
// ОСНОВНЫЕ СЛОТЫ
// ============================================================================

void MainWindow::onStructureChanged(int index)
{
    data.clear();
    currentType = static_cast<DataType>(ui->structureCombo->itemData(index).toInt());
    updateDisplay();
}

void MainWindow::onInsertClicked()
{
    bool ok;
    int value = ui->valueLineEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Введите целое число для вставки.");
        return;
    }

    if (currentType == ARRAY && data.size() >= MAX_ARRAY_SIZE) {
        QMessageBox::warning(this, "Ошибка",
                             QString("Array достиг максимального размера (%1).").arg(MAX_ARRAY_SIZE));
        return;
    }

    data.push_back(value);
    updateDisplay();
    ui->valueLineEdit->clear();
    ui->valueLineEdit->setFocus();
}

void MainWindow::onRemoveLastClicked()
{
    if (data.empty()) {
        QMessageBox::warning(this, "Ошибка", "Структура пуста, нечего удалять.");
        return;
    }
    data.pop_back();
    updateDisplay();
}

void MainWindow::onReplaceClicked()
{
    if (data.empty()) {
        QMessageBox::warning(this, "Ошибка", "Структура пуста, нечего заменять.");
        return;
    }

    bool okIdx, okVal;
    int idx = ui->indexLineEdit->text().toInt(&okIdx);
    int newVal = ui->newValueLineEdit->text().toInt(&okVal);

    if (!okIdx || !okVal) {
        QMessageBox::warning(this, "Ошибка", "Введите корректный индекс и новое значение.");
        return;
    }

    if (!isIndexValid(idx)) {
        QMessageBox::warning(this, "Ошибка",
                             QString("Индекс %1 вне диапазона (0..%2)").arg(idx).arg(data.size() - 1));
        return;
    }

    data[idx] = newVal;
    updateDisplay();
    ui->indexLineEdit->clear();
    ui->newValueLineEdit->clear();
    ui->indexLineEdit->setFocus();
}

void MainWindow::onSearchClicked()
{
    bool ok;
    int searchVal = ui->searchValueLineEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Введите целое число для поиска.");
        return;
    }
    int index = findFirstIndex(searchVal);
    if (index != -1) {
        ui->searchResultLabel->setText(QString("Индекс: %1").arg(index));
        updateDisplay(index);
    } else {
        ui->searchResultLabel->setText("Элемент не найден");
    }
    ui->searchValueLineEdit->clear();
}

void MainWindow::onSortClicked()
{
    if (data.size() < 2) {
        QMessageBox::information(this, "Сортировка",
                                 "Недостаточно элементов для сортировки (нужно хотя бы 2).");
        return;
    }

    operationTimer.start();
    shellSort(data);
    updateDisplay();
}

// ============================================================================
// ASCII И ГЕНЕРАЦИЯ
// ============================================================================

void MainWindow::onAsciiConvertClicked()
{
    if (data.empty()) {
        QMessageBox::warning(this, "ASCII", "Нет данных для конвертации.");
        return;
    }

    operationTimer.start();
    ui->asciiDisplayList->clear();

    for (size_t i = 0; i < data.size(); ++i) {
        int val = data[i];
        int asciiCode = ((val % 128) + 128) % 128;
        char ch = static_cast<char>(asciiCode);

        QString display;

        if (asciiCode >= 32 && asciiCode <= 126) {
            if (ch == ' ') {
                display = "' ' (пробел)";
            } else {
                display = QString("'%1'").arg(QChar(ch));
            }
        } else {
            switch (asciiCode) {
            case 0:  display = "NUL"; break;
            case 1:  display = "SOH"; break;
            case 2:  display = "STX"; break;
            case 3:  display = "ETX"; break;
            case 4:  display = "EOT"; break;
            case 5:  display = "ENQ"; break;
            case 6:  display = "ACK"; break;
            case 7:  display = "BEL"; break;
            case 8:  display = "BS";  break;
            case 9:  display = "TAB"; break;
            case 10: display = "LF";  break;
            case 11: display = "VT";  break;
            case 12: display = "FF";  break;
            case 13: display = "CR";  break;
            case 14: display = "SO";  break;
            case 15: display = "SI";  break;
            case 16: display = "DLE"; break;
            case 17: display = "DC1"; break;
            case 18: display = "DC2"; break;
            case 19: display = "DC3"; break;
            case 20: display = "DC4"; break;
            case 21: display = "NAK"; break;
            case 22: display = "SYN"; break;
            case 23: display = "ETB"; break;
            case 24: display = "CAN"; break;
            case 25: display = "EM";  break;
            case 26: display = "SUB"; break;
            case 27: display = "ESC"; break;
            case 28: display = "FS";  break;
            case 29: display = "GS";  break;
            case 30: display = "RS";  break;
            case 31: display = "US";  break;
            case 127:display = "DEL"; break;
            default: display = "-";   break;
            }
        }

        ui->asciiDisplayList->addItem(
            QString("%1 → %2 → %3")
                .arg(i, 3)
                .arg(val, 3)
                .arg(display)
            );
    }

    ui->asciiDisplayList->scrollToTop();
}

void MainWindow::onGenerateDataClicked()
{
    bool ok;
    int count = ui->generateCountEdit->text().toInt(&ok);
    if (!ok || count <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите положительное число элементов.");
        return;
    }
    if (currentType == ARRAY && count > MAX_ARRAY_SIZE) {
        QMessageBox::warning(this, "Ошибка",
                             QString("Array ограничен %1 элементами.").arg(MAX_ARRAY_SIZE));
        return;
    }

    operationTimer.start();
    data.clear();
    data.reserve(count);

    std::srand(static_cast<unsigned>(QTime::currentTime().msec()));
    for (int i = 0; i < count; ++i) {
        data.push_back(std::rand() % 256);
    }
    updateDisplay();
}

// ============================================================================
// ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ
// ============================================================================

void MainWindow::updateDisplay(int highlightIndex)
{
    if (!ui->displayListWidget) return;

    ui->displayListWidget->clear();

    for (size_t i = 0; i < data.size(); ++i) {
        QListWidgetItem* item = new QListWidgetItem(
            QString("%1 → %2").arg(i).arg(data[i]));

        if (static_cast<int>(i) == highlightIndex) {
            item->setBackground(QColor(255, 128, 0));
            item->setForeground(Qt::white);
            item->setFont(QFont("Arial", 10, QFont::Bold));
        }
        ui->displayListWidget->addItem(item);
    }
    ui->displayListWidget->scrollToBottom();

    QString typeStr;
    switch (currentType) {
    case ARRAY:  typeStr = "Array"; break;
    case VECTOR: typeStr = "Vector"; break;
    case STACK:  typeStr = "Stack"; break;
    case QUEUE:  typeStr = "Queue"; break;
    }
    setWindowTitle(QString("Структуры данных — %1 (%2 элементов)")
                       .arg(typeStr).arg(data.size()));
}

bool MainWindow::isIndexValid(int idx) const
{
    return idx >= 0 && idx < static_cast<int>(data.size());
}

int MainWindow::findFirstIndex(int value) const
{
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] == value) return static_cast<int>(i);
    }
    return -1;
}

void MainWindow::shellSort(std::vector<int>& arr)
{
    int n = static_cast<int>(arr.size());
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; ++i) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

QString MainWindow::formatTime(qint64 microseconds) const
{
    if (microseconds < 1000)
        return QString("%1 мкс").arg(microseconds);
    else if (microseconds < 1000000)
        return QString("%1 мс").arg(microseconds / 1000.0, 0, 'f', 2);
    else
        return QString("%1 с").arg(microseconds / 1000000.0, 0, 'f', 3);
}

void MainWindow::onClearStructClicked()
{
    data.clear();
    updateDisplay();
    ui->asciiDisplayList->clear();   // очищаем и ASCII-список, если нужно
}