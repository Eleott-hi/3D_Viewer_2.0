#pragma once
#include <QBindable>
#include <QDebug>
#include <QObject>
#include <QProperty>
#include <QVector3D>

namespace s21 {
class CameraViewModel : public QObject {
  Q_OBJECT

 public:
  Q_PROPERTY(QVector3D position READ GetPosition WRITE SetPosition NOTIFY
                 positionChanged BINDABLE BindablePosition)

  CameraViewModel(QObject* parent = nullptr) : QObject(parent) {
    position_.setBinding([this] {
      qDebug() << "HERE";
      return QVector3D(14.0843, 7.39728, -14.4024);
    });
  }
  ~CameraViewModel() {}
  QBindable<QVector3D> BindablePosition() const { return &position_; }
  QVector3D const& GetPosition() const {
    qDebug() << "GetPosition";
    return position_;
  }
  void SetPosition(QVector3D pos) {
    qDebug() << "SetPosition";
    position_ = pos;
  }

 signals:
  void positionChanged();

 private:
  //  QProperty<QVector3D> position_   ;
  Q_OBJECT_BINDABLE_PROPERTY(CameraViewModel, QVector3D, position_,
                             &CameraViewModel::positionChanged);
};

}  // namespace s21
