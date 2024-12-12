/* Отображение табличной модели данных */


#include <QApplication>
#include <QtWidgets>


class TableModel: public QAbstractTableModel{
private:
    int m_nRows;
    int m_nColumns;
    QHash<QModelIndex, QVariant> m_hash;

public:

    TableModel(int nRows, int nColumns, QObject* pobj = 0) : QAbstractTableModel(pobj)
        ,m_nRows(nRows), m_nColumns(nColumns)
    {
    }


    QVariant data(const QModelIndex& index, int nRole) const
    {
        if(!index.isValid())
        {
            return QVariant();
        }

        QString str = QString("%1, %2").arg(index.row() + 1).arg(index.column() + 1);
        return (nRole == Qt::DisplayRole || nRole == Qt::EditRole)
                   ? m_hash.value(index, QVariant(str)) : QVariant();
    }

    bool setData(const QModelIndex& index, const QVariant& value, int nRole)
    {
        if(index.isValid() && nRole == Qt::EditRole)
        {
            m_hash[index] = value;
            emit dataChanged(index, index);
            return true;
        }
        return false;
    }

    int rowCount(const QModelIndex& index) const
    {
        return m_nRows;
    }

    int columnCount(const QModelIndex&) const
    {
        return m_nColumns;
    }

    Qt::ItemFlags flags(const QModelIndex& index) const
    {
        Qt::ItemFlags flags = QAbstractTableModel::flags(index);
        return index.isValid() ? (flags | Qt::ItemIsEditable) : flags;
    }
};

class SimpleDelegate : public QStyledItemDelegate{
public:
    SimpleDelegate(QObject* pobj = 0) : QStyledItemDelegate(pobj)
    {
    }



    void paint(QPainter* pPainter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const
    {
        if(option.state & QStyle::State_MouseOver)
        {
            QRect rect = option.rect;


            /*
            QLinearGradient gradient(0,0, rect.width(), rect.height());

            gradient.setColorAt(0, Qt::white);
            gradient.setColorAt(0.5, Qt::blue);
            gradient.setColorAt(1, Qt::green);

            pPainter->setBrush(gradient);
            */
            QBrush brush;
            brush.setStyle(Qt::SolidPattern);
            brush.setColor(Qt::red);

            pPainter->setBrush(brush);

            pPainter->drawRect(rect);
        }
        QStyledItemDelegate::paint(pPainter, option, index);
        qDebug() << "Mouse move " << index.column() << ", " << index.row();


    }

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& /*index*/) const
    {
        return QSize(option.rect.width() - 1, option.rect.height() - 1);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TableModel model(10, 5);

    QWidget wgt;
    QVBoxLayout* pvbxLayout = new QVBoxLayout;

    QTableView* pTableView = new QTableView;
    pTableView->setModel(&model);
    //pTableView->setShowGrid(false);

    //SimpleDelegate* pDelegate = new SimpleDelegate;
    pTableView->setItemDelegate(new SimpleDelegate(pTableView));
    pTableView->viewport()->setAttribute(Qt::WA_Hover);

    pvbxLayout->addWidget(pTableView);

    wgt.setLayout(pvbxLayout);

    wgt.show();

    return app.exec();
}




