#ifndef ASCIIDIALOG_H
#define ASCIIDIALOG_H

#include <QDialog>
#include <vector>

class QListWidget;

class AsciiDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AsciiDialog(const std::vector<int>& data, QWidget *parent = nullptr);

private:
    QListWidget* m_listWidget;
};

#endif // ASCIIDIALOG_H