from termcolor import colored
import sys

class AES():
    def __init__(self, key, debug=False):
        assert len(key) == 16
        self.key = key
        self.debug = debug
        with open("s_box.txt", "r") as file:
            self.sBox = [int(i) for i in file.read().rstrip("\n").split("\n")]
        with open("is_box.txt", "r") as file:
            self.isBox = [int(i) for i in file.read().rstrip("\n").split("\n")]

    def printHex(self, block):
        print(colored(" ".join(
            [f"0x0{hex(cell)[2:]}" if len(hex(cell)[2:]) == 1 else hex(cell)
             for cell in block]), "blue"))

    def multiplyWithinGaloisField(self, a, b):
        mul = 0
        for _ in range(8):
            if b & 1:
                mul ^= a
            a <<= 1
            if a & 256:
                a ^= 283
            b >>= 1
        return mul

    def arrayToMatrix(self, array):
        return [[array[4 * j + i] for j in range(4)] for i in range(4)]
    
    def matrixToArray(self, matrix):
        array = [0 for _ in range(16)]
        for i in range(4):
            for j in range(4):
                array[4 * j + i] = matrix[i][j]
        return array
    
    def keyExpansion(self, key):
        roundKeys = [[ord(i) for i in key]]
        roundConstants = [(i, 0, 0, 0) for i in (1, 2, 4, 8, 16, 32, 64, 128, 27, 54)]
        for round in range(10):
            initVector = roundKeys[-1][-4:]
            initVector.append(initVector.pop(0))
            initVector = [self.sBox[i] for i in initVector]
            initVector = [initVector[i] ^ roundConstants[round][i] for i in range(4)]
            preRoundKey = roundKeys[-1].copy()
            roundKey = []
            for _ in range(4):
                word = [preRoundKey.pop(0) for _ in range(4)]
                initVector = [initVector[i] ^ word[i] for i in range(4)]
                roundKey.extend(initVector)
            roundKeys.append(roundKey)
        return roundKeys

    def substituteBlock(self, block):
        return [self.sBox[i] for i in block]

    def shiftRows(self, block):
        shiftRowIndex = (0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12, 1, 6, 11)
        return [block[shiftRowIndex[i]] for i in range(16)]

    def inverseSubstituteBlock(self, block):
        return [self.isBox[i] for i in block]

    def inverseShiftRows(self, block):
        shiftRowIndex = (0, 13, 10, 7, 4, 1, 14, 11, 8, 5, 2, 15, 12, 9, 6, 3)
        return [block[shiftRowIndex[i]] for i in range(16)]

    def mixColumns(self, block):
        mixColumnConstant = ((2, 3, 1, 1), (1, 2, 3, 1), (1, 1, 2, 3), (3, 1, 1, 2),)
        stateMatrix = self.arrayToMatrix(block)
        newStateMatrix = [[0] * 4 for _ in range(4)]
        newStateMatrix = [[(
            self.multiplyWithinGaloisField(mixColumnConstant[i][0], stateMatrix[0][j])
            ^ self.multiplyWithinGaloisField(mixColumnConstant[i][1], stateMatrix[1][j])
            ^ self.multiplyWithinGaloisField(mixColumnConstant[i][2], stateMatrix[2][j])
            ^ self.multiplyWithinGaloisField(mixColumnConstant[i][3], stateMatrix[3][j])
        ) for j in range(4)] for i in range(4)]
        return self.matrixToArray(newStateMatrix)

    def inverseMixColumns(self, block):
        mixColumnConstant = ((14, 11, 13, 9), (9, 14, 11, 13), (13, 9, 14, 11), (11, 13, 9, 14),)
        newStateMatrix = [[0] * 4 for _ in range(4)]
        stateMatrix = self.arrayToMatrix(block)
        for i in range(4):
            newStateMatrix[i] = [(
                self.multiplyWithinGaloisField(mixColumnConstant[i][0], stateMatrix[0][j])
                ^ self.multiplyWithinGaloisField(mixColumnConstant[i][1], stateMatrix[1][j])
                ^ self.multiplyWithinGaloisField(mixColumnConstant[i][2], stateMatrix[2][j])
                ^ self.multiplyWithinGaloisField(mixColumnConstant[i][3], stateMatrix[3][j])
            ) for j in range(4)]
        return self.matrixToArray(newStateMatrix)

    def addRoundKey(self, block, roundKeys, round):
        return [block[i] ^ roundKeys[round][i] for i in range(16)]
    
    def encrypt(self, plainText):
        assert len(plainText) == 16
        block = [ord(i) for i in plainText]
        roundKeys = self.keyExpansion(self.key)
        block = self.addRoundKey(block, roundKeys, 0)
        if self.debug:
            print(colored(f"ROUND {0}", "green"), end="  \t" if 0 < 10 else " \t")
            self.printHex(block)
        for round in range(1, 11):
            block = self.substituteBlock(block)
            block = self.shiftRows(block)
            if round < 10:
                block = self.mixColumns(block)
            block = self.addRoundKey(block, roundKeys, round)
            if self.debug:
                print(colored(f"ROUND {round}", "green"), end="  \t" if round < 10 else " \t")
                self.printHex(block)
        cipherText = ""
        for i in block:
            char = hex(i)[2:]
            if len(char) == 1:
                char = "0" + char
            cipherText += char
        return cipherText

    def decrypt(self, cipherText):
        assert len(cipherText) == 32
        block = [int(cipherText[i : i + 2], 16) for i in range(0, 32, 2)]
        if self.debug:
            print(colored(f"ROUND {0}", "green"), end="  \t" if 0 < 10 else " \t")
            self.printHex(block)
        roundKeys = self.keyExpansion(self.key)
        roundKeys.reverse()
        block = self.addRoundKey(block, roundKeys, 0)
        for round in range(1, 11):
            block = self.inverseShiftRows(block)
            block = self.inverseSubstituteBlock(block)
            if self.debug:
                print(colored(f"ROUND {round}", "green"), end="  \t" if round < 10 else " \t")
                self.printHex(block)
            block = self.addRoundKey(block, roundKeys, round)
            if round < 10:
                block = self.inverseMixColumns(block)
        return "".join([chr(i) for i in block])

if __name__ == "__main__":
    aes = AES(sys.argv[3], True)
    if sys.argv[1] == "encrypt":
        print(colored(f"plain text: {sys.argv[2]}", "magenta"))
        print(colored(f"cipher text: {aes.encrypt(sys.argv[2])}", "magenta"))
    if sys.argv[1] == "decrypt":
        print(colored(f"cipher text: {sys.argv[2]}", "magenta"))
        print(colored(f"plain text: {aes.decrypt(sys.argv[2])}", "magenta"))
