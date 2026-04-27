#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QElapsedTimer>
#include <QTimer>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStructureChanged(int index);
    void onInsertClicked();
    void onRemoveLastClicked();
    void onReplaceClicked();
    void onSearchClicked();
    void onSortClicked();

    // === НОВЫЕ ФУНКЦИИ ===
    void onAsciiConvertClicked();      // Открывает диалог с ASCII
    void onGenerateDataClicked();      // Генерация больших данных
    void onAnimatedSortClicked();      // Сортировка с анимацией
    void onAnimationStep();            // Шаг анимации

private:
    void updateDisplay(int highlightIndex = -1);
    bool isIndexValid(int idx) const;
    void shellSort(std::vector<int>& arr);
    int findFirstIndex(int value) const;
    QString formatTime(qint64 microseconds) const;

    Ui::MainWindow *ui;
    std::vector<int> data;

    enum DataType { ARRAY, VECTOR, STACK, QUEUE };
    DataType currentType;
    static const int MAX_ARRAY_SIZE = 100;

    // Анимация сортировки
    QTimer* animationTimer = nullptr;
    std::vector<int> sortAnimationData;
    int animGap = 0, animI = 0, animJ = 0, animState = 0;
    QElapsedTimer operationTimer;
};

#endif // MAINWINDOW_H