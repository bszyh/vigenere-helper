#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <utility>
#include <cctype>
#include <fstream>

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


std::vector<int> findRepeated(std::string text, std::string substr) {
    std::vector<int> indexes;
    int index = 0;
    while ((index = text.find(substr, index)) != std::string::npos) {
        indexes.push_back(index);
        index += substr.length();
    }

    return indexes;
}

std::map<std::string, unsigned> ngram_data(int n, std::string text) {
    std::map<std::string, unsigned> ngrams;

    for(int i = 0; i <= text.size() - n; i++) {
        char str_char = text.at(i);
        std::string substr = "";

        for(int k = i; k < i + n; k++) {
            substr += text.at(k);
        }

        std::vector<int> indexes = findRepeated(text, substr);
        if(indexes.size() > 1) {
	    for (int i = 0; i < indexes.size() - 1; i++) {
            	int distance = indexes[i+1] - indexes[i];
             	ngrams.insert(std::pair<std::string, int>(substr, distance));
	    }
        }
    }
    return ngrams;
}

std::vector<std::pair<int, int> > common_divisors(std::map<std::string, unsigned> ngrams) {
    std::vector<std::pair<int, int> > div;
    for(int i = 2; i < 25; i++) { 
        div.push_back(std::make_pair(i, 0));
    }

    for(const auto [key, value] : ngrams) {
        for(auto &d : div) {
            if(value % d.first == 0) {
                d.second++;
            }
        }
    }
    return div;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <text>"<< std::endl;
        return 1;
    }

    std::string text(argv[1]);


    text = cleanText(text);

    std::map<std::string, unsigned> ngrams(ngram_data(4, text));
    std::map<std::string, unsigned> tmp(ngram_data(3, text));
    ngrams.insert(tmp.begin(), tmp.end());

    std::ofstream file_ngram = fileCheckOpen("output_ngrams.csv");
    file_ngram << "N-gram,Distance\n"; 

    for(const auto [key, value] : ngrams) {
        file_ngram << key << "," << value << "\n";
    }

    file_ngram.close();

    std::ofstream file_div = fileCheckOpen("output_div.csv");
    file_div << "Divisor,Hits\n";

    std::vector<std::pair<int, int> > comm_div = common_divisors(ngrams);
    for(auto div : comm_div) {
        if(div.second != 0) {
            file_div << div.first << ",     " << div.second << "\n";  
        }     
    }

    file_div.close();
    
    return 0;
}
