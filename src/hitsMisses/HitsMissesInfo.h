#ifndef SRC_HITSMISSESINFO_H
#define SRC_HITSMISSESINFO_H


class HitsMissesInfo {

    unsigned int hits;
    unsigned int misses;

    public:

        explicit HitsMissesInfo(unsigned int hits, unsigned int misses);
        explicit HitsMissesInfo();

        unsigned int getHits();
        unsigned int getMisses();

        void setHits(unsigned int hits);
        void setMisses(unsigned int misses);

        void incrementHits();
        void incrementMisses();

};


#endif //SRC_HITSMISSESINFO_H
