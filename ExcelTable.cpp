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
    QAxObject* excel = new QAxObject( "Excel.Application" );
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    QAxObject* workbook = workbooks->querySubObject( "Open(const QString&)", fileName );
    QAxObject* sheets = workbook->querySubObject( "Worksheets" );
    int sheetCount = sheets->dynamicCall("Count()").toInt();        //worksheets count
    if (sheetNumber > sheetCount)
        return;
    QAxObject* sheet = sheets->querySubObject( "Item( int )", sheetNumber );
    // Find the cells that actually have content
    QAxObject* usedrange = sheet->querySubObject( "UsedRange");
    QAxObject * rows = usedrange->querySubObject("Rows");
    QAxObject * columns = usedrange->querySubObject("Columns");
    int intRowStart = usedrange->property("Row").toInt();
    int intColStart = usedrange->property("Column").toInt();
    int intCols = columns->property("Count").toInt();
    int intRows = rows->property("Count").toInt();

    // replicate the Excel content in the QTableWidget
    this->setColumnCount(intColStart+intCols);
    this->setRowCount(intRowStart+intRows);
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
