#include "mainwindow.h"
#include <QtWidgets>          // ← СНАЧАЛА ПОДКЛЮЧАЕМ ВСЕ ВИДЖЕТЫ
#include "ui_mainwindow.h"
#include "asciidialog.h"
#include "asciiservice.h"

#include <QTime>
#include <QDebug>
#include <cstdlib>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_type(ContainerType::Vector)
{
    ui->setupUi(this);
    applyDarkStyleSheet();

    // Заполнение комбобокса типами контейнеров
    ui->structureCombo->addItem("Array", static_cast<int>(ContainerType::Array));
    ui->structureCombo->addItem("Vector", static_cast<int>(ContainerType::Vector));
    ui->structureCombo->addItem("Stack", static_cast<int>(ContainerType::Stack));
    ui->structureCombo->addItem("Queue", static_cast<int>(ContainerType::Queue));

    ui->valueLineEdit->setPlaceholderText("Введите число для вставки");
    ui->indexLineEdit->setPlaceholderText("Введите индекс элемента");
    ui->newValueLineEdit->setPlaceholderText("Новое значение");
    ui->searchValueLineEdit->setPlaceholderText("Число для поиска");
    ui->generateCountEdit->setPlaceholderText("Кол-во элементов");

    connect(ui->structureCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onContainerTypeChanged);
    connect(ui->insertButton, &QPushButton::clicked, this, &MainWindow::onInsertClicked);
    connect(ui->removeLastButton, &QPushButton::clicked, this, &MainWindow::onRemoveLastClicked);
    connect(ui->replaceButton, &QPushButton::clicked, this, &MainWindow::onReplaceClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(ui->sortButton, &QPushButton::clicked, this, &MainWindow::onSortClicked);
    connect(ui->asciiButton, &QPushButton::clicked, this, &MainWindow::onAsciiConvertClicked);
    connect(ui->generateButton, &QPushButton::clicked, this, &MainWindow::onGenerateDataClicked);
    connect(ui->clearStructButton, &QPushButton::clicked, this, &MainWindow::onClearDataClicked);

    setWindowTitle("Структуры данных");
    onContainerTypeChanged(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ---------- Реализация операций с данными ----------
bool MainWindow::insertValue(int value)
{
    if (m_type == ContainerType::Array && static_cast<int>(m_data.size()) >= ARRAY_MAX)
        return false;
    m_data.push_back(value);
    return true;
}

bool MainWindow::removeLastValue()
{
    if (m_data.empty())
        return false;
    m_data.pop_back();
    return true;
}

bool MainWindow::replaceValue(int index, int value)
{
    if (index < 0 || static_cast<size_t>(index) >= m_data.size())
        return false;
    m_data[index] = value;
    return true;
}

int MainWindow::searchFirstValue(int value) const
{
    for (size_t i = 0; i < m_data.size(); ++i)
        if (m_data[i] == value)
            return static_cast<int>(i);
    return -1;
}

void MainWindow::sortData()
{
    std::sort(m_data.begin(), m_data.end());
}

void MainWindow::clearData()
{
    m_data.clear();
}

void MainWindow::generateRandomData(int count)
{
    clearData();
    int limit = maxSizeAllowed();
    if (limit > 0 && count > limit)
        count = limit;
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    for (int i = 0; i < count; ++i)
    {
        int value = std::rand() % 201 - 100;
        insertValue(value);
    }
}

bool MainWindow::isDataEmpty() const
{
    return m_data.empty();
}

int MainWindow::dataSize() const
{
    return static_cast<int>(m_data.size());
}

int MainWindow::maxSizeAllowed() const
{
    switch (m_type)
    {
    case ContainerType::Array: return ARRAY_MAX;
    default: return 1000000; // достаточно большой лимит
    }
}

// ---------- Слоты ----------
void MainWindow::onContainerTypeChanged(int index)
{
    m_type = static_cast<ContainerType>(ui->structureCombo->itemData(index).toInt());
    clearData();
    updateDisplay();
}

void MainWindow::onInsertClicked()
{
    bool ok;
    int value = ui->valueLineEdit->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::warning(this, "Ошибка", "Введите целое число.");
        return;
    }
    if (!insertValue(value))
    {
        QMessageBox::warning(this, "Ошибка",
                             QString("Достигнут максимальный размер для %1.")
                                 .arg(ui->structureCombo->currentText()));
        return;
    }
    updateDisplay();
    ui->valueLineEdit->clear();
    ui->valueLineEdit->setFocus();
}

void MainWindow::onRemoveLastClicked()
{
    if (!removeLastValue())
    {
        QMessageBox::warning(this, "Ошибка", "Структура пуста, нечего удалять.");
        return;
    }
    updateDisplay();
}

void MainWindow::onReplaceClicked()
{
    if (isDataEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Структура пуста.");
        return;
    }
    bool okIdx, okVal;
    int idx = ui->indexLineEdit->text().toInt(&okIdx);
    int newVal = ui->newValueLineEdit->text().toInt(&okVal);
    if (!okIdx || !okVal)
    {
        QMessageBox::warning(this, "Ошибка", "Введите корректный индекс и новое значение.");
        return;
    }
    if (!replaceValue(idx, newVal))
    {
        QMessageBox::warning(this, "Ошибка",
                             QString("Индекс %1 вне диапазона (0..%2)")
                                 .arg(idx).arg(dataSize() - 1));
        return;
    }
    updateDisplay();
    ui->indexLineEdit->clear();
    ui->newValueLineEdit->clear();
    ui->indexLineEdit->setFocus();
}

void MainWindow::onSearchClicked()
{
    bool ok;
    int searchVal = ui->searchValueLineEdit->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::warning(this, "Ошибка", "Введите целое число для поиска.");
        return;
    }
    int index = searchFirstValue(searchVal);
    if (index != -1)
    {
        ui->searchResultLabel->setText(QString("Индекс: %1").arg(index));
        updateDisplay(index);
    }
    else
    {
        ui->searchResultLabel->setText("Элемент не найден");
    }
    ui->searchValueLineEdit->clear();
}

void MainWindow::onSortClicked()
{
    if (dataSize() < 2)
    {
        QMessageBox::information(this, "Сортировка", "Нужно хотя бы 2 элемента.");
        return;
    }
    m_timer.start();
    sortData();
    updateDisplay();
}

void MainWindow::onAsciiConvertClicked()
{
    if (isDataEmpty())
    {
        QMessageBox::warning(this, "ASCII", "Нет данных для конвертации.");
        return;
    }
    ui->asciiDisplayList->clear();
    const auto& data = getData();
    for (size_t i = 0; i < data.size(); ++i)
    {
        int val = data[i];
        QString description = AsciiService::describe(val);
        ui->asciiDisplayList->addItem(
            QString("%1 → %2 → %3")
                .arg(static_cast<int>(i), 3)
                .arg(val, 3)
                .arg(description)
            );
    }
    ui->asciiDisplayList->scrollToTop();
}

void MainWindow::onGenerateDataClicked()
{
    bool ok;
    int count = ui->generateCountEdit->text().toInt(&ok);
    if (!ok || count <= 0)
    {
        QMessageBox::warning(this, "Ошибка", "Введите положительное число.");
        return;
    }
    int maxSize = maxSizeAllowed();
    if (maxSize > 0 && count > maxSize)
    {
        QMessageBox::warning(this, "Ошибка",
                             QString("Максимум элементов для %1: %2")
                                 .arg(ui->structureCombo->currentText())
                                 .arg(maxSize));
        return;
    }
    m_timer.start();
    generateRandomData(count);
    updateDisplay();
}

void MainWindow::onClearDataClicked()
{
    clearData();
    updateDisplay();
    ui->asciiDisplayList->clear();
}

void MainWindow::updateDisplay(int highlightIndex)
{
    ui->displayListWidget->clear();
    const auto& data = getData();
    for (size_t i = 0; i < data.size(); ++i)
    {
        auto item = new QListWidgetItem(QString("%1 → %2").arg(i).arg(data[i]));
        if (static_cast<int>(i) == highlightIndex)
        {
            item->setBackground(QColor(255, 128, 0));
            item->setForeground(Qt::white);
            item->setFont(QFont("Arial", 10, QFont::Bold));
        }
        ui->displayListWidget->addItem(item);
    }
    ui->displayListWidget->scrollToBottom();
}

void MainWindow::applyDarkStyleSheet()
{
    // Ваша существующая стилизация (оставьте как есть)
    this->setStyleSheet(
        "QMainWindow { background-color: #0a0a0a; }"
        "QWidget#centralwidget { background-color: #141414; border-radius: 12px; margin: 3px; }"
        "QListWidget { background-color: #1a1a1a; color: #e0e0e0; border: 1px solid #2a2a2a; border-radius: 8px; padding: 4px; font: 10pt 'Segoe UI'; outline: none; }"
        "QListWidget::item { padding: 6px; border-bottom: 1px solid #222222; }"
        "QListWidget::item:selected { background-color: rgba(255, 140, 0, 0.2); color: #ffaa55; border-left: 3px solid #ff8c00; }"
        "QListWidget::item:hover:!selected { background-color: #202020; }"
        "QLineEdit { background-color: #1e1e1e; color: #e0e0e0; border: 1px solid #333333; border-radius: 6px; padding: 6px; font: 9pt 'Segoe UI'; selection-background-color: #ff8c00; }"
        "QLineEdit:focus { border: 1px solid #ff8c00; background-color: #252525; }"
        "QPushButton { background-color: #1e1e1e; color: #cccccc; border: 1px solid #333333; border-radius: 6px; padding: 6px 12px; font: 9pt 'Segoe UI'; }"
        "QPushButton:hover { background-color: #2a2a2a; border-color: #ff8c00; color: #ffaa55; }"
        "QPushButton:pressed { background-color: #111111; border-color: #ff8c00; }"
        "QComboBox { background-color: #1e1e1e; color: #e0e0e0; border: 1px solid #333333; border-radius: 6px; padding: 5px; font: 9pt 'Segoe UI'; }"
        "QComboBox:hover { border-color: #ff8c00; }"
        "QComboBox::drop-down { subcontrol-origin: padding; subcontrol-position: top right; width: 20px; border-left: 1px solid #333333; }"
        "QComboBox::down-arrow { image: none; border-left: 4px solid transparent; border-right: 4px solid transparent; border-top: 5px solid #ff8c00; margin-right: 6px; }"
        "QComboBox QAbstractItemView { background-color: #1a1a1a; color: #e0e0e0; selection-background-color: #2a2a2a; border: 1px solid #ff8c00; border-radius: 4px; }"
        "QComboBox QAbstractItemView::item { padding: 5px; }"
        "QComboBox QAbstractItemView::item:selected { background-color: rgba(255, 140, 0, 0.3); color: #ffaa55; }"
        "QLabel { color: #aaaaaa; font: 9pt 'Segoe UI'; }"
        "QLabel#searchResultLabel { color: #ffaa55; font-weight: bold; }"
        "QScrollBar:vertical { background: #141414; width: 10px; border-radius: 5px; }"
        "QScrollBar::handle:vertical { background: #333333; border-radius: 4px; min-height: 20px; }"
        "QScrollBar::handle:vertical:hover { background: #ff8c00; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
        );
}