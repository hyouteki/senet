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
    env: {}
}).then((wasm) => {
    console.log("INFO: wasm module loaded");
    globalContext.wasm = wasm;
    globalContext.exports = wasm.instance.exports;
	
    const hashPtr = 1024;
    writeStringToMemory(globalContext, "Hello World", hashPtr);
    const resultDiv = document.getElementById("result");
    const hashResult = wasm.instance.exports.hash(hashPtr) >>> 0;
	console.log(Transposition_Cipher);
    const transformedResult = Transposition_Cipher.transformHash(globalContext, hashResult);
	const retransformedResult = Transposition_Cipher.retransformHash(globalContext, transformedResult);
	resultDiv.textContent += `Raw Hash: ${hashResult}, Transformed Hash: ${transformedResult}, ReTransformed Hash: ${retransformedResult}, Equality Check: ${hashResult === retransformedResult}`;

}).catch((err) => {
    console.error("ERROR: could not load WebAssembly module:", err);
});
