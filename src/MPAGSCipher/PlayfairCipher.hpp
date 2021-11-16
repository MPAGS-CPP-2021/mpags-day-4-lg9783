#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

#include "CipherMode.hpp"
#include <string>
#include <map>


/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */
class PlayfairCipher {
  public:
    /**
     * \brief Create a new PlayfairCipher with the given key
     *
     * \param key the key to use in the cipher
     */
    explicit PlayfairCipher(const std::string& key);

    /**
     * \brief sets the supplied key
     * 
     * \param key the key to use in the cipher
     */

    void setKey(const std::string& key);

    /**
     * \brief Apply the cipher to the provided text
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    std::string applyCipher(const std::string& inputText,
                           const CipherMode cipherMode);

    private:
    /// The cipher key
    std::string key_;

    using coords = std::pair<int, int>;

    /// The cipher map 
    std::map<char, coords> map_;
    /// The cipher 
    std::map<coords,char> map_reverse_;

    //grid size
    const std::string::size_type gridSize_{5};
    
};

#endif
