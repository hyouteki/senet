from termcolor import colored
import sys

def encrypt(plainText, key):
    """
    @desc: Takes plainText and 128bit Key and encrypts it using AES128
    @param {string} plainText: 128bit plainText
    @param {string} key: 128bit key
    @returns {string} cipherText: encrypted plainText
    """

    assert len(plainText) == 16

    with open("s_box.txt", "r") as file:
        sBox = [int(i) for i in file.read().rstrip("\n").split("\n")]

    def keyExpansion(key):
        roundKeys = [[ord(i) for i in key]]
        roundConsants = [(i, 0, 0, 0) for i in (1, 2, 4, 8, 16, 32, 64, 128, 27, 54)]
        for round in range(10):
            initVector = roundKeys[-1][-4:]
            initVector.append(initVector.pop(0))
            initVector = [sBox[i] for i in initVector]
            initVector = [initVector[i] ^ roundConsants[round][i] for i in range(4)]
            preRoundKey = roundKeys[-1].copy()
            roundKey = []
            for _ in range(4):
                word = [preRoundKey.pop(0) for _ in range(4)]
                initVector = [initVector[i] ^ word[i] for i in range(4)]
                roundKey.extend(initVector)
            roundKeys.append(roundKey)
        return roundKeys

    def substituteBlock(block):
        return [sBox[i] for i in block]

    def shiftRows(block):
        shiftRowIndex = (0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12, 1, 6, 11)
        return [block[shiftRowIndex[i]] for i in range(16)]

    def multiplyWithinGaloisField(a, b):
        ans = 0
        for _ in range(8):
            if b & 1:
                ans ^= a
            a <<= 1
            if a & 256:
                a ^= 283
            b >>= 1
        return ans

    def arrayToMatrix(array):
        return [[array[4 * j + i] for j in range(4)] for i in range(4)]

    def matrixToArray(matrix):
        array = [0 for _ in range(16)]
        for i in range(4):
            for j in range(4):
                array[4 * j + i] = matrix[i][j]
        return array

    def mixColumns(block):
        mixColumnConstant = ((2, 3, 1, 1), (1, 2, 3, 1), (1, 1, 2, 3), (3, 1, 1, 2),)
        stateMatrix = arrayToMatrix(block)
        newStateMatrix = [[0] * 4 for _ in range(4)]
        newStateMatrix = [[(
            multiplyWithinGaloisField(mixColumnConstant[i][0], stateMatrix[0][j])
            ^ multiplyWithinGaloisField(mixColumnConstant[i][1], stateMatrix[1][j])
            ^ multiplyWithinGaloisField(mixColumnConstant[i][2], stateMatrix[2][j])
            ^ multiplyWithinGaloisField(mixColumnConstant[i][3], stateMatrix[3][j])
        ) for j in range(4)] for i in range(4)]
        return matrixToArray(newStateMatrix)

    def addRoundKey(block, roundKeys, round):
        return [block[i] ^ roundKeys[round][i] for i in range(16)]

    def printHex(block):
        print(colored(" ".join([
            f"0x0{hex(cell)[2:]}" if len(hex(cell)[2:]) == 1 else hex(cell)
            for cell in block]), "blue",)) 

    block = [ord(i) for i in plainText]
    roundKeys = keyExpansion(key)
    block = addRoundKey(block, roundKeys, 0)
    print(colored(f"ROUND {0}", "green"), end="  \t" if 0 < 10 else " \t")
    printHex(block)
    for round in range(1, 11):
        block = substituteBlock(block)
        block = shiftRows(block)
        if round < 10:
            block = mixColumns(block)
        block = addRoundKey(block, roundKeys, round)
        print(colored(f"ROUND {round}", "green"), end="  \t" if round < 10 else " \t")
        printHex(block)
    cipherText = ""
    for i in block:
        char = hex(i)[2:]
        if len(char) == 1:
            char = "0" + char
        cipherText += char
    return cipherText


