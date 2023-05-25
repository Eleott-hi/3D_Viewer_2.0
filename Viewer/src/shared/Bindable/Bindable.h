
#include <iostream>

#include "Observer.h"

template <typename Type>
class Bindable : public Observable, public Observer {
 private:
  std::function<Type()> subscribe_ = nullptr;
  Type value_ = {};
  int updating_ = 0;

 public:
  Bindable() = default;
  ~Bindable() = default;
  Bindable(Bindable&&) = default;
  Bindable(Bindable const&) = default;
  Bindable& operator=(Bindable&&) = default;
  Bindable& operator=(Bindable const&) = default;

  Bindable(Type const& value) : value_(value) {
    subscribe_ = [value] { return value; };
  }

  Bindable(Bindable& bind) {
    value_ = bind.value_;
    subscribe_ = [&] { return bind.value_; };
    bind.AddObserver(this);
  }

  void OnNotify() override { Update(); }

  void Update() {
    if (updating_ > 2) {
      std::cerr << "Bind loop detected! More than 2 loops. Aborting update."
                << std::endl;
      return;
    }

    updating_++;

    Type old_value = value_;
    if (subscribe_) value_ = subscribe_();
    if (old_value != value_) notifyObservers();

    updating_ = 0;
  }

  void SetValue(Type const& value) {
    subscribe_ = [value = value] { return value; };
    StopObserving();

    Update();
  }

  Bindable& operator=(Type const& bind) {
    SetValue(bind);
    return *this;
  }

  Bindable& operator=(Bindable& bind) {
    if (this != &bind) {
      StopObserving();
      subscribe_ = [&] { return bind.value_; };
      bind.AddObserver(this);
      Update();
    }

    return *this;
  }

  Type const& Value() const { return value_; }
  Type& Value() { return value_; }

  template <typename... Args>
  void Subscribe(std::function<Type()> const& callback, Args&... bind) {
    subscribe_ = callback;

    StopObserving();

    (bind.AddObserver(this), ...);

    Update();
  }

  friend std::ostream& operator<<(std::ostream& os, Bindable const& bind) {
    return os << bind.value_;
  }
};
