#include <iostream>
#include "prime_number.h"
using namespace std;

int main()
{
    /*
    Imagine Alice & Bob wants to talk in symmetric 
    key cryptography environment. Meaning Alice and 
    Bob only possesses their own private key and not
    a public key. 
    
    So, they want to encrypt their message but they 
    can't share keys over network; cause anybody can 
    see them. Diffie-Hellman provides a way through
    some simple key exchanges with which both Alice &
    Bob can reach to same key and only Alice & Bob can.
    */
    
    int bit_size = 64, generator_bit_size = 16;
    big_number a, b, g, n, A, B, ka, kb;
    
    // STEP 1
    // a & b are private to only Alice & Bob 
    a = get_random_prime(bit_size); // Alice's private key
    b = get_random_prime(bit_size); // Bob's private key
    
    cout << "a: " << a << endl;
    cout << "b: " << b << endl;
    
    // STEP 2
    // g & n are publicly known
    // g :: a generator with which we generate keys
    // n :: a very big number; useful in taking modulus
    
    // g is usually a very small prime number
    g = get_random_prime(generator_bit_size);
    cout << "g: " << g << endl;
    
    // n is a big number for this to securely work
    n = get_random_number(2*bit_size);
    cout << "n: " << n << endl;
    
    // STEP 3
    // Alice and bob will calculate A and B
    // i.e. A = g^a (mod n); B = g^b (mod n)
    // and share these over the network.
    // for analogy sake; we can say that these
    // are their public key equivalent.
    
    A = power_modulus(g, a, n);
    cout << "A: " << A << endl;
    
    B = power_modulus(g, b, n);
    cout << "B: " << B << endl;
    
    // STEP 4
    // Alice and Bob will calculate ka and kb
    // which will be there exchange keys.
    // ka = B^a (mod n)
    // kb = A^b (mod n)
    
    ka = power_modulus(B, a, n);
    cout << "ka: " << ka << endl;
    
    kb = power_modulus(A, b, n);
    cout << "kb: " << kb << endl;
    
    // STEP 5
    // check whether the exchange keys are same
    cout << "samekey: " << (ka == kb) << endl;
    
    return 0;
}
