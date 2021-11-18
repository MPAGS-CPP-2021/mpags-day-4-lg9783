#include "PlayfairCipher.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

PlayfairCipher::PlayfairCipher(const std::string& key)
{
    setKey(key);
}

void PlayfairCipher::setKey(const std::string& key)
{
    // store the original key
    key_ = key;
    // Append the alphabet

    key_ += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Make sure the key is upper case
    std::transform(key_.begin(), key_.end(), key_.begin(), ::toupper);

    auto iter = std::remove_if(key_.begin(), key_.end(),
                               [](char val) { return !std::isalpha(val); });
    key_.erase(iter, key_.end());

    // Change J -> I
    auto func2 = [](char val) {
        if (val == 'J')
            return 'I';
        else
            return val;
    };
    std::transform(key_.begin(), key_.end(), key_.begin(), func2);

    // Remove duplicated letters

    std::string encounteredLetters{""};
    auto func3 = [&](char val) {
        std::size_t found = encounteredLetters.find(val);
        if (found != std::string::npos)
            return true;    //matches are found
        else {
            encounteredLetters += val;
            return false;
        }
    };
    auto iter2 = std::remove_if(key_.begin(), key_.end(), func3);
    key_.erase(iter2, key_.end());

    // Store the coords of each letter
    // Store the playfair cipher key map
    for (size_t c{0}; c < key_.size(); c++) {
        size_t row{c / gridSize_};
        size_t column{c % gridSize_};

        coords p = std::make_pair(row, column);
        map_[key_[c]] = p;

        map_reverse_[p] = key_[c];
    }
}

std::string PlayfairCipher::applyCipher(const std::string& inputText,
                                        const CipherMode cipherMode)
{
    // ChangeJ→I
    auto j2i = [](char val) {
        if (val == 'J')
            return 'I';
        else
            return val;
    };

    std::string inputText_{inputText};

    std::transform(inputText_.begin(), inputText_.end(), inputText_.begin(),
                   j2i);

    std::string newInput{inputText_[0]};

    std::string outputText{""};

    switch (cipherMode) {
        case CipherMode::Encrypt:
            // If repeated chars in a digraph add an X or Q if XX
            for (size_t c{1}; c < inputText_.size(); c++) {
                std::string temp{""};
                if (inputText_[c] == inputText_[c - 1]) {
                    if ((inputText_[c - 1] == 'X') && (inputText_[c] == 'X')) {
                        temp += "Q";
                    } else {
                        temp += "X";
                    }
                    temp += inputText_[c];
                } else {
                    temp += inputText_[c];
                }
                newInput += temp;
            }
            // if the size of input is odd, add a trailing Z
            if (newInput.size() % 2 != 0) {
                newInput += "Z";
            }

            // Loop over the input in Digraphs
            for (size_t c{0}; c < newInput.size(); c += 2) {
                //   - Find the coords in the grid for each digraph
                coords coord_1{map_[newInput[c]]};
                coords coord_2{map_[newInput[c + 1]]};
                coords new_coord_1;
                coords new_coord_2;

                //   - Apply the rules to these coords to get 'new' coords
                if (coord_1.first == coord_2.first) {    //same row
                    new_coord_1 = std::make_pair(
                        coord_1.first, (coord_1.second + 1) % gridSize_);
                    new_coord_2 = std::make_pair(
                        coord_2.first, (coord_2.second + 1) % gridSize_);

                } else if (coord_1.second == coord_2.second) {    //same column
                    new_coord_1 = std::make_pair(
                        (coord_1.first + 1) % gridSize_, coord_1.second);
                    new_coord_2 = std::make_pair(
                        (coord_2.first + 1) % gridSize_, coord_2.second);
                } else {
                    new_coord_1 = std::make_pair(coord_1.first, coord_2.second);
                    new_coord_2 = std::make_pair(coord_2.first, coord_1.second);
                }
                //   - Find the letter associated with the new coords
                char out1{map_reverse_[new_coord_1]};
                char out2{map_reverse_[new_coord_2]};
                outputText += out1;
                outputText += out2;
            }
            break;

        case CipherMode::Decrypt:
            std::string tempOutput{""};

            // Loop over the input in Digraphs
            for (size_t c{0}; c < inputText_.size(); c += 2) {
                //   - Find the coords in the grid for each digraph
                coords coord_1{map_[inputText_[c]]};
                coords coord_2{map_[inputText_[c + 1]]};
                coords new_coord_1;
                coords new_coord_2;
                //   - Apply the rules to these coords to get 'new' coords
                if (coord_1.first == coord_2.first) {    //same row
                    new_coord_1 = std::make_pair(
                        coord_1.first,
                        (coord_1.second + gridSize_ - 1) % gridSize_);
                    new_coord_2 = std::make_pair(
                        coord_2.first,
                        (coord_2.second + gridSize_ - 1) % gridSize_);

                } else if (coord_1.second == coord_2.second) {    //same column
                    new_coord_1 = std::make_pair(
                        (coord_1.first + gridSize_ - 1) % gridSize_,
                        coord_1.second);
                    new_coord_2 = std::make_pair(
                        (coord_2.first + gridSize_ - 1) % gridSize_,
                        coord_2.second);
                } else {
                    new_coord_1 = std::make_pair(coord_1.first, coord_2.second);
                    new_coord_2 = std::make_pair(coord_2.first, coord_1.second);
                }

                //   - Find the letter associated with the new coords
                char out1{map_reverse_[new_coord_1]};
                char out2{map_reverse_[new_coord_2]};
                tempOutput += out1;
                tempOutput += out2;
            }

            // remove final letter if Z
            if (tempOutput.back() == 'Z') {
                tempOutput.pop_back();
            }
            // remove xs and qs
            outputText.push_back(tempOutput[0]);
            for (size_t c{1}; c < tempOutput.size(); c++) {
                std::string temp{""};
                if (!(((tempOutput[c + 1] == tempOutput[c - 1]) &&
                       (tempOutput[c] == 'X')) ||
                      ((tempOutput[c + 1] == tempOutput[c - 1]) &&
                       (tempOutput[c] == 'Q')))) {
                    temp += tempOutput[c];
                }
                outputText += temp;
            }

            break;
    }

    // return the text

    return outputText;
}
