#pragma once

#include <algorithm>
#include <vector>

class Observable;

class Observer {
 protected:
  std::vector<Observable*> observables;

 public:
  virtual void OnNotify() = 0;

  void StopObserving();
  void Notify() { OnNotify(); }
  void AddObservable(Observable* observable);
  void RemoveObservable(Observable* observable);
};

class Observable {
 protected:
  std::vector<Observer*> observers;

 public:
  virtual void notifyObservers();
  virtual void AddObserver(Observer* observer);
  virtual void removeObserver(Observer* observer);
};