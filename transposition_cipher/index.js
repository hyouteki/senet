const context = {};

function cstrlen(ptr) {
	const buffer = context.buffer;
	let len = 0;
	while (buffer[ptr] != 0) {
		len++;
		ptr++;
	}
	return len;
}

function cstrFromPtr(ptr) {
    const mem = new Uint8Array(context.buffer);
    const len = cstrlen(ptr);
    const bytes = new Uint8Array(context.buffer, ptr, len);
    return new TextDecoder().decode(bytes);
}

function writeStringToMemory(string, offset) {
	const encoder = new TextEncoder();
	const encodedStr = encoder.encode(string + '\0');
	const memory = new Uint8Array(context.buffer);
	memory.set(encodedStr, offset);
	return offset;
}

WebAssembly.instantiateStreaming(fetch("module.wasm"), {
	env: {
		print: (textPtr) => {
			console.log(textPtr, cstr_by_ptr(textPtr));
		}
	}
}).then((wasm) => {
	console.log("INFO: wasm module loaded");
	console.log(wasm);
	
	context.wasm = wasm;
	context.buffer = wasm.instance.exports.memory.buffer;

	const hashPtr = 1024;
	writeStringToMemory("Hello World", hashPtr);
	const resultDiv = document.getElementById("result");
	const result = wasm.instance.exports.hash(hashPtr) >>> 0;
	resultDiv.textContent += `${result}`;
});
