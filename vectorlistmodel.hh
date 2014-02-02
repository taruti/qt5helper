#ifndef VECTORLISTMODEL_HH
#define VECTORLISTMODEL_HH

#include <QAbstractListModel>

class VectorListModel : public QAbstractListModel
{
    Q_OBJECT

    std::vector<std::string> vec_ = {};
    Qt::ItemFlags flags_;
public:
    VectorListModel(Qt::ItemFlags flags) : flags_(flags) {}
    void add(std::string s) {
        auto len = vec_.size();
        beginInsertRows(QModelIndex(), len, len);
        vec_.push_back(std::move(s));
        endInsertRows();
    }

    int rowCount(const QModelIndex&) const {
        return vec_.size();
    }

    QVariant data(const QModelIndex &index, int role) const {
        if (!index.isValid())
            return QVariant();
        switch(role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return QString::fromStdString(vec_[index.row()]);
        }

        return QVariant();
    }

    Qt::ItemFlags flags(const QModelIndex &index) const {
        if (!index.isValid())
            return Qt::ItemIsEnabled;

        return QAbstractItemModel::flags(index) | flags_;
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role) {
        if (index.isValid() && role == Qt::EditRole) {

            vec_[index.row()] = value.toString().toStdString();
            emit dataChanged(index, index);
            return true;
        }

        return false;
    }

};

#endif // VECTORLISTMODEL_HH
