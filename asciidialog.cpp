#include "asciidialog.h"
#include <QVBoxLayout>
#include <QLabel>

AsciiDialog::AsciiDialog(const std::vector<int>& sourceData, QWidget *parent)
    : QDialog(parent), sourceData(sourceData)
{
    setWindowTitle("ASCII-конвертация");
    resize(400, 300);

    auto* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Число → Символ по таблице ASCII:"));

    asciiList = new QListWidget();
    asciiList->setAlternatingRowColors(true);
    layout->addWidget(asciiList);

    populateAsciiList();
}

void AsciiDialog::populateAsciiList()
{
    asciiList->clear();
    for (size_t i = 0; i < sourceData.size(); ++i) {
        int val = sourceData[i];
        asciiList->addItem(QString("%1 → %2")
                               .arg(i, 3).arg(charDescription(val)));
    }
}

QString AsciiDialog::charDescription(int value) const
{
    if (value < 0 || value > 127)
        return QString("N/A (вне ASCII: %1)").arg(value);

    char ch = static_cast<char>(value);

    if (value >= 32 && value <= 126) {
        // Печатаемые символы: экранируем спецсимволы
        if (ch == '\\') return "'\\\\' (backslash)";
        if (ch == '\'') return "'\\'' (apostrophe)";
        return QString("'%1'").arg(QChar(ch));
    }

    // Непечатаемые управляющие символы
    switch (value) {
    case 0:  return "'\\0' (NULL)";
    case 7:  return "'\\a' (BEL)";
    case 8:  return "'\\b' (BACKSPACE)";
    case 9:  return "'\\t' (TAB)";
    case 10: return "'\\n' (LF)";
    case 11: return "'\\v' (VT)";
    case 12: return "'\\f' (FF)";
    case 13: return "'\\r' (CR)";
    case 27: return "'\\e' (ESC)";
    default: return QString("'?' (CTRL-%1)").arg(QChar(value + 64));
    }
}