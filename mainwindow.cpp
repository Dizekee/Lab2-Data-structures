#include "mainwindow.h"
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QListWidget>
#include <QLabel>
#include <QMessageBox>
#include <QStringList>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentType(ARRAY)
{
    ui->setupUi(this);

    // Заполнение выбора структур
    ui->structureCombo->addItem("Array", ARRAY);
    ui->structureCombo->addItem("Vector", VECTOR);
    ui->structureCombo->addItem("Stack", STACK);
    ui->structureCombo->addItem("Queue", QUEUE);

    // ПОДСКАЗКИ ДЛЯ ПОЛЕЙ ВВОДА (для удобства)
    ui->valueLineEdit->setPlaceholderText("Введите число для вставки");
    ui->indexLineEdit->setPlaceholderText("Введите индекс элемента");
    ui->newValueLineEdit->setPlaceholderText("Новое значение");
    ui->searchValueLineEdit->setPlaceholderText("Число для поиска");

    // Подключение сигналов
    connect(ui->structureCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onStructureChanged);
    connect(ui->insertButton, &QPushButton::clicked, this, &MainWindow::onInsertClicked);
    connect(ui->removeLastButton, &QPushButton::clicked, this, &MainWindow::onRemoveLastClicked);
    connect(ui->replaceButton, &QPushButton::clicked, this, &MainWindow::onReplaceClicked);

    // НОВЫЕ КНОПКИ: поиск и сортировка (добавьте их в UI-файл)
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(ui->sortButton, &QPushButton::clicked, this, &MainWindow::onSortClicked);

    // Начальная инициализация
    onStructureChanged(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
        QMessageBox::warning(this, "Ошибка", "Array достиг максимального размера (100).");
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
        QMessageBox::warning(this, "Ошибка", QString("Индекс %1 вне диапазона (0..%2)")
                                                 .arg(idx).arg(data.size() - 1));
        return;
    }

    data[idx] = newVal;
    updateDisplay();
    ui->indexLineEdit->clear();
    ui->newValueLineEdit->clear();
    ui->indexLineEdit->setFocus();
}

// ========== ПОИСК ==========
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
    } else {
        ui->searchResultLabel->setText("Элемент не найден");
    }
    ui->searchValueLineEdit->clear();
}

// ========== СОРТИРОВКА ==========
void MainWindow::onSortClicked()
{
    if (data.size() < 2) {
        QMessageBox::information(this, "Сортировка", "Недостаточно элементов для сортировки (нужно хотя бы 2).");
        return;
    }
    shellSort(data);
    updateDisplay();
}

// ------------------------------------------------------------------
// Вспомогательные методы
// ------------------------------------------------------------------

void MainWindow::updateDisplay()
{
    ui->displayListWidget->clear();
    for (size_t i = 0; i < data.size(); ++i) {
        QString itemText = QString("%1 → %2").arg(i).arg(data[i]);
        ui->displayListWidget->addItem(itemText);
    }
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