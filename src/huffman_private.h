#include "huffman.h"

HuffNode *buildHufftree(Buffer *src);
void encodePayload(Buffer *src, BitArray *dst, BitArray **codes);
void encodeLength(BitArray *ba, size_t val);
void encodeLength(BitArray *ba, size_t val);
size_t decodeLength(BitArrayReader *br);
HuffNode *buildHufftree(Buffer *src);
void cacheHuffcodes(HuffNode *, BitArray **, char *, int length);
void encodePayload(Buffer *src, BitArray *dst, BitArray **codes);
Buffer *decodePayload(BitArrayReader *reader, HuffNode *tree, size_t decoded_length);
