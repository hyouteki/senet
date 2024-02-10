package main
import "fmt"

type word [4]byte
type key [4]word
type block [4]word

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

func sub_block(b block, box [256]byte) block {
	var new_b block
	for i, w := range b {
		new_b[i] = sub_word(w, box)
	}
	return new_b
}

func shift_rows(b block) block {
	var new_b block
	for i := 0; i < 4; i++ {
		for j := 0; j < 4; j++ {
			new_b[i][j] = b[i][(i+j)%4]
		}
	}
	return new_b
}

func multiply_within_galois_field(b1 byte, b2 byte) byte {
	n1 := int(b1)
	n2 := int(b2)
	var n3 int = 0
	for i := 0; i < 8; i++ {
		if n2 & 1 == 1 {
			n3 ^= n1
		}
		n1 <<= 1
		if n1 & 256 == 1 {
			n1 ^= 283
		}
		n2 >>= 1
	}
	return byte(n3)
}

func mix_columns(b block) block {
	var mix_column_constants = block {
		{2, 3, 1, 1},
		{1, 2, 3, 1},
		{1, 1, 2, 3},
		{3, 1, 1, 2},
	}
	var new_b block
	for i := 0; i < 4; i++ {
		for j := 0; j < 4; j++ {
			var x byte = 0
			for k := 0; k < 4; k++ {
				x ^= multiply_within_galois_field(b[k][j],
					mix_column_constants[i][k])
			}
			new_b[i][j] = x
		}
	}
	return new_b
}

func add_round_key(b block, k key) block {
	for i, w := range b {
		b[i] = xor_words(w, k[i])
	}
	return b
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

func make_block(str string) block {
	var b block
	for i := 0; i < 4; i++ {
		for j := 0; j < 4; j++ {
			b[i][j] = byte(str[i+4*j]) 
		}
	}
	return b
}

func print_block(b block) {
	for i := 0; i < 4; i++ {
		for j := 0; j < 4; j++ {
			fmt.Printf("%x", b[j][i])
		}
	}
	fmt.Printf("\n")
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

func encrypt(plaintext string, key_str string, debug bool) block {
	k := make_key(key_str)
	round_keys := expand_key(k)
	if debug {
		fmt.Println("round-keys:")
		for round, round_key := range round_keys {
			fmt.Printf("\t%d\t: %v\n", round, round_key)
		}
		fmt.Printf("\n")
	}
	b := make_block(plaintext)
	b = add_round_key(b, round_keys[0])
	for round := 1; round <= 10; round++ {
		b = sub_block(b, sbox)
		b = shift_rows(b)
		if round != 10 {
			b = mix_columns(b)
		}
		b = add_round_key(b, round_keys[round])
		if debug {
			fmt.Printf("encryption-round-%d\t: ", round)
			print_block(b)
		}
	}
	if debug {
		fmt.Printf("\n")
	}
	return b
}

func main() {
	encrypted_block := encrypt("nosferatu0123456", "ABCDabcd12344321", true)
	fmt.Printf("encrypted-text: ")
	print_block(encrypted_block)
}
