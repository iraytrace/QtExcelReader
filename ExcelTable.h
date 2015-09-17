#ifndef EXCELTABLE_H
#define EXCELTABLE_H

#include <QTableWidget>
#include <QAxObject>
#include <QTableWidget>

class ExcelTable : public QTableWidget
{
        Q_OBJECT
public:
    explicit ExcelTable(QWidget *parent = 0);

    ~ExcelTable() {}
public slots:
    void import(QString fileName, int sheetNumber=1);

private:
    void printProperties(QAxObject* ob, const char *label, QTextStream &stream);
    void setHorizontalAlignmentForCell(QTableWidgetItem *twi, QAxObject* cell);
};
#endif // EXCELTABLE_H