def decrypt(cipherText, key):
    """
    @desc: Takes cipherText and 128bit Key and decrypts it using AES128
    @param {string} cipherText: 128bit cipherText
    @param {string} key: 128bit key
    @returns {string} plainText: decrypted cipherText
    """

    assert len(cipherText) == 32

    with open("s_box.txt", "r") as file:
        sBox = [int(i) for i in file.read().rstrip("\n").split("\n")]
    with open("is_box.txt", "r") as file:
        isBox = [int(i) for i in file.read().rstrip("\n").split("\n")]

    def keyExpansion(key):
        roundKeys = [[ord(i) for i in key]]
        roundConsants = [(i, 0, 0, 0) for i in (1, 2, 4, 8, 16, 32, 64, 128, 27, 54)]
        for round in range(10):
            initVector = roundKeys[-1][-4:]
            initVector.append(initVector.pop(0))
            initVector = [sBox[i] for i in initVector]
            initVector = [initVector[i] ^ roundConsants[round][i] for i in range(4)]
            preRoundKey = roundKeys[-1].copy()
            roundKey = []
            for _ in range(4):
                word = [preRoundKey.pop(0) for _ in range(4)]
                initVector = [initVector[i] ^ word[i] for i in range(4)]
                roundKey.extend(initVector)
            roundKeys.append(roundKey)
        return roundKeys

    def inverseSubstituteBlock(block):
        return [isBox[i] for i in block]

    def inverseShiftRows(block):
        shiftRowIndex = (0, 13, 10, 7, 4, 1, 14, 11, 8, 5, 2, 15, 12, 9, 6, 3)
        return [block[shiftRowIndex[i]] for i in range(16)]

    def multiplyWithinGaloisField(a, b):
        ans = 0
        for _ in range(8):
            if b & 1:
                ans ^= a
            a <<= 1
            if a & 256:
                a ^= 283
            b >>= 1
        return ans

    def arrayToMatrix(array):
        return [[array[4 * j + i] for j in range(4)] for i in range(4)]

    def matrixToArray(matrix):
        array = [0 for _ in range(16)]
        for i in range(4):
            for j in range(4):
                array[4 * j + i] = matrix[i][j]
        return array

    def inverseMixColumns(block):
        mixColumnConstant = ((14, 11, 13, 9), (9, 14, 11, 13), (13, 9, 14, 11), (11, 13, 9, 14),)
        newStateMatrix = [[0] * 4 for _ in range(4)]
        stateMatrix = arrayToMatrix(block)
        for i in range(4):
            newStateMatrix[i] = [(
                multiplyWithinGaloisField(mixColumnConstant[i][0], stateMatrix[0][j])
                ^ multiplyWithinGaloisField(mixColumnConstant[i][1], stateMatrix[1][j])
                ^ multiplyWithinGaloisField(mixColumnConstant[i][2], stateMatrix[2][j])
                ^ multiplyWithinGaloisField(mixColumnConstant[i][3], stateMatrix[3][j])
            ) for j in range(4)]
        return matrixToArray(newStateMatrix)

    def addRoundKey(block, roundKeys, round):
        return [block[i] ^ roundKeys[round][i] for i in range(16)]

    def printHex(block):
        print(colored(" ".join([
            f"0x0{hex(cell)[2:]}" if len(hex(cell)[2:]) == 1 else hex(cell)
            for cell in block]), "blue",))

    block = [int(cipherText[i : i + 2], 16) for i in range(0, 32, 2)]
    print(colored(f"ROUND {0}", "green"), end="  \t" if 0 < 10 else " \t")
    printHex(block)
    roundKeys = keyExpansion(key)
    roundKeys.reverse()
    block = addRoundKey(block, roundKeys, 0)
    for round in range(1, 11):
        block = inverseShiftRows(block)
        block = inverseSubstituteBlock(block)
        print(colored(f"ROUND {round}", "green"), end="  \t" if round < 10 else " \t")
        printHex(block)
        block = addRoundKey(block, roundKeys, round)
        if round < 10:
            block = inverseMixColumns(block)
    return "".join([chr(i) for i in block])

if __name__ == "__main__":
    if sys.argv[1] == "encrypt":
        key = sys.argv[3]
        plainText = sys.argv[2]
        print(colored(f"plain text: {plainText}", "magenta"))
        cipherText = encrypt(plainText, key)
        print(colored(f"cipher text: {cipherText}", "magenta"))
    if sys.argv[1] == "decrypt":
        key = sys.argv[3]
        cipherText = sys.argv[2]
        print(colored(f"cipher text: {cipherText}", "magenta"))
        plainText = decrypt(cipherText, key)
        print(colored(f"plain text: {plainText}", "magenta"))
