#ifndef ASCIIDIALOG_H
#define ASCIIDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <vector>

class AsciiDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AsciiDialog(const std::vector<int>& sourceData, QWidget *parent = nullptr);

private:
    void populateAsciiList();
    QString charDescription(int value) const;

    QListWidget* asciiList;
    std::vector<int> sourceData;
};

#endif // ASCIIDIALOG_H