#include <iostream>
#include <vector>
#include <algorithm>

void PrintArray(int* output, int n) {
  for (int i = 0; i < n; ++i) {
    std::cout << output[i] << ", ";
  }
  std::cout << std::endl;
}

void PrintVector(const std::vector<int> & v) {
  for (const int & it : v) {
    std::cout << it << ", ";
  }
  std::cout << std::endl;
}

void PermutationsRI(int* inputSet, int n, std::vector<int> &output) {
  if (n == output.size()) {
    PrintVector(output);
    return;
  }
  for (int i = 0; i < n; ++i) {
    output.push_back(inputSet[i]);
    PermutationsRI(inputSet, n, output);
    output.pop_back();
  }
}

void PermutationsR(int* inputSet, int n) {
  std::vector<int> output;
  // avoid re-allocations and just allocate the known expected items
  output.reserve(n);
  PermutationsRI(inputSet, n, output);
}

void Permutations(int* input, int n, int pos = 0) {
  if (pos == n) {
      PrintArray(input, n);
  }
  else
  {
      Permutations(input, n, pos + 1);
      for (int i = pos + 1; i < n; i++)
      {
          std::swap(input[pos], input[i]);
          Permutations(input, n, pos + 1);
          std::swap(input[pos], input[i]);
      }
  }
}

void VariationsRI(int* inputSet, std::vector<int>& vs, int n, int k) {
  if (vs.size() == k) {
    // subset is full to the desired ammount
    PrintVector(vs);
    return;
  }
  for (int i = 0; i < n; ++i) {
    vs.push_back(inputSet[i]);
    VariationsRI(inputSet, vs, n, k);
    vs.pop_back();
  }
}

// Stub function to set-up the parameters for the real implementation
void VariationsR(int* inputSet, int n, int k) {
  std::vector<int> variation;
  // avoid re-allocations and just allocate the known expected items
  variation.reserve(k);
  VariationsRI(inputSet, variation, n, k);
}

void VariationsI(int* inputSet, std::vector<int>& subset, int n, int k, int currentPosition = 0) {
  if (subset.size() == k) {
    // subset is full to the desired ammount
    Permutations(subset.data(), k);
    return;
  }
  for (int i = currentPosition; i < n; ++i) {
    subset.push_back(inputSet[i]);
    VariationsI(inputSet, subset, n, k, i + 1);
    subset.pop_back();
  }
}

// Stub function to set-up the parameters for the real implementation
void Variations(int* inputSet, int n, int k) {
  std::vector<int> variation;
  VariationsI(inputSet, variation, n, k);
}

void CombinationsRI(int* inputSet, std::vector<int>& cs, int n, int k, int currentPosition = 0) {
  if (cs.size() == k) {
    // subset is full to the desired ammount
    PrintVector(cs);
    return;
  }
  for (int i = currentPosition; i < n; ++i) {
    cs.push_back(inputSet[i]);
    CombinationsRI(inputSet, cs, n, k, i);
    cs.pop_back();
  }
}

void CombinationsR(int* inputSet, int n, int k) {
  std::vector<int> combinations;
  // avoid re-allocations and just allocate the known expected items
  combinations.reserve(k);
  CombinationsRI(inputSet, combinations, n, k);
}


void CombinationsI(int* inputSet, std::vector<int>& cs, int n, int k, int currentPosition = 0) {
  if (cs.size() == k) {
    // subset is full to the desired ammount
    PrintVector(cs);
    return;
  }
  for (int i = currentPosition; i < n; ++i) {
    cs.push_back(inputSet[i]);
    CombinationsI(inputSet, cs, n, k, i + 1);
    cs.pop_back();
  }
}

void Combinations(int* inputSet, int n, int k) {
  std::vector<int> combinations;
  // avoid re-allocations and just allocate the known expected items
  combinations.reserve(k);
  CombinationsI(inputSet, combinations, n, k);
}


int main() {
  std::cout << "Permutations with repetions" << std::endl;
  {
    int inputSet[] = {1, 2, 3};
    PermutationsR(inputSet, sizeof(inputSet) / sizeof(inputSet[0]));
  }

  std::cout << "Permutations without repetions" << std::endl;
  {
    int inputSet[] = {1, 2, 3};
    Permutations(inputSet, sizeof(inputSet) / sizeof(inputSet[0]));
  }

  std::cout << "Variations with repetions" << std::endl;
  {
    int inputSet[] = {1, 2, 3, 4};
    int n = sizeof(inputSet) / sizeof(inputSet[0]);
    int k = 3;
    VariationsR(inputSet, n, k);
  }

  std::cout << "Variations without repetions" << std::endl;
  {
    int inputSet[] = {1, 2, 3, 4};
    int n = sizeof(inputSet) / sizeof(inputSet[0]);
    int k = 3;
    Variations(inputSet, n, k);
  }

  std::cout << "Combinations with repetions" << std::endl;
  {
    int inputSet[] = {1, 2, 3, 4};
    int n = sizeof(inputSet) / sizeof(inputSet[0]);
    int k = 3;
    CombinationsR(inputSet, n, k);
  }

  std::cout << "Combinations without repetions" << std::endl;
  {
    int inputSet[] = {1, 2, 3, 4, 5};
    int n = sizeof(inputSet) / sizeof(inputSet[0]);
    int k = 3;
    Combinations(inputSet, n, k);
  }
}
