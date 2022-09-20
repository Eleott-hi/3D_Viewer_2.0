#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// #include "./matrix_algebra/s21_matrix.h"

class Mesh {
 public:
  using Verteces = std::vector<float>;
  using Facets = std::vector<std::vector<uint32_t>>;

 private:
  float x_min = INFINITY, y_min = INFINITY, z_min = INFINITY;
  float x_max = -INFINITY, y_max = -INFINITY, z_max = -INFINITY;
  Verteces verteces;
  Facets facets;

 public:
  Mesh() {}
  ~Mesh() {}

  const Verteces &getVerteces() { return verteces; }
  const Facets &getFacets() { return facets; }

  void loadFromFile(const std::string &filename) {
    clear();
    verteces.push_back(0.);
    verteces.push_back(0.);
    verteces.push_back(0.);
    std::ifstream file(filename);

    if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
        if (line.find("v ") == 0) {
          ParseVertex(line);
        } else if (line.find("f ") == 0) {
          ParseSurface(line);
        }
      }
      file.close();
    }
  }

  void clear() {
    verteces = Verteces();
    facets = Facets();
    x_min = y_min = z_min = INFINITY;
    x_max = y_max = z_max = -INFINITY;
  }

  bool isEmpty() { return verteces.empty(); }

  float getMinX() { return x_min; }
  float getMaxX() { return x_max; }
  float getMinY() { return y_min; }
  float getMaxY() { return y_max; }
  float getMinZ() { return z_min; }
  float getMaxZ() { return z_max; }

  void ParseVertex(std::string const &s) {
    std::stringstream ss(s.substr(s.find(' ')));
    float x = 0, y = 0, z = 0;
    ss >> x >> y >> z;
    if (x > x_max) x_max = x;
    if (y > y_max) y_max = y;
    if (z > z_max) z_max = z;
    if (x < x_min) x_min = x;
    if (y < y_min) y_min = y;
    if (z < z_min) z_min = z;
    verteces.push_back(x);
    verteces.push_back(y);
    verteces.push_back(z);
  }

  void ParseSurface(std::string const &s) {
    std::vector<unsigned int> numbers;
    int n = s.find(' ');
    while (n != std::string::npos) {
      if (n + 1 < s.size() && std::isdigit(s.at(n + 1))) {
        unsigned int value = std::stoi(s.substr(n + 1), nullptr);
        numbers.push_back(value);
      }
      n = s.find(' ', n + 1);
    }
    facets.push_back(std::move(numbers));
  }

  void print_info() {
    for (auto &i : verteces) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
    for (auto &i : facets) {
      for (auto &k : i) {
        std::cout << k << " ";
      }
      std::cout << std::endl;
    }
  }
};
