#include "asciiservice.h"

int AsciiService::toAsciiCode(int value)
{
    // поэтому нормализуем.
    int code = value % 128;
    if (code < 0) code += 128;
    return code;
}

QString AsciiService::describe(int value)
{
    int code = toAsciiCode(value);

    if (code < 0 || code > 127)
        return QString("N/A (вне ASCII: %1)").arg(value);

    char ch = static_cast<char>(code);

    if (code >= 32 && code <= 126) {
        if (ch == '\\') return "'\\\\'";
        if (ch == '\'') return "'\\''";
        return QString("'%1'").arg(QChar(ch));
    }

    switch (code) {
    case 0:  return "'\\0' (NUL)";
    case 1:  return "SOH";
    case 2:  return "STX";
    case 3:  return "ETX";
    case 4:  return "EOT";
    case 5:  return "ENQ";
    case 6:  return "ACK";
    case 7:  return "'\\a' (BEL)";
    case 8:  return "'\\b' (BS)";
    case 9:  return "'\\t' (TAB)";
    case 10: return "'\\n' (LF)";
    case 11: return "'\\v' (VT)";
    case 12: return "'\\f' (FF)";
    case 13: return "'\\r' (CR)";
    case 14: return "SO";
    case 15: return "SI";
    case 16: return "DLE";
    case 17: return "DC1";
    case 18: return "DC2";
    case 19: return "DC3";
    case 20: return "DC4";
    case 21: return "NAK";
    case 22: return "SYN";
    case 23: return "ETB";
    case 24: return "CAN";
    case 25: return "EM";
    case 26: return "SUB";
    case 27: return "'\\e' (ESC)";
    case 28: return "FS";
    case 29: return "GS";
    case 30: return "RS";
    case 31: return "US";
    case 127: return "DEL";
    default: return QString("<%1>").arg(code);
    }
}