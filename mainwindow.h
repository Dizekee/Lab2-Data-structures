#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum class ContainerType
{
    Array,
    Vector,
    Stack,
    Queue
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onContainerTypeChanged(int index);
    void onInsertClicked();
    void onRemoveLastClicked();
    void onReplaceClicked();
    void onSearchClicked();
    void onSortClicked();
    void onAsciiConvertClicked();
    void onGenerateDataClicked();
    void onClearDataClicked();

private:
    void updateDisplay(int highlightIndex = -1);
    void applyDarkStyleSheet();

    bool insertValue(int value);
    bool removeLastValue();
    bool replaceValue(int index, int value);
    int  searchFirstValue(int value) const;
    void sortData();
    void clearData();
    void generateRandomData(int count);
    bool isDataEmpty() const;
    int  dataSize() const;
    int  maxSizeAllowed() const;
    const std::vector<int>& getData() const { return m_data; }

    Ui::MainWindow *ui;
    ContainerType m_type;
    std::vector<int> m_data;
    QElapsedTimer m_timer;

    static const int ARRAY_MAX = 100;
};

#endif // MAINWINDOW_H