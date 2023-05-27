#ifndef SRC_VIEW_CUSTOM_LABEL_H_
#define SRC_VIEW_CUSTOM_LABEL_H_

#include <QLabel>
#include <QPainter>

class CustomLabel : public QLabel {
  Q_OBJECT
 public:
  CustomLabel(CustomLabel &&) = delete;
  CustomLabel(CustomLabel const &) = delete;
  CustomLabel &operator=(CustomLabel &&) = delete;
  CustomLabel &operator=(CustomLabel const &) = delete;
  ~CustomLabel() = default;

  CustomLabel(QWidget *parent = nullptr) : QLabel(parent) {
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &CustomLabel::CloseSignal, this, &CustomLabel::close);
    connect(this, &CustomLabel::ShowSignal, this, &CustomLabel::show);
  }

  template <typename... Args>
  void Replace(Args... args) {
    setText(text().replace(args...));
  }

 signals:
  void mousePressEvent(QMouseEvent *event) override;
  void CloseSignal();
  void ShowSignal();
};

#endif  // SRC_VIEW_CUSTOM_LABEL_H_
