#include "asciidialog.h"
#include "asciiservice.h"
#include <QChar>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>

AsciiDialog::AsciiDialog(const std::vector<int>& data, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("ASCII-конвертация");
    resize(400, 300);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Число → Символ по таблице ASCII:"));

    m_listWidget = new QListWidget;
    m_listWidget->setAlternatingRowColors(true);
    layout->addWidget(m_listWidget);

    for (size_t i = 0; i < data.size(); ++i) {
        int val = data[i];
        m_listWidget->addItem(QString("%1 → %2")
                                  .arg(i, 3)
                                  .arg(AsciiService::describe(val)));
    }
}