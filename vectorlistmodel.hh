#ifndef VECTORLISTMODEL_HH
#define VECTORLISTMODEL_HH

#include <QAbstractListModel>

class VectorListModel : public QAbstractListModel
{
    Q_OBJECT

    std::vector<std::string> vec_ = {};
    Qt::ItemFlags flags_;
public:
    // vector interface
    using value_type = std::string;
    using size_type = std::vector<std::string>::size_type;
    size_type size() const { return vec_.size(); }
    void resize (size_type n, value_type val = value_type()) {
        int ldiff = vec_.size() - n;
        if(ldiff < 0) { // grow
            beginInsertRows(QModelIndex{}, vec_.size(), n);
            vec_.resize(n);
            endInsertRows();
        } else if(ldiff > 0) { // shrink
            beginRemoveRows(QModelIndex{}, n, vec_.size());
            vec_.resize(n, std::move(val));
            endRemoveRows();
        }
    }
    bool empty() const { return vec_.empty(); }
    void push_back (const value_type& val) {
        beginInsertRows(QModelIndex{}, vec_.size(), vec_.size());
        vec_.push_back(val);
        endInsertRows();
    }
    template <class... Args>
    void emplace_back (Args&&... args) {
        beginInsertRows(QModelIndex{}, vec_.size(), vec_.size());
        vec_.emplace_back(std::forward(args)...);
        endInsertRows();
    }


    // QAbstractListModel interface

    VectorListModel(Qt::ItemFlags flags) : flags_(flags) {}

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
