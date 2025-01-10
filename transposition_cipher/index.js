import * as Transposition_Cipher from "./transposition_cipher.js";

const globalContext = {};

function cstrlen(context, ptr, maxLength = 1024) {
    const buffer = context.exports.memory.buffer;
    let len = 0;
    while (buffer[ptr] !== 0 && len < maxLength) {
        len++;
        ptr++;
    }
    if (len >= maxLength) throw new Error("String length exceeds maximum allowed size.");
    return len;
}

function cstrFromPtr(context, ptr) {
    const mem = new Uint8Array(context.exports.memory.buffer);
    const len = cstrlen(context, ptr);
    const bytes = new Uint8Array(context.exports.memory.buffer, ptr, len);
    return new TextDecoder().decode(bytes);
}

function writeStringToMemory(context, string, offset) {
    const encoder = new TextEncoder();
    const encodedStr = encoder.encode(string + '\0');
    const memory = new Uint8Array(context.exports.memory.buffer);
    memory.set(encodedStr, offset);
    return offset;
}

WebAssembly.instantiateStreaming(fetch("module.wasm"), {
    env: {
		cstrlen: (textPtr) => cstrlen(globalContext, textPtr, maxLength=1024)
	}
}).then((wasm) => {
    console.log("INFO: wasm module loaded");
    globalContext.wasm = wasm;
    globalContext.exports = wasm.instance.exports;
}).catch((err) => {
    console.error("ERROR: could not load WebAssembly module:", err);
});


document.getElementById("execute-btn").addEventListener("click", () => {
    const mode = document.getElementById("mode").value;
    const inputText = document.getElementById("input-text").value.trim();
    const keyLength = parseInt(document.getElementById("key-length").value) || null;
    const key = document.getElementById("key").value.trim().split(" ").map(Number).filter(Boolean);
    const outputDiv = document.getElementById("output");

    const cipher = {
        encrypt: (text, key) => `Encrypted: ${text} (Key: ${key.join(", ")})`,
        decrypt: (text, key) => `Decrypted: ${text} (Key: ${key.join(", ")})`,
        hash: (text) => `Hash: ${text}`,
        bruteforce: (text) => `Brute-forced plaintext: ${text}`
    };

    try {
        let result;
        switch (mode) {
        case "encrypt":
            if (!keyLength || !key.length) throw new Error("Key length and key are required for encryption.");
            result = cipher.encrypt(inputText, key);
            break;
        case "decrypt":
            if (!keyLength || !key.length) throw new Error("Key length and key are required for decryption.");
            result = cipher.decrypt(inputText, key);
            break;
        case "hash":
			const hashPtr = 1024;
			writeStringToMemory(globalContext, "Hello World", hashPtr);
			const resultDiv = document.getElementById("result");
			const hashResult = globalContext.exports.hash(hashPtr) >>> 0;
			console.log(Transposition_Cipher);
			result = Transposition_Cipher.transformHash(globalContext, hashResult);
            break;
        case "bruteforce":
            result = cipher.bruteforce(inputText);
            break;
        default:
            throw new Error("Invalid mode selected.");
        }
        outputDiv.textContent = result;
    } catch (error) {
        outputDiv.textContent = `Error: ${error.message}`;
    }
});
