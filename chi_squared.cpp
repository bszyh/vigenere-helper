#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <vector>
#include <algorithm>
#include <fstream>

const std::string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::map<char, double> ENGLISH_FREQ = {
  {'A', 0.08167}, {'B', 0.01492}, {'C', 0.02782}, {'D', 0.04253},
  {'E', 0.12702}, {'F', 0.02228}, {'G', 0.02015}, {'H', 0.06094},
  {'I', 0.06966}, {'J', 0.00153}, {'K', 0.00772}, {'L', 0.04025},
  {'M', 0.02406}, {'N', 0.06749}, {'O', 0.07507}, {'P', 0.01929},
  {'Q', 0.00095}, {'R', 0.05987}, {'S', 0.06327}, {'T', 0.09056},
  {'U', 0.02758}, {'V', 0.00978}, {'W', 0.0236}, {'X', 0.0015},
  {'Y', 0.01974}, {'Z', 0.00074}
};


std::string cleanText(std::string& text){
    std::string cleant = "";

    for(auto c : text) {
        if(isalpha(c)) {
            cleant.push_back(toupper(c));
        }
    }
    return cleant;
}

std::ofstream fileCheckOpen(std::string file_name) {
    std::ofstream file;
    std::ifstream file_check(file_name);
    if(file_check.good()) {
      file_check.close();
      file.open(file_name, std::ofstream::out | std::ofstream::trunc);
    } else {
      file.open(file_name);
    }
    return file;
}


int get_shift(std::string text) {
  std::map<char, int> freq;
  for (int i = 0; i < text.length(); i++) {
    char c = toupper(text[i]);
    if (isalpha(c)) {
      freq[c]++;
    }
  }

  double min_chi_squared = INFINITY;
  int best_shift = 0;
  for (int i = 0; i < 26; i++) {
    double chi_squared = 0;
    for (int j = 0; j < 26; j++) {
      char c = ALPHABET[j];
      double expected = ENGLISH_FREQ.at(c) * text.length();
      double observed = freq[ALPHABET[(j + i) % 26]];
      chi_squared += pow(observed - expected, 2) / expected;
    }
    if (chi_squared < min_chi_squared) {
      min_chi_squared = chi_squared;
      best_shift = i;
    }
  }

  return best_shift;
}

std::vector<double> get_chis(std::string text) {
  std::map<char, int> freq;
  for (int i = 0; i < text.length(); i++) {
    char c = toupper(text[i]);
    if (isalpha(c)) {
      freq[c]++;
    }
  }

  std::vector<double> chi_s;
  double min_chi_squared = INFINITY;
  for (int i = 0; i < 26; i++) {
    double chi_squared = 0;
    for (int j = 0; j < 26; j++) {
      char c = ALPHABET[j];
      double expected = ENGLISH_FREQ.at(c) * text.length();
      double observed = freq[ALPHABET[(j + i) % 26]];
      chi_squared += pow(observed - expected, 2) / expected;
    }
    chi_s.push_back(chi_squared);
  }

  return chi_s;
}


std::string decipher_vigenere(std::string ciphertext, std::string key) {
  std::string plaintext = "";
  for (int i = 0; i < ciphertext.length(); i++) {
    char c = toupper(ciphertext[i]);
    if (isalpha(c)) {
      int shift = ALPHABET.find(key[i % key.length()]);
      int idx = ALPHABET.find(c);
      plaintext += ALPHABET[(idx - shift + 26) % 26];
    } else {
      plaintext += c;
    }
  }
  return plaintext;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
       std::cerr << "Usage: " << argv[0] << " <text> <key length>" << std::endl;
       return 1;
    }

    std::string ciphertext(argv[1]);
    int key_length = std::stoi(argv[2]);

    ciphertext = cleanText(ciphertext);

    std::vector<std::string> substrings;

    for(int period = 0; period < key_length; period++) {

        std::string substr = "";

        for(int i = period; i < ciphertext.size(); i += key_length) {
            substr += ciphertext[i];
        }
        substrings.push_back(substr);
    }

    std::ofstream file_ch = fileCheckOpen("output_ch.csv");
  

    std::vector<std::vector<double> > all_chi2val;
    std::string key;
    file_ch << "Letter,Chi_Squared\n";

    for(auto s : substrings) {
        std::vector<double> chi2s = get_chis(s);
        all_chi2val.push_back(chi2s);
        int shift = get_shift(s);
        float chi_sq = *(std::min_element(chi2s.begin(), chi2s.end()));
        file_ch << ALPHABET[shift] << ",  " << chi_sq << "\n";
        key.push_back(ALPHABET[shift]);
    }

    file_ch.close();

    std::string plaintext = decipher_vigenere(ciphertext, key);

    std::ofstream file_dec = fileCheckOpen("output_dec.csv");
    file_dec << "Key: " << key << "\n";
    file_dec << plaintext << "\n";
    file_dec.close();

    return 0;
}
