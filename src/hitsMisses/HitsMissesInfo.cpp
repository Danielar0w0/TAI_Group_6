#include "HitsMissesInfo.h"

HitsMissesInfo::HitsMissesInfo(unsigned int hits, unsigned int misses) {
    this->hits = hits;
    this->misses = misses;
}

HitsMissesInfo::HitsMissesInfo() {
    this->hits = 0;
    this->misses = 0;
}

unsigned int HitsMissesInfo::getMisses() {
    return this->misses;
}

unsigned int HitsMissesInfo::getHits() {
    return this->hits;
}

void HitsMissesInfo::setMisses(unsigned int misses) {
    this->misses = misses;
}

void HitsMissesInfo::setHits(unsigned int hits) {
    this->hits = hits;
}

void HitsMissesInfo::incrementHits() {
    this->hits++;
}

void HitsMissesInfo::incrementMisses() {
    this->misses++;
}
