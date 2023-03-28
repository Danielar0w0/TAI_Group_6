#include "AbstractGenerator.h"

void AbstractGenerator::printHeader() {

    logger.info("-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-[ GENERATOR ]-=-=-==-=-=-=-=-==-=-=-=-=-=-=-=-=-");
    logger.info("");
    logger.info("Write a sentence and the generator will try to complete it.");
    logger.info("When you are done, type 'exit'.");
    logger.info("");
    logger.info("-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-==-=-=-=-=-=-=-=-=-");

}
