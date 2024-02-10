package main

import (
	"fmt"
)

func rotate_word(word [4]byte) [4]byte {
	var rotated_word [4]byte 
	for i := 0; i < 3; i++ {
		rotated_word[i] = word[i+1]
	}
	rotated_word[3] = word[0]
	return rotated_word
}

func sub_word(word [4]byte, box [256]byte) [4]byte {
	var new_word [4]byte
	for i := 0; i < 4; i++ {
		new_word[i] = box[word[i]]
	}
	return new_word
}

func byte_array_to_words(array [16]byte) [4][4]byte {
	var words [4][4]byte
	for i := 0; i < 4; i++ {
		for j := 0; j < 4; j++ {
			words[i][j] = array[i+4*j] 
		}
	}
	return words
}

func words_to_byte_array(words [4][4]byte) [16]byte {
	var array [16]byte
	for i := 0; i < 4; i++ {
		for j := 0; j < 4; j++ {
			array[i+4*j] = words[i][j]
		}
	}
	return array
}

func xor_words(word1 [4]byte, word2 [4]byte) [4]byte {
	var new_word [4]byte
	for i := 0; i < len(word1); i++ {
		new_word[i] = word1[i] ^ word2[i]
	}
	return new_word
}

func make_key(str string) [16]byte {
	var key [16]byte
	for i, ch := range str {
		key[i] = byte(ch)
	}
	return key
}

func expand_key(key [16]byte) [11][16]byte {
	var expanded_key [11][16]byte
	for i, num := range key {
		expanded_key[0][i] = num
	}
	var round_constants_val = [10]byte {1, 2, 4, 8, 16, 32, 64, 128, 27, 54}
	var round_constants [10][4]byte
    for i, x := range round_constants_val {
        round_constants[i][0] = x
    }
	for i := 0; i < 10; i++ {
		pre_words := byte_array_to_words(expanded_key[i])
		var new_words [4][4]byte
		pre := rotate_word(pre_words[3])
		pre = sub_word(pre, sbox)
		pre = xor_words(pre, round_constants[i])
		for i := 0; i < 4; i++ {
			new_words[i] = xor_words(pre, pre_words[i])
		}
		for j, x := range words_to_byte_array(new_words) {
			expanded_key[i+1][j] = x
		}
	} 
	return expanded_key
}

func main() {
	key := make_key("ABCDabcd12344321")
	expanded_key := expand_key(key)
	fmt.Println("key: ", key)
	fmt.Println("Expanded key: ", expanded_key)
}
