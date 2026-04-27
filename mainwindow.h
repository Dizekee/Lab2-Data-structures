#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
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
    void onSearchClicked();     // поиск по значению
    void onSortClicked();       // сортировка Шелла

private:
    void updateDisplay();
    bool isIndexValid(int idx) const;
    void shellSort(std::vector<int>& arr);
    int findFirstIndex(int value) const;

    Ui::MainWindow *ui;
    std::vector<int> data;
    enum DataType { ARRAY, VECTOR, STACK, QUEUE };
    DataType currentType;
    static const int MAX_ARRAY_SIZE = 100;
};

#endif // MAINWINDOW_H