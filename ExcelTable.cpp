#include "ExcelTable.h"
#include <QTableWidgetItem>
#include <QMetaProperty>
#include <QFile>
#include <QTextStream>
static int junk = 0;

ExcelTable::ExcelTable(QWidget *parent) : QTableWidget(parent)
{

}

void ExcelTable::import(QString fileName, int sheetNumber)
{

    QFile outFile("excel.txt");
    outFile.open((QIODevice::WriteOnly));
    QTextStream stream(&outFile);


    QAxObject* excel = new QAxObject( "Excel.Application" );
    printProperties(excel, "excel", stream);
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    printProperties(workbooks, "workbooks", stream);
    QAxObject* workbook = workbooks->querySubObject( "Open(const QString&)", fileName );
    printProperties(workbook, "workbook", stream);
    QAxObject* sheets = workbook->querySubObject( "Worksheets" );
    printProperties(sheets, "sheets", stream);
    int sheetCount = sheets->dynamicCall("Count()").toInt();        //worksheets count
    if (sheetNumber > sheetCount)
        return;
    QAxObject* sheet = sheets->querySubObject( "Item( int )", sheetNumber );
    printProperties(sheet, "sheet", stream);
    // Find the cells that actually have content
    QAxObject* usedrange = sheet->querySubObject( "UsedRange");
    printProperties(usedrange, "usedrange", stream);
    QAxObject * rows = usedrange->querySubObject("Rows");

    printProperties(rows, "rows", stream);
    QAxObject * columns = usedrange->querySubObject("Columns");
    printProperties(columns, "columns", stream);
    int intRowStart = usedrange->property("Row").toInt();
    int intColStart = usedrange->property("Column").toInt();
    int intCols = columns->property("Count").toInt();
    int intRows = rows->property("Count").toInt();

    // replicate the Excel content in the QTableWidget
    this->setColumnCount(intColStart+intCols);
    this->setRowCount(intRowStart+intRows);
    QAxObject *junkCell = sheet->querySubObject( "Cells( int, int )",intRowStart, intColStart);
    printProperties(junkCell, "cell", stream);

#if 0
    QVariant v = junkCell->property("HorizontalAlignment");
    stream << "  cellHorizontalAlignment (" << v.toInt() << ")\n";
#else

#endif
    for (int row=intRowStart ; row < intRowStart+intRows ; row++) {
        for (int col=intColStart ; col < intColStart+intCols ; col++) {

            QAxObject* cell = sheet->querySubObject( "Cells( int, int )", row, col );
            QVariant value = cell->dynamicCall( "Value()" );
            if (value.isNull())
                continue;

            QTableWidgetItem * twi = new QTableWidgetItem;
            twi->setData(Qt::DisplayRole, value);
            setHorizontalAlignmentForCell(twi, cell);

            this->setItem(row-1, col-1, twi);
        }
    }

    // clean up and close up
    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");

    this->resizeColumnsToContents();
    stream.flush();
    outFile.close();
}

void ExcelTable::printProperties(QAxObject *ob, const char *label, QTextStream &stream)
{
    return;
    const QMetaObject *mo = ob->metaObject();
    stream << QString::asprintf("%s has %d:\n", label, mo->propertyCount());

    for (int i=0 ; i < mo->propertyCount() ; i++) {
        QMetaProperty mp = mo->property(i);
        if (mp.type() == QVariant::String) {

            QVariant v = mp.read(ob);
            if (v.isValid() && !v.isNull()) {
                QString s = v.toString();
                stream << QString::asprintf("    %s:%s(%s)\n", mp.name(), mp.typeName(), qPrintable(s));
            } else {
                stream << QString::asprintf("    %s:%s(void)\n", mp.name(), mp.typeName());
            }

        } else if (mp.type() == QVariant::Bool) {

            bool tf = mp.read(ob).toBool();
            stream << QString::asprintf("    %s:%s[%s]\n", mp.name(), mp.typeName(), (tf?"true":"false"));

        } else if (mp.type() == QVariant::Int) {

            stream << QString::asprintf("    %s:%s(%d)\n", mp.name(), mp.typeName(), mp.read(ob).toInt());

        } else {
            QString tName(mp.typeName());

            if (!tName.compare("QVariant")) {
                QVariant v = mp.read(ob);
                if (v.isValid() && !v.isNull()) {
                    stream << QString::asprintf("    %s:%s varies<%s>\n", mp.name(), mp.typeName(), qPrintable(v.toString()));
                } else {
                    stream << QString::asprintf("    %s:%s varies<null>\n", mp.name(), mp.typeName());
                }
            } else {
                stream << QString::asprintf("    %s:%s\n", mp.name(), mp.typeName());
            }
        }
    }
}

void ExcelTable::setHorizontalAlignmentForCell(QTableWidgetItem *twi, QAxObject *cell)
{
    QVariant horzAlignVariant = cell->dynamicCall( "HorizontalAlignment()" );
    int horizAlign = horzAlignVariant.toInt();

    switch (horizAlign) { // note these are all MS "Magic Numbers"
    case -4117: twi->setTextAlignment(Qt::AlignHCenter); break;
    case -4130: twi->setTextAlignment(Qt::AlignJustify); break;
    case -4131: twi->setTextAlignment(Qt::AlignLeft); break;
    case -4108: twi->setTextAlignment(Qt::AlignHCenter); break;
    case -4152: twi->setTextAlignment(Qt::AlignRight); break;
    default:
        break;
    }
}
