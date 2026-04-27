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
    , animationTimer(nullptr)
{
    ui->setupUi(this);

    // Заполнение выбора структур данных
    ui->structureCombo->addItem("Array", ARRAY);
    ui->structureCombo->addItem("Vector", VECTOR);
    ui->structureCombo->addItem("Stack", STACK);
    ui->structureCombo->addItem("Queue", QUEUE);

    // Подсказки для полей ввода
    ui->valueLineEdit->setPlaceholderText("Введите число для вставки");
    ui->indexLineEdit->setPlaceholderText("Введите индекс элемента");
    ui->newValueLineEdit->setPlaceholderText("Новое значение");
    ui->searchValueLineEdit->setPlaceholderText("Число для поиска");
    ui->generateCountEdit->setPlaceholderText("Кол-во элементов");

    // Подключение сигналов - оригинальные кнопки
    connect(ui->structureCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onStructureChanged);
    connect(ui->insertButton, &QPushButton::clicked, this, &MainWindow::onInsertClicked);
    connect(ui->removeLastButton, &QPushButton::clicked, this, &MainWindow::onRemoveLastClicked);
    connect(ui->replaceButton, &QPushButton::clicked, this, &MainWindow::onReplaceClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(ui->sortButton, &QPushButton::clicked, this, &MainWindow::onSortClicked);

    // Подключение сигналов - новые кнопки (рекомендации)
    connect(ui->asciiButton, &QPushButton::clicked, this, &MainWindow::onAsciiConvertClicked);
    connect(ui->generateButton, &QPushButton::clicked, this, &MainWindow::onGenerateDataClicked);
    connect(ui->animatedSortButton, &QPushButton::clicked, this, &MainWindow::onAnimatedSortClicked);

    // Настройка таймера для анимации сортировки
    animationTimer = new QTimer(this);
    animationTimer->setInterval(50); // 50 мс между шагами анимации
    connect(animationTimer, &QTimer::timeout, this, &MainWindow::onAnimationStep);

    // Начальная инициализация
    onStructureChanged(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ============================================================================
// ОСНОВНЫЕ СЛОТЫ - УПРАВЛЕНИЕ СТРУКТУРАМИ
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

// ============================================================================
// ПОИСК И СОРТИРОВКА
// ============================================================================

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
        updateDisplay(index); // подсветить найденный элемент
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
// НОВЫЕ ФУНКЦИИ (по рекомендациям)
// ============================================================================

// --- 1. ASCII-конвертация в отдельном окне ---
// --- ASCII-конвертация: вывод строки символов в QLineEdit ---
// === ASCII-конвертация: вывод в asciiDisplayList ===
void MainWindow::onAsciiConvertClicked()
{
    if (data.empty()) {
        QMessageBox::warning(this, "ASCII", "Нет данных для конвертации.");
        return;
    }

    operationTimer.start();

    // Очищаем список перед выводом
    ui->asciiDisplayList->clear();

    // Конвертация: берём числа из data, применяем % 128, получаем символ ASCII
    for (size_t i = 0; i < data.size(); ++i) {
        int val = data[i];
        int asciiCode = ((val % 128) + 128) % 128;  // корректный модуль для отрицательных
        char ch = static_cast<char>(asciiCode);

        QString display;

        // Показываем ТОЛЬКО печатаемые символы (коды 32-126)
        if (asciiCode >= 32 && asciiCode <= 126) {
            if (ch == ' ') {
                display = "' ' (пробел)";
            } else {
                display = QString("'%1'").arg(QChar(ch));
            }
        } else {
            // Все управляющие символы (0-31 и 127) — официальные названия
            switch (asciiCode) {
            case 0:  display = "NUL"; break;  // Null
            case 1:  display = "SOH"; break;  // Start of Heading
            case 2:  display = "STX"; break;  // Start of Text
            case 3:  display = "ETX"; break;  // End of Text
            case 4:  display = "EOT"; break;  // End of Transmission
            case 5:  display = "ENQ"; break;  // Enquiry
            case 6:  display = "ACK"; break;  // Acknowledge
            case 7:  display = "BEL"; break;  // Bell
            case 8:  display = "BS";  break;  // Backspace
            case 9:  display = "TAB"; break;  // Horizontal Tab
            case 10: display = "LF";  break;  // Line Feed
            case 11: display = "VT";  break;  // Vertical Tab
            case 12: display = "FF";  break;  // Form Feed
            case 13: display = "CR";  break;  // Carriage Return
            case 14: display = "SO";  break;  // Shift Out
            case 15: display = "SI";  break;  // Shift In
            case 16: display = "DLE"; break;  // Data Link Escape
            case 17: display = "DC1"; break;  // Device Control 1
            case 18: display = "DC2"; break;  // Device Control 2
            case 19: display = "DC3"; break;  // Device Control 3
            case 20: display = "DC4"; break;  // Device Control 4 ← вот он!
            case 21: display = "NAK"; break;  // Negative Acknowledge
            case 22: display = "SYN"; break;  // Synchronous Idle
            case 23: display = "ETB"; break;  // End of Transmission Block
            case 24: display = "CAN"; break;  // Cancel
            case 25: display = "EM";  break;  // End of Medium
            case 26: display = "SUB"; break;  // Substitute
            case 27: display = "ESC"; break;  // Escape
            case 28: display = "FS";  break;  // File Separator
            case 29: display = "GS";  break;  // Group Separator
            case 30: display = "RS";  break;  // Record Separator
            case 31: display = "US";  break;  // Unit Separator
            case 127:display = "DEL"; break;  // Delete
            default: display = "-";           // На всякий случай
            }
        }

        // Добавляем элемент в список: индекс → исходное число → ASCII-символ
        ui->asciiDisplayList->addItem(
            QString("%1 → %2 → %3")
                .arg(i, 3)           // индекс с отступом
                .arg(val, 3)         // исходное число
                .arg(display)        // ASCII-описание
            );
    }

    // Прокручиваем список в начало
    ui->asciiDisplayList->scrollToTop();

}

// --- 2. Генерация больших данных (имитация работы пользователя) ---
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

    // Генерация случайных чисел 0-255 (удобно для демонстрации ASCII)
    std::srand(static_cast<unsigned>(QTime::currentTime().msec()));
    for (int i = 0; i < count; ++i) {
        data.push_back(std::rand() % 256);
    }

    qint64 elapsed = operationTimer.elapsed();
    updateDisplay();

    QMessageBox::information(this, "Готово",
                             QString("Создано %1 элементов.\nВремя генерации: %2")
                                 .arg(count).arg(formatTime(elapsed * 1000)));
}

// --- 3. Анимированная сортировка Шелла ---
void MainWindow::onAnimatedSortClicked()
{
    if (data.size() < 2) {
        QMessageBox::information(this, "Сортировка",
                                 "Нужно хотя бы 2 элемента для сортировки.");
        return;
    }
    if (data.size() > 200) {
        auto reply = QMessageBox::question(this, "Большой массив",
                                           "Анимация " + QString::number(data.size()) +
                                               " элементов может занять время. Продолжить?",
                                           QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) return;
    }

    // Подготовка данных для анимации
    sortAnimationData = data;
    animGap = static_cast<int>(sortAnimationData.size()) / 2;
    animI = animGap;
    animJ = 0;
    animState = 0; // 0=инициализация, 1=сравнение, 2=сдвиг, 3=вставка

    animationTimer->start();
    operationTimer.start();

    ui->timeLabel->setText("Выполняется анимированная сортировка...");
    ui->animatedSortButton->setEnabled(false);
    ui->sortButton->setEnabled(false);
}

void MainWindow::onAnimationStep()
{
    int n = static_cast<int>(sortAnimationData.size());

    // Завершение сортировки
    if (animGap <= 0) {
        animationTimer->stop();
        data = sortAnimationData; // применяем отсортированные данные
        updateDisplay();

        ui->animatedSortButton->setEnabled(true);
        ui->sortButton->setEnabled(true);
        return;
    }

    // Переход к следующему gap
    if (animI >= n) {
        animGap /= 2;
        animI = animGap;
        if (animGap <= 0) return;
    }

    // Подсветка текущего элемента в интерфейсе
    updateDisplay(animI);

    // Пошаговое выполнение алгоритма Шелла
    if (animState == 0) {
        // Инициализация внутреннего цикла
        animJ = animI;
        animState = 1;
    }
    else if (animState == 1) {
        // Сравнение элементов
        if (animJ >= animGap && sortAnimationData[animJ - animGap] > sortAnimationData[animI]) {
            animState = 2; // нужен сдвиг
        } else {
            animState = 3; // вставка
        }
    }
    else if (animState == 2) {
        // Сдвиг элемента
        sortAnimationData[animJ] = sortAnimationData[animJ - animGap];
        updateDisplay(animJ); // визуализация сдвига
        animJ -= animGap;
        animState = 1; // вернуться к сравнению
        return; // ждём следующий таймер
    }
    else if (animState == 3) {
        // Вставка элемента на позицию
        sortAnimationData[animJ] = sortAnimationData[animI];
        animI++;
        animState = 0;
    }
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

        // Подсветка элемента (для анимации или поиска)
        if (static_cast<int>(i) == highlightIndex) {
            item->setBackground(QColor(255, 255, 150));
            item->setFont(QFont("Arial", 10, QFont::Bold));
        }
        ui->displayListWidget->addItem(item);
    }
    ui->displayListWidget->scrollToBottom();

    // Обновление заголовка окна
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