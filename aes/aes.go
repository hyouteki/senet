package main
import "fmt"

type word [4]byte
type key [4]word

func rotate_word(w word) word {
	var rotated_w word
	for i := 0; i < 3; i++ {
		rotated_w[i] = w[i+1]
	}
	rotated_w[3] = w[0]
	return rotated_w
}

func sub_word(w word, box [256]byte) word {
	var new_w word
	for i := 0; i < 4; i++ {
		new_w[i] = box[w[i]]
	}
	return new_w
}

func xor_words(w1 word, w2 word) word {
	var new_w word
	for i := 0; i < 4; i++ {
		new_w[i] = w1[i] ^ w2[i]
	}
	return new_w
}

func make_key(str string) key {
	var k key
	for i := 0; i < 4; i++ {
		for j := 0; j < 4; j++ {
			k[i][j] = byte(str[i+4*j]) 
		}
	}
	return k
}

func expand_key(k0 key) [11]key {
	var keys [11]key
	keys[0] = k0
	var round_constants_val = [10]byte {1, 2, 4, 8, 16, 32, 64, 128, 27, 54}
	var round_constants [10]word
    for i, x := range round_constants_val {
        round_constants[i][0] = x
    }
	for i := 0; i < 10; i++ {
		var ki key
		pre := rotate_word(keys[i][3])
		pre = sub_word(pre, sbox)
		pre = xor_words(pre, round_constants[i])
		for j := 0; j < 4; j++ {
			ki[j] = xor_words(pre, keys[i][j])
			pre = ki[j]
		}
		keys[i+1] = ki
	} 
	return keys
}

func main() {
	key := make_key("ABCDabcd12344321")
	expanded_key := expand_key(key)
	fmt.Println("key: ", key)
	fmt.Println("Expanded key: ", expanded_key)
}
