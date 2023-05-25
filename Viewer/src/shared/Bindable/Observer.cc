#include "Observer.h"

void Observer::StopObserving() {
  for (auto& observable : observables) observable->removeObserver(this);
}

void Observer::AddObservable(Observable* observable) {
  if (std::find(observables.begin(), observables.end(), observable) ==
      observables.end())
    observables.push_back(observable);
}

void Observer::RemoveObservable(Observable* observable) {
  observables.erase(
      std::remove(observables.begin(), observables.end(), observable),
      observables.end());
}

void Observable::AddObserver(Observer* observer) {
  if (std::find(observers.begin(), observers.end(), observer) !=
      observers.end())
    return;

  observer->AddObservable(this);
  observers.push_back(observer);
}

void Observable::removeObserver(Observer* observer) {
  if (std::find(observers.begin(), observers.end(), observer) ==
      observers.end())
    return;

  observer->RemoveObservable(this);
  observers.erase(std::remove(observers.begin(), observers.end(), observer),
                  observers.end());
}

void Observable::notifyObservers() {
  for (auto& observer : observers) observer->Notify();
}
