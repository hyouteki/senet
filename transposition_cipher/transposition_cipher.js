export function transformHash(context, hash) {
    let transformedHash = "";
    for (let i = 0; i < context.exports.hash_len() - 1; i++) {
        transformedHash = String.fromCharCode((hash % 10) + 97) + transformedHash;
        hash = Math.floor(hash / 10);
    }
    return transformedHash;
}

export function retransformHash(context, transformedHash) {
    let hash = 0;
    for (let c of transformedHash) {
		hash = hash*10 + c.charCodeAt(0) - 97;
    }
    return hash;
}
