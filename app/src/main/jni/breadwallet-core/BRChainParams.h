//
//  BRChainParams.h
//
//  Created by Aaron Voisine on 1/10/18.
//  Copyright (c) 2019 bitdinar LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#ifndef BRChainParams_h
#define BRChainParams_h

#include "BRMerkleBlock.h"
#include <assert.h>

typedef struct {
    uint32_t height;
    UInt256 hash;
    uint32_t timestamp;
    uint32_t target;
} BRCheckPoint;

typedef struct {
    const char * const *dnsSeeds; // NULL terminated array of dns seeds
    uint16_t standardPort;
    uint32_t magicNumber;
    uint64_t services;
    int (*verifyDifficulty)(const BRMerkleBlock *block, const BRMerkleBlock *previous, uint32_t transitionTime);
    const BRCheckPoint *checkpoints;
    size_t checkpointsCount;
} BRChainParams;

static const char *BRMainNetDNSSeeds[] = {
    "ammar.bitdinar.xyz","hammadi.bitdinar.xyz", NULL
};

static const char *BRTestNetDNSSeeds[] = {
    "ammar.bitdinar.xyz","hammadi.bitdinar.xyz", NULL
};

// blockchain checkpoints - these are also used as starting points for partial chain downloads, so they must be at
// difficulty transition boundaries in order to verify the block difficulty at the immediately following transition
static const BRCheckPoint BRMainNetCheckpoints[] = {
  {      0, uint256("323655a66c1c5dcd26947f06dc19ba24397a5baf0ac388757016c251373119a1"), 1519120111, 0x1e0ffff0 },
  {      2, uint256("1b6dd98561c2ec6a06e3ee84cb9208000c9d14f86b102445f83c277abff5fde6"), 1519251111, 0x1e0ffff0 },
  {       250, uint256("ea4c4b0b8d15d2fd8c975a5af6a7c42df5965e0e4d1a0f2ddaa3db918072461a"), 1519241279, 0x1e0ffff0 }
};

static const BRCheckPoint BRTestNetCheckpoints[] = {
    {       0, uint256("323655a66c1c5dcd26947f06dc19ba24397a5baf0ac388757016c251373119a1"), 1296688602, 0x1d00ffff }
};

static int BRTestNetVerifyDifficulty(const BRMerkleBlock *block, const BRMerkleBlock *previous, uint32_t transitionTime)
{
    int r = 1;

    assert(block != NULL);
    assert(previous != NULL);

    if (! previous || !UInt256Eq(block->prevBlock, previous->blockHash) || block->height != previous->height + 1) r = 0;
    if (r && (block->height % BLOCK_DIFFICULTY_INTERVAL) == 0 && transitionTime == 0) r = 0;
    return r;
}

static const BRChainParams BRMainNetParams = {
    BRMainNetDNSSeeds,
    1956,      // standardPort
    0xd2b4c1fc, // magicNumber
    0,          // services
    BRMerkleBlockVerifyDifficulty,
    BRMainNetCheckpoints,
    sizeof(BRMainNetCheckpoints)/sizeof(*BRMainNetCheckpoints)
};

static const BRChainParams BRTestNetParams = {
    BRTestNetDNSSeeds,
    2014,      // standardPort
    0xf1c8d2fd, // magicNumber
    0,          // services
    BRTestNetVerifyDifficulty,
    BRTestNetCheckpoints,
    sizeof(BRTestNetCheckpoints)/sizeof(*BRTestNetCheckpoints)
};

#endif // BRChainParams_h
